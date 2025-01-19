from extract_logs import LogExtractor, SSHConfig
from merge_logs import LogMerger
from split_logs import LogSplitter
from zip_logs import LogArchiver
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
    print("\n=== 日志处理工具 ===")
    print("版本: 1.0")
    print("功能: 提取、合并、分类处理日志文件")
    
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
        
        # 步骤1: 提取日志
        print("\n=== 步骤1: 提取日志 ===")
        print(f"正在连接到 {ssh_config['host']}...")
        extractor = LogExtractor(config)
        
        if not extractor.extract():
            print("日志提取失败!")
            input("\n按回车键退出程序...")
            return
        print("日志提取成功!")
        
        # 步骤2: 合并日志
        print("\n=== 步骤2: 合并日志 ===")
        merger = LogMerger(
            log_dir="logs",
            output_dir="output"
        )
        
        if not merger.merge_logs():
            print("日志合并失败!")
            input("\n按回车键退出程序...")
            return
        print("日志合并成功!")
        
        # 步骤3: 分割日志
        print("\n=== 步骤3: 分割日志 ===")
        splitter = LogSplitter(
            input_file="output/02_filter.log",
            output_dir="output"
        )
        
        if not splitter.split():
            print("日志分割失败!")
            input("\n按回车键退出程序...")
            return
        print("日志分割成功!")
        
        # 步骤4: 压缩日志
        print("\n=== 步骤4: 压缩日志 ===")
        archiver = LogArchiver(
            source_dir="output",
            backup_dir="backup"
        )
        
        if not archiver.archive_logs():
            print("日志压缩失败!")
            input("\n按回车键退出程序...")
            return
        print("日志压缩成功!")
        
        # 总结处理结果
        print("\n=== 处理完成 ===")
        print("1. 日志已从设备提取到 logs 目录")
        print("2. 合并的日志位于 output/01_merge.log")
        print("3. 过滤后的日志位于 output/02_filter.log")
        print("4. 按进程分类的日志位于 output/03_*.log")
        print("5. 压缩包已保存到 backup 目录")
            
    except Exception as e:
        print(f"\n程序执行出错: {str(e)}")
    finally:
        print("\n提示：按回车键退出程序...")
        print("      输出文件位于当前目录的 output 文件夹中")
        print("      压缩包位于当前目录的 backup 文件夹中")
        input()

if __name__ == "__main__":
    main()