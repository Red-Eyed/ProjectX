#!/bin/bash
FIRMWARE=$(find -L $PWD -iregex ".*\.elf$")
openocd -f /usr/share/openocd/scripts/board/stm32f4discovery.cfg -c "program "$FIRMWARE" verify reset exit"
