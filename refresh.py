import os
import shutil

# 获取当前工作目录
current_dir = os.getcwd()

# 返回上级目录
parent_dir = os.path.dirname(current_dir)

# 删除当前目录
shutil.rmtree(current_dir)


# 进入上级目录
os.chdir(parent_dir)

# 进行 git clone 操作
os.system("git clone https://github.com/better-712/com_project")
