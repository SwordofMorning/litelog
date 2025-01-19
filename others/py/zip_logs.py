import zipfile
from pathlib import Path
from datetime import datetime
import os

class LogArchiver:
    def __init__(self, source_dir: str, backup_dir: str):
        """
        初始化日志压缩器
        :param source_dir: 源目录（output目录）
        :param backup_dir: 备份目录
        """
        self.source_dir = Path(source_dir)
        self.backup_dir = Path(backup_dir)
        
    def get_archive_name(self) -> str:
        """生成压缩包文件名：yyyy-mm-dd_hh-mm-ss.zip"""
        current_time = datetime.now()
        return current_time.strftime("%Y-%m-%d_%H-%M-%S.zip")
    
    def create_backup_dir(self) -> bool:
        """创建备份目录"""
        try:
            self.backup_dir.mkdir(exist_ok=True)
            return True
        except Exception as e:
            print(f"创建备份目录失败: {str(e)}")
            return False
            
    def archive_logs(self) -> bool:
        """压缩日志文件"""
        try:
            print("\n=== 开始压缩日志 ===")
            
            # 检查源目录
            if not self.source_dir.exists():
                print(f"源目录 {self.source_dir} 不存在")
                return False
                
            # 创建备份目录
            if not self.create_backup_dir():
                return False
            
            # 获取所有日志文件
            log_files = list(self.source_dir.glob("*.log"))
            if not log_files:
                print("没有找到需要压缩的日志文件")
                return False
                
            # 创建压缩包
            archive_name = self.get_archive_name()
            archive_path = self.backup_dir / archive_name
            
            print(f"找到 {len(log_files)} 个日志文件")
            print(f"正在创建压缩包: {archive_name}")
            
            with zipfile.ZipFile(archive_path, 'w', zipfile.ZIP_DEFLATED) as zipf:
                for log_file in log_files:
                    print(f"正在压缩: {log_file.name}")
                    zipf.write(log_file, log_file.name)
            
            print(f"压缩完成，文件已保存到: {archive_path}")
            return True
            
        except Exception as e:
            print(f"压缩日志时出错: {str(e)}")
            return False