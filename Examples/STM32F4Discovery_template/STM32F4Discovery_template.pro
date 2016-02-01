TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

PRJ_PATH=$$system(pwd)
SRC_PROJECT_PATH="$$PRJ_PATH/Src"
INCLUDE_PROJECT_PATH="$$PRJ_PATH/Inc"
INCLUDE_PATH=/home/vadimstupakov/Data/STM32/STM32Cube/Repository/STM32Cube_FW_F4_V1.10.0

SOURCES = $$system(find -L $$SRC_PROJECT_PATH -type f | grep -E \".*\.(c|cpp|s|S)$\")
HEADERS = $$system(find -L $$INCLUDE_PROJECT_PATH -type f | grep -E ".*\.h$")
OTHER_FILES = $$system(find -L $$PRJ_PATH -type f | grep -Ev \".*\.(h|c|cpp|s|S)$\")
INCLUDEPATH = $$system(find -L $$PRJ_PATH -type f -regex \".*\.h$\" | xargs -I{} dirname {} | sort |uniq)
INCLUDEPATH += $$system(find -L $$INCLUDE_PATH -type f -regex \".*\.h$\" | xargs -I{} dirname {} | sort | uniq)

