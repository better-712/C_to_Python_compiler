import os
import shutil

# 获取当前工作目录
current_dir = os.getcwd()

# 返回上级目录
parent_dir = os.path.dirname(current_dir)

# 删除当前目录
shutil.rmtree(current_dir)

# 打印上级目录
print("Parent directory:", parent_dir)
