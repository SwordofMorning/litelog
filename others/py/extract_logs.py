import paramiko
import os
import sys
from dataclasses import dataclass
from typing import Optional

@dataclass
class SSHConfig:
    host: str
    username: str
    password: str
    remote_path: str
    local_path: str

class LogExtractor:
    def __init__(self, config: SSHConfig):
        self.config = config
        self.ssh = None
        self.sftp = None

    def connect(self):
        """建立SSH连接"""
        self.ssh = paramiko.SSHClient()
        self.ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        
        try:
            print(f"正在连接到 {self.config.host}...")
            self.ssh.connect(
                hostname=self.config.host,
                username=self.config.username,
                password=self.config.password
            )
            self.sftp = self.ssh.open_sftp()
            return True
        except paramiko.AuthenticationException:
            print("认证失败：用户名或密码错误")
            return False
        except paramiko.SSHException as ssh_exception:
            print(f"SSH错误：{str(ssh_exception)}")
            return False
        except Exception as e:
            print(f"连接错误：{str(e)}")
            return False

    def disconnect(self):
        """关闭连接"""
        if self.sftp:
            self.sftp.close()
        if self.ssh:
            self.ssh.close()

    def fetch_logs(self) -> bool:
        """提取日志文件"""
        try:
            # 确保本地目录存在
            os.makedirs(self.config.local_path, exist_ok=True)
            
            # 获取远程文件列表
            print("正在获取文件列表...")
            remote_files = self.sftp.listdir(self.config.remote_path)
            
            # 下载文件
            for filename in remote_files:
                remote_file = f"{self.config.remote_path}/{filename}"
                local_file = os.path.join(self.config.local_path, filename)
                
                print(f"正在下载: {filename}")
                self.sftp.get(remote_file, local_file)
            
            print("所有文件下载完成!")
            return True
            
        except Exception as e:
            print(f"下载错误：{str(e)}")
            return False

    def extract(self) -> bool:
        """执行完整的日志提取流程"""
        try:
            if not self.connect():
                return False
            
            success = self.fetch_logs()
            
            return success
        finally:
            self.disconnect()