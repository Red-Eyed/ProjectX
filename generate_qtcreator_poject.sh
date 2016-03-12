#!/bin/bash

source ./users.config.sh

if [ "$PROJECT_PATH" == "" ] ||
   [ "$OPENOCD_CFG_FILE" == "" ]; then
    echo "Please, set variables in the users.config.sh!"
    exit -1
fi

if [ ! -d "$PROJECT_PATH" ]; then
    echo "The PROJECT_PATH var doesn't exit"
    exit -1
fi

sudo sh -c "find -L $PROJECT_PATH | xargs -I{} chown $USER:$USER {}"
find -L $PROJECT_PATH -type f | xargs -I{} chmod 666 {}

PROJECT_NAME="$(find -L "$PROJECT_PATH" -type f -regex '.*\.ioc' | sed -e 's/\(.*\)\.ioc/\1/g')"

if [ "$PROJECT_NAME" == "" ]; then
    PROJECT_NAME=$(basename "$PROJECT_PATH")
else
    PROJECT_NAME=$(basename "$PROJECT_NAME")
fi

# Generating Makefile for SW4STM32 project
./CubeMX2Makefile/CubeMX2Makefile.py "$PROJECT_PATH" "$STM32_CUBE_MX_INCLUDE_PATH"

FILES=$(make -f "$PROJECT_PATH/Makefile" C_SOURCES_PRINT ASM_SOURCES_PRINT)
INCLUDES=$(make -f "$PROJECT_PATH/Makefile" C_INCLUDES_PRINT ASM_INCLUDES_PRINT)
DEFINES=$(make -f "$PROJECT_PATH/Makefile" C_DEFS_PRINT ASM_DEFS_PRINT)

printf "%s\n" $FILES           > "$PROJECT_PATH/$PROJECT_NAME.files"
printf "%s\n" $INCLUDES        > "$PROJECT_PATH/$PROJECT_NAME.includes"
printf "[General]\n"           > "$PROJECT_PATH/$PROJECT_NAME.creator"
printf "#define %s\n" $DEFINES > "$PROJECT_PATH/$PROJECT_NAME.config"

# Removing -I from include file
sed -i "s/^-I//g" "$PROJECT_PATH/$PROJECT_NAME.includes"

# Removing -D from define file
sed -i "s/-D//g" "$PROJECT_PATH/$PROJECT_NAME.config"

# Removing "weak"
sed -i "/.*weak.*/d" "$PROJECT_PATH/Makefile"

printf "\
#!/bin/bash\n\
FIRMWARE=\$(find -L \$PWD -iregex \".*\.elf$\")\n\
openocd -f $OPENOCD_CFG_FILE -c \"program \"\$FIRMWARE\" verify reset\"\n\
" > $PROJECT_PATH/flash.sh
chmod +x $PROJECT_PATH/flash.sh

