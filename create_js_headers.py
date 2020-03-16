import glob
import sys

log = open("create_js_headers.log", "w")
filePath = sys.argv[1]
log.write("Processing path: "+filePath + "\*.min.js\n")
fileList = glob.glob(filePath + "\*.min.js")
log.write("File list:")
log.writelines(fileList)
log.write("\n")
for jsFileName in fileList:
  log.write("Processing file: " + jsFileName +"\n")
  jsName = jsFileName.split("\\")[-1]
  log.write("Remove \\: " + jsName +"\n")
  jsName = jsName.split(".")[0]
  log.write("Removing extensions: " + jsName +"\n")
  jsFd = open(jsFileName, "r")
  jsContent = jsFd.read().replace('"', "'")
  jsFd.close()
  jsHeaderFd = open(filePath + "\\"+ jsName +".h", "w")
  log.write("Processing file: " + jsName  +".h\n")
  jsHeaderFd.write("const char  *"+jsName+" = \"")
  jsHeaderFd.write(jsContent)
  jsHeaderFd.write('";\n')
  jsHeaderFd.close()
  log.write("Closed file: " + filePath + "/"+ jsName +".h"  +"\n")

log.close()