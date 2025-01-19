import re
from pathlib import Path
from typing import Dict, List
from collections import defaultdict

class LogSplitter:
    def __init__(self, input_file: str, output_dir: str):
        """
        初始化日志分割器
        :param input_file: 输入文件路径（02_filter.log）
        :param output_dir: 输出目录
        """
        self.input_file = Path(input_file)
        self.output_dir = Path(output_dir)
    
    def extract_process_name(self, line: str) -> str:
        """
        从日志行中提取进程名
        格式：[序号][时间][等级][进程名] 内容
        """
        pattern = r'\[\d+\]\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\]\[[A-Z]\]\[([^]]+)\]'
        match = re.match(pattern, line)
        if match:
            return match.group(1).strip()
        return "Unknown"
    
    def group_logs_by_process(self) -> Dict[str, List[str]]:
        """将日志按进程名分组"""
        process_logs = defaultdict(list)
        
        try:
            with open(self.input_file, 'r', encoding='utf-8') as f:
                for line in f:
                    process_name = self.extract_process_name(line)
                    process_logs[process_name].append(line)
            
            return process_logs
        except Exception as e:
            print(f"分组日志时出错: {str(e)}")
            return {}
    
    def save_process_logs(self, process_logs: Dict[str, List[str]]) -> bool:
        """保存分组后的日志到单独的文件"""
        try:
            for process_name, logs in process_logs.items():
                # 处理进程名中的特殊字符
                safe_name = process_name.replace('/', '_')
                output_file = self.output_dir / f"03_{safe_name}.log"
                
                print(f"正在保存 {output_file.name}...")
                with open(output_file, 'w', encoding='utf-8') as f:
                    f.writelines(logs)
            
            return True
        except Exception as e:
            print(f"保存进程日志时出错: {str(e)}")
            return False
    
    def split(self) -> bool:
        """执行日志分割"""
        try:
            print("\n=== 开始分割日志 ===")
            
            # 检查输入文件
            if not self.input_file.exists():
                print(f"输入文件 {self.input_file} 不存在")
                return False
            
            # 分组日志
            process_logs = self.group_logs_by_process()
            if not process_logs:
                print("没有找到有效的日志条目")
                return False
            
            # 显示找到的进程
            print(f"找到 {len(process_logs)} 个进程的日志:")
            for process_name in process_logs.keys():
                print(f"- {process_name}")
            
            # 保存分组后的日志
            if self.save_process_logs(process_logs):
                print("日志分割完成!")
                return True
            else:
                print("保存分割后的日志失败")
                return False
            
        except Exception as e:
            print(f"分割日志时出错: {str(e)}")
            return False