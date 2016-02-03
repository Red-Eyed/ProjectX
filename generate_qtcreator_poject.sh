#!/bin/bash

source ./users.config.sh

if [ "$STM32_CUBE_MX_INCLUDE_PATH" == "" ] ||
   [ "$DEFINES" == "" ] ||
   [ "$PROJECT_PATH" == "" ]; then
   echo "Please, set variables in the users.config.sh!"
   exit -1
fi

PROJECT_NAME="$(find -L "$PROJECT_PATH" -type f -regex '.*\.ioc' | sed -e 's/\(.*\)\.ioc/\1/g')"
PROJECT_NAME="$(basename \"$PROJECT_NAME\")"

if [ "$PROJECT_NAME" == "" ]; then
    PROJECT_NAME="$(basename \"$PROJECT_PATH\")"
fi

FILES=$(find -L $PROJECT_PATH -type f | grep -E ".*\.(c|cpp|s|S|asm|h)$")
INCLUDES=$(find -L $PROJECT_PATH $STM32_CUBE_MX_INCLUDE_PATH -type f -regex ".*\.h$" | xargs dirname | sort | uniq)

printf "%s\n" $FILES           > "$PROJECT_PATH/$PROJECT_NAME.files"
printf "%s\n" $INCLUDES        > "$PROJECT_PATH/$PROJECT_NAME.includes"
printf "[General]\n"           > "$PROJECT_PATH/$PROJECT_NAME.creator"
printf "#define %s\n" $DEFINES > "$PROJECT_PATH/$PROJECT_NAME.config"

