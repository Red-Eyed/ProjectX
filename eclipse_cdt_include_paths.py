from xml.etree import ElementTree
import os
import shutil
import sys
import argparse

xml_header = '<?xml version="1.0" encoding="UTF-8" standalone="no"?> \
<?fileVersion 4.0.0?>'

# Parse arguments

parser = argparse.ArgumentParser(description='Script that adds in .cproject include paths from a file')
parser.add_argument('--project_path', help='Paths to eclipse project')
parser.add_argument('--include_file_path', help='File path to file which contains list with includes')

args = parser.parse_args()
cproject_path = ""
include_file_path = ""

try:
    cproject_path = os.path.abspath(args.project_path)
    include_file_path = os.path.abspath(args.include_file_path)
except AttributeError:
    if not cproject_path:
        print "Wrong project path."

    if not include_file_path:
        print "Wrong Include file path."

    sys.exit(-1)

if not os.path.isfile(include_file_path):
    print "The include_file_path does not exit"
    sys.exit(-1)

if not os.path.basename(cproject_path) == ".cproject":
    cproject_path += "/.cproject"

# Making backup

cproject_path_bkp = cproject_path + ".bkp"
shutil.copy(cproject_path, cproject_path_bkp)

# Adding include paths to the .cproject

tree = ElementTree.parse(cproject_path)
root = tree.getroot()
include_paths = root.find('./storageModule/cconfiguration/storageModule/configuration/folderInfo/toolChain/tool/option')
file_incl = open(include_file_path, "r")
include_path_list = file_incl.readlines()

for include_path in include_path_list:
    include_path = include_path[:-1]
    include_paths.append(ElementTree.Element('listOptionValue', {'builtIn': 'false', 'value': include_path}))

cproject_path_new = cproject_path + ".new.xml"
xml_content = ElementTree.tostring(root)

cproject_file = open(cproject_path, "w+")
cproject_file.write(xml_header)
cproject_file.write(xml_content)
cproject_file.close()

