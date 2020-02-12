import glob
import sys

log = open("create_css_headers.log", "w")
filePath = sys.argv[1]
log.write("Processing path: "+filePath + "\*.min.css\n")
fileList = glob.glob(filePath + "\*.min.css")
log.write("File list:")
log.writelines(fileList)
log.write("\n")
for cssFileName in fileList:
  log.write("Processing file: " + cssFileName +"\n")
  cssName = cssFileName.split("\\")[-1]
  log.write("Remove \\: " + cssName +"\n")
  cssName = cssName.split(".")[0]
  log.write("Removing extensions: " + cssName +"\n")
  cssFd = open(cssFileName, "r")
  cssContent = cssFd.read().replace('"', "'")
  cssFd.close()
  cssHeaderFd = open(filePath + "\\"+ cssName +".h", "w")
  log.write("Processing file: " + cssName  +".h\n")
  cssHeaderFd.write("const char  *"+cssName+" = \"")
  cssHeaderFd.write(cssContent)
  cssHeaderFd.write('";\n')
  cssHeaderFd.close()
  log.write("Closed file: " + filePath + "/"+ cssName +".h"  +"\n")

log.close()