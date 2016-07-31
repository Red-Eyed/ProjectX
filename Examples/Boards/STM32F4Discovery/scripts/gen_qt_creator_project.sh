#!/bin/bash

PROJECT_PATH=$(readlink -e "$1")

if [ "$PROJECT_PATH" == "" ]; then
    echo "Enter the project path!"
    echo "exit"
    exit -1
fi

cd $PROJECT_PATH

PROJECT_NAME=$(make PROJECT_NAME_PRINT)
SRC_FILES=$(make C_SOURCES_PRINT ; make ASM_SOURCES_PRINT)
INC_DIRS=$(make C_INCLUDES_PRINT ; make ASM_INCLUDES_PRINT)
CC=$(make CC_PRINT)
CFLAGS=$(make C_FLAGS_PRINT)
DEFS=$(make C_DEFS_PRINT ; make ASM_DEFS_PRINT)
INC_FILES=$(find -L $INC_DIRS -regex ".*\.h" | sort | uniq)

printf "%s\n" $SRC_FILES    >  "$PROJECT_NAME.files"
printf "%s\n" $INC_FILES    >> "$PROJECT_NAME.files"
printf "%s\n" $INC_DIRS     >  "$PROJECT_NAME.includes"
printf "[General]\n"        >  "$PROJECT_NAME.creator"
printf "#define %s\n" $DEFS >  "$PROJECT_NAME.config"
printf -- "-I%s\n" $INC_DIRS > .clang_complete

cd -
