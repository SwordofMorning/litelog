import os
from pathlib import Path
import re
from datetime import datetime

class LogMerger:
    def __init__(self, log_dir: str, output_dir: str):
        self.log_dir = Path(log_dir)
        self.output_dir = Path(output_dir)
        self.output_file = self.output_dir / "01_merge.log"
        self.filter_file = self.output_dir / "02_filter.log"
    
    def get_timestamp_from_filename(self, filename: str) -> datetime:
        """从文件名中提取时间戳"""
        pattern = r'litelog_(\d{4}-\d{2}-\d{2}_\d{2}-\d{2}-\d{2})\.log'
        match = re.match(pattern, filename)
        if match:
            time_str = match.group(1)
            return datetime.strptime(time_str, '%Y-%m-%d_%H-%M-%S')
        return datetime.min
    
    def get_sorted_log_files(self):
        """获取按文件名时间戳排序的日志文件列表"""
        files = [f for f in self.log_dir.glob("*.log") 
                if f.is_file() and f.name.startswith('litelog_')]
        
        sorted_files = sorted(files, key=lambda x: self.get_timestamp_from_filename(x.name))
        
        print(f"找到{len(sorted_files)}个日志文件：")
        for f in sorted_files:
            print(f"- {f.name}")
        
        return sorted_files
    
    def is_valid_log_line(self, line: str) -> bool:
        """判断是否为有效的日志行"""
        # 标准日志行格式：[序号][时间][等级][时间戳/进程名] 内容
        pattern = r'^\[\d+\]\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\]\[[A-Z]\]\[.+?\]'
        return bool(re.match(pattern, line.strip()))
    
    def is_control_line(self, line: str) -> bool:
        """判断是否为控制信息行"""
        control_patterns = [
            r'===== litlog .* =====',
            r'\[\s*Branch\s*\]',
            r'\[\s*User\s*\]',
            r'\[ Last Commit ID \]',
            r'\[\s*Startup time\s*\]'
        ]
        return any(bool(re.match(pattern, line.strip())) for pattern in control_patterns)
    
    def clean_header(self) -> bool:
        """清理日志文件的控制信息"""
        try:
            if not self.output_file.exists():
                print(f"源文件 {self.output_file} 不存在")
                return False
            
            print("开始清理控制信息...")
            cleaned_lines = []
            with open(self.output_file, 'r', encoding='utf-8') as infile:
                for line in infile:
                    # 跳过空行和控制信息行
                    if not line.strip() or self.is_control_line(line):
                        continue
                    # 保留有效的日志行
                    if self.is_valid_log_line(line):
                        cleaned_lines.append(line)
            
            with open(self.filter_file, 'w', encoding='utf-8') as outfile:
                outfile.writelines(cleaned_lines)
            
            file_size = os.path.getsize(self.filter_file)
            print(f"日志清理完成，输出文件大小：{file_size} bytes")
            return True
            
        except Exception as e:
            print(f"清理控制信息时发生错误：{str(e)}")
            return False
    
    def merge_logs(self) -> bool:
        """合并日志文件"""
        try:
            self.output_dir.mkdir(exist_ok=True)
            
            log_files = self.get_sorted_log_files()
            if not log_files:
                print("未找到日志文件")
                return False
            
            print("开始合并日志文件...")
            with open(self.output_file, 'w', encoding='utf-8') as outfile:
                for file in log_files:
                    print(f"处理文件: {file.name}")
                    with open(file, 'r', encoding='utf-8') as infile:
                        outfile.write(infile.read())
                    outfile.write("\n")
            
            file_size = os.path.getsize(self.output_file)
            print(f"日志合并完成，输出文件大小：{file_size} bytes")
            
            # 合并完成后执行清理
            return self.clean_header()
            
        except Exception as e:
            print(f"合并日志时发生错误：{str(e)}")
            return False