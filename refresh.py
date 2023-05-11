import os
import shutil


current_dir = os.getcwd()


parent_dir = os.path.dirname(current_dir)


shutil.rmtree(current_dir)


os.chdir(parent_dir)


os.system("git clone https://github.com/better-712/com_project")

os.chdir("com_project")
