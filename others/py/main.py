from extract_logs import LogExtractor, SSHConfig
from merge_logs import LogMerger
import os

def main():
    # 配置参数
    config = SSHConfig(
        host="192.168.121.91",
        username="root",
        password="1",
        remote_path="/var/run/litelog",
        local_path="logs"
    )
    
    # 创建提取器实例
    extractor = LogExtractor(config)
    
    # 执行提取
    if extractor.extract():
        print("日志提取成功!")
        
        # 创建日志合并器
        merger = LogMerger(
            log_dir="logs",
            output_dir="output"
        )
        
        # 执行合并
        if merger.merge_logs():
            print("日志合并成功!")
        else:
            print("日志合并失败!")
    else:
        print("日志提取失败!")

if __name__ == "__main__":
    main()