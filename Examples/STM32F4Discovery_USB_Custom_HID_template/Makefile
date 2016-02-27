######################################
# Makefile by CubeMX2Makefile.py
######################################

######################################
# target
######################################
TARGET = STM32F4Discovery_USB_Custom_HID_template

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -O0

#######################################
# pathes
#######################################
# source path
# Build path
BUILD_DIR = build
PRJ_PATH = /home/vadimstupakov/GIT/ProjectX/Examples/STM32F4Discovery_USB_Custom_HID_template
REPO_PATH = /home/vadimstupakov/Data/STM32/STM32Cube/Repository/STM32Cube_FW_F4_V1.10.0

######################################
# source
######################################
C_SOURCES =  \
    $(PRJ_PATH)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
    $(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c \
    $(PRJ_PATH)/Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Src/usbd_customhid.c \
    $(PRJ_PATH)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
    $(PRJ_PATH)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
    $(PRJ_PATH)/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c \
    $(PRJ_PATH)/Src/gpio.c \
    $(PRJ_PATH)/Src/main.c \
    $(PRJ_PATH)/Src/stm32f4xx_hal_msp.c \
    $(PRJ_PATH)/Src/stm32f4xx_it.c \
    $(PRJ_PATH)/Src/usb_device.c \
    $(PRJ_PATH)/Src/usbd_conf.c \
    $(PRJ_PATH)/Src/usbd_custom_hid_if.c \
    $(PRJ_PATH)/Src/usbd_desc.c 
  
ASM_SOURCES =  \
    $(PRJ_PATH)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f407xx.s 


#######################################
# binaries
#######################################
CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc -x assembler-with-cpp
CP = arm-none-eabi-objcopy
AR = arm-none-eabi-ar
SZ = arm-none-eabi-size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# macros for gcc
ASM_DEFS = \

C_DEFS = \
    -D__packed="__attribute__\(\(__packed__\)\)" \
    -DUSE_HAL_DRIVER \
    -DSTM32F407xx 

# includes for gcc
ASM_INCLUDES = 

C_INCLUDES = \
    -I$(PRJ_PATH)/Inc \
    -I$(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Inc \
    -I$(PRJ_PATH)/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy \
    -I$(PRJ_PATH)/Middlewares/ST/STM32_USB_Device_Library/Core/Inc \
    -I$(PRJ_PATH)/Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Inc \
    -I$(PRJ_PATH)/Drivers/CMSIS/Include \
    -I$(PRJ_PATH)/Drivers/CMSIS/Device/ST/STM32F4xx/Include 

# compile gcc flags
ASFLAGS = -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS = -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif
# Generate dependency information
CFLAGS += -MD -MP -MF .dep/$(@F).d

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F407VGTx_FLASH.ld
# libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir -p $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***

