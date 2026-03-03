#!/usr/bin/env python3
"""
批量删除当前目录下的 build 文件夹
"""

import os
import shutil
import sys

def remove_build_folders():
    """删除当前目录及其子目录中的所有 build 文件夹"""
    current_dir = os.getcwd()
    removed_count = 0
    error_count = 0
    
    print(f"正在扫描目录: {current_dir}")
    print("=" * 50)
    
    # 遍历当前目录及其所有子目录
    for root, dirs, files in os.walk(current_dir, topdown=False):
        if 'build' in dirs:
            build_path = os.path.join(root, 'build')
            try:
                # 检查是否是目录
                if os.path.isdir(build_path):
                    # 删除 build 文件夹及其所有内容
                    shutil.rmtree(build_path)
                    print(f"✓ 已删除: {build_path}")
                    removed_count += 1
                else:
                    print(f"⚠ 不是目录: {build_path}")
                    error_count += 1
            except PermissionError:
                print(f"✗ 权限不足: {build_path}")
                error_count += 1
            except Exception as e:
                print(f"✗ 删除失败 {build_path}: {e}")
                error_count += 1
    
    print("=" * 50)
    print(f"操作完成!")
    print(f"成功删除: {removed_count} 个 build 文件夹")
    print(f"失败: {error_count} 个")
    
    if removed_count == 0 and error_count == 0:
        print("未找到任何 build 文件夹")

if __name__ == "__main__":
    # 询问确认
    response = input("确定要删除当前目录及其子目录中的所有 build 文件夹吗？(y/N): ")
    
    if response.lower() in ['y', 'yes', '是']:
        remove_build_folders()
    else:
        print("操作已取消")