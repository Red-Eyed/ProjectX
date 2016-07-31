#!/bin/bash
echo "GDB server port: localhost:3333"
echo ""

openocd -f /usr/share/openocd/scripts/board/stm32f4discovery.cfg
