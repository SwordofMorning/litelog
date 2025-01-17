from extract_logs import LogExtractor, SSHConfig
from merge_logs import LogMerger
import os

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
    ssh_config = get_user_input()
    
    if not ssh_config:
        print("配置无效，程序退出")
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