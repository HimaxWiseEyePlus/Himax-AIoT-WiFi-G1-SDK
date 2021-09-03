# Application name: SDK generating tool.
# Description:      Generating SDK packge from complete project.
# Author:           904352
# Usage:            1. Put this tool under "your_proj_dir/options".
#                   2. Prepare "sdkFileList.txt" and put it under "your_proj_dir/options" as well.
#                      You can modify it directly or refer to tool: "gen_sdk_file_list.py" to 
#                      create this file.
#                   3. Excute cmd under "your_proj_dir/options": "python gen_sdk.py sdkFileList.txt".
#                   4. If no any error message, you can see the location of sdk. 
# History:          2021-09-01 Initial
##################################################################################################

# Importing module
import os
import glob
import time
import shutil
import sys

# Function declaration
def changeDir(currDir, change):
    if change == "../":
        currDir = currDir[:currDir.rfind("/")]

    return currDir

def createFolder(sdkRootDir, folderHierarchy):
    #print("[SDK GEN MSG] func createFolder called")
    folderName = folderHierarchy.split("/")

    #print(folderName)
    folderLevel=""
    #print(len(folderName))
    for idx in range(0, len(folderName)-1):
        
        folderLevel = folderLevel + "/" + folderName[idx]
        #print(folderLevel)
        #print(sdkRootDir + folderLevel)
        if not os.path.isdir(sdkRootDir + "/" + folderLevel):
            os.mkdir(sdkRootDir + folderLevel)
            #print("create new folder: " + sdkRootDir + folderLevel)

        #print("=================")
    return folderName

# root directory of this project
root_dir = "../"
missFiles=[] # to record the missing files

curr_working_dir = os.path.abspath(os.getcwd()).replace("\\", "/")
root_working_dir = changeDir(curr_working_dir, "../")
sdk_out_folder_name = "sdk_output"
sdk_out_folder_dir = changeDir(root_working_dir, "../") + "/" + sdk_out_folder_name

print("[SDK GEN MSG] Script dir:     " + curr_working_dir)
print("[SDK GEN MSG] Project dir:    " + root_working_dir)
print("[SDK GEN MSG] SDK output dir: " + sdk_out_folder_dir)
# check the current OS, "nt" mean Windows
# print("++++++++++++++++++++")
# print("Current OS is: " + os.name)
# print("++++++++++++++++++++")

#####os.mkdir("D:/Dev/Temp/SDK_GEN/sdk_output/test")

# Check if SDK list file existed
sdkFileList = str(sys.argv[1])
if os.path.isfile(sdkFileList):
    print("[SDK GEN MSG] Load SDK list file: [" + sdkFileList + "] sucessfully!")


# Create new SDK folder for put all necessary files.
if os.path.isdir(sdk_out_folder_dir):
    print("[SDK GEN MSG] sdk output folder is existed, which is on: " + sdk_out_folder_dir)
else:
    print("[SDK GEN MSG] sdk output folder isn't existed, create new on: " + sdk_out_folder_dir)
    os.mkdir(sdk_out_folder_dir)



#Read line from SDK list file
file = open(sdkFileList, 'r', encoding='UTF-8')
try:
    for line in file:
        print("[SDK GEN MSG] Read line: [" + line + "] from list file.\n", end='')
        srcDir = root_working_dir + "/" + line
        dstDir = sdk_out_folder_dir + "/" + line

        # Remove newlines /n from string.
        srcDir = srcDir.strip()
        dstDir = dstDir.strip()

        print("[SDK GEN MSG] Src: [" + srcDir + "]")
        print("[SDK GEN MSG] Dst: [" + dstDir + "]")

        if not os.path.isfile(srcDir):
            print(srcDir + " isn't exist, do not copy this file. Recommend to check this file individually")
            missFiles.append(srcDir)
            continue

        # Create folders on destination.
        createFolder(sdk_out_folder_dir, line)
    
        # Copy files from src to dst.
        shutil.copyfile(srcDir, dstDir)

        print("------------------------------------------------------")

except Exception as e:
    print(e)
finally:
    file.close()




for file in missFiles:
    print(file+"\n")

print("[SDK GEN MSG] " + str(len(missFiles)) + " files are missing from src project!")
print("[SDK GEN MSG] Recommond to check this file individually.")
print("\n")
print("================================================================================")
print("[SDK GEN MSG] SDK generate done on: " + sdk_out_folder_dir)
print("================================================================================")


