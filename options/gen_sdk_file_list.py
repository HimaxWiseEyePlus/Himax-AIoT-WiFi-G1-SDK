# Application name: SDK file list generator
# Description:      Collect SDK file list from SDK package so that user can use this list to generate
#                   SDK from complete project.
# Author:           904352
# Usage:            1. Put this tool under "your_proj_dir/options" and make sure "gmake clean" first.
#                   2. Excute cmd under "your_proj_dir/options": "python gen_sdk_file_list.py".
#                   3. "sdkFileList" is generated and put under the same folder.
#                   4. Use tool "gen_sdk.py" with "sdkFileList" to finish SDK generation. 
# History:          2021-09-01 Initial
##################################################################################################

# Importing the os module library
import os
import glob
import time
import shutil

# root directory of this project
root_dir = "../"

# File to list all files in SDK package
sdkListFile = "sdkFileList.txt"

# Check if old SDK list file was existed, if yes, delect it first
print("/" + sdkListFile)
if os.path.isfile(sdkListFile):
    os.remove(sdkListFile)

# Start to scan & generate SDK file list
print("Start generating SDK file list...")
output_file = open(sdkListFile, "w")

for path, subdirs, files in os.walk(root_dir):
    for name in files:
        #print(os.path.join(path, name))
        
        # no need to add hidden files
        if os.path.join(path, name).find("../.") == -1:
            # use backslash("\") as seperator
            output_file.write(os.path.join(path, name)[3:].replace("\\", "/") + '\n')
            
            # use slash("/") as seperator
            #output_file.write(os.path.join(path, name)[3:] + '\n')

            print(os.path.join(path, name)[3:].replace("\\", "/"))
            
output_file.close()

print("++++++++++++++++++++")
print("List generation is done !!")
print("List location: " + os.path.dirname(os.path.abspath(__file__)).replace("\\", "/") + "/" + sdkListFile)
print("++++++++++++++++++++")