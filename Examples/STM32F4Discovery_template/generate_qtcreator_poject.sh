#!/bin/bash

source ./users.config.sh

PROJECT_NAME="$(ls -1 | grep '.*\.ioc' | sed -e 's/\(.*\)\.ioc/\1/g')"

if [ "$PROJECT_NAME" == "" ]; then
    echo "Please, set project name, or generate project by using STMCubeMX"
    echo "Example: basename $0 [project name]"
    exit 1
fi

sudo chown -R $USER:$USER *

PRJ_PATH=$PWD
SRC_PROJECT_PATH="$PRJ_PATH/Src"
INCLUDE_PROJECT_PATH="$PRJ_PATH/Inc"

FILES=$(find -L $SRC_PROJECT_PATH -type f | grep -E ".*\.(c|cpp|s|S)$";\
        find -L $INCLUDE_PROJECT_PATH -type f | grep -E ".*\.h$")

INCLUDES=$(find -L $PRJ_PATH -type f -regex ".*\.h$" | xargs -I{} dirname {} | sort |uniq; \
           find -L $INCLUDE_PATH -type f -regex ".*\.h$" | xargs -I{} dirname {} | sort | uniq)

printf "%s\n" $FILES > $PROJECT_NAME.files
printf "%s\n" $INCLUDES > $PROJECT_NAME.includes
printf "[General]\n" > $PROJECT_NAME.creator
printf "#define %s\n" $DEFINES > $PROJECT_NAME.config

