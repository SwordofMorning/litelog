from extract_logs import LogExtractor, SSHConfig
from merge_logs import LogMerger
import os
import shutil
from pathlib import Path

def clean_directory(dir_path: str) -> bool:
    """清理指定目录下的所有文件"""
    try:
        dir_path = Path(dir_path)
        if dir_path.exists():
            # 如果目录存在，删除目录及其内容
            shutil.rmtree(dir_path)
        # 重新创建空目录
        dir_path.mkdir(exist_ok=True)
        return True
    except Exception as e:
        print(f"清理目录 {dir_path} 时出错: {str(e)}")
        return False

def clean_working_directories() -> bool:
    """清理工作目录"""
    print("\n=== 清理工作目录 ===")
    directories = ['logs', 'output']
    
    for dir_path in directories:
        path = Path(dir_path)
        if path.exists():
            # 检查目录是否包含文件
            files = list(path.glob('*'))
            if files:
                print(f"发现 {dir_path} 目录下有 {len(files)} 个文件，正在清理...")
                if not clean_directory(dir_path):
                    return False
            else:
                print(f"{dir_path} 目录为空")
        else:
            print(f"创建 {dir_path} 目录...")
            path.mkdir(exist_ok=True)
    
    print("目录清理完成")
    return True

def get_user_input():
    """获取用户输入的SSH连接信息"""
    print("\n=== SSH连接配置 ===")
    host = input("请输入目标主机IP (例如 192.168.121.91): ").strip()
    username = input("请输入用户名 (默认 root): ").strip() or "root"
    password = input("请输入密码: ").strip()
    
    # 验证输入
    if not host:
        print("错误: IP地址不能为空")
        return None
    if not password:
        print("错误: 密码不能为空")
        return None
    
    return {
        "host": host,
        "username": username,
        "password": password
    }

def main():
    # 首先清理工作目录
    if not clean_working_directories():
        print("目录清理失败，程序退出")
        input("\n按回车键退出程序...")
        return
        
    # 获取用户输入
    ssh_config = get_user_input()
    
    if not ssh_config:
        print("配置无效，程序退出")
        input("\n按回车键退出程序...")
        return
    
    try:
        # 配置参数
        config = SSHConfig(
            host=ssh_config["host"],
            username=ssh_config["username"],
            password=ssh_config["password"],
            remote_path="/var/run/litelog",
            local_path="logs"
        )
        
        # 创建提取器实例
        print(f"\n正在连接到 {ssh_config['host']}...")
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
            
    except Exception as e:
        print(f"程序执行出错: {str(e)}")
    
    # 等待用户确认后退出
    input("\n按回车键退出程序...")

if __name__ == "__main__":
    main()