#!/bin/bash

source ./users.config.sh

if [ "$STM32_CUBE_MX_INCLUDE_PATH" == "" ] ||
   [ "$DEFINES" == "" ] ||
   [ "$PROJECT_PATH" == "" ] ||
   [ "$OPENOCD_CFG_FILE" == "" ]; then
    echo "Please, set variables in the users.config.sh!"
    exit -1
fi

if [ ! -d "$STM32_CUBE_MX_INCLUDE_PATH" ]; then
    echo "The STM32_CUBE_MX_INCLUDE_PATH var path doesn't exist."
    exit -1
elif [ ! -d "$PROJECT_PATH" ]; then
    echo "The PROJECT_PATH var doesn't exit"
    exit -1
fi

sudo chown -R $USER:$USER $PROJECT_PATH
find -L $PROJECT_PATH -type f | xargs -I{} chmod 666 {}

PROJECT_NAME="$(find -L "$PROJECT_PATH" -type f -regex '.*\.ioc' | sed -e 's/\(.*\)\.ioc/\1/g')"

if [ "$PROJECT_NAME" == "" ]; then
    PROJECT_NAME=$(basename "$PROJECT_PATH")
else
    PROJECT_NAME=$(basename "$PROJECT_NAME")
fi

FILES=$(find -L "$PROJECT_PATH" -type f | grep -E ".*(\.(c|cpp|s|S|asm|h)|Makefile)$")
INCLUDES=$(find -L $PROJECT_PATH $STM32_CUBE_MX_INCLUDE_PATH/Middlewares $STM32_CUBE_MX_INCLUDE_PATH/Drivers -type f -regex ".*\.h$" | xargs dirname | sort | uniq)

printf "%s\n" $FILES           > "$PROJECT_PATH/$PROJECT_NAME.files"
printf "%s\n" $INCLUDES        > "$PROJECT_PATH/$PROJECT_NAME.includes"
printf "[General]\n"           > "$PROJECT_PATH/$PROJECT_NAME.creator"
printf "#define %s\n" $DEFINES > "$PROJECT_PATH/$PROJECT_NAME.config"

# Generating Makefile for SW4STM32 project
./CubeMX2Makefile/CubeMX2Makefile.py "$PROJECT_PATH" "$STM32_CUBE_MX_INCLUDE_PATH"

printf "\
#!/bin/bash\n\
FIRMWARE=\$(find -L \$PWD -iregex \".*\.elf$\")\n\
openocd -f $OPENOCD_CFG_FILE -c \"program \"\$FIRMWARE\" verify reset\"\n\
" > $PROJECT_PATH/flash.sh
chmod +x $PROJECT_PATH/flash.sh

