# combo driver: UWE5622
# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.

export CONFIG_SPARD_WLAN_SUPPORT=y
export CONFIG_SC23XX=y
export CONFIG_AW_WIFI_DEVICE_UWE5622=y

#temporary set it to no because not AW platform
export CONFIG_AW_BIND_VERIFY=n

export CONFIG_WLAN_UWE5622=m
export CONFIG_TTY_OVERY_SDIO=m

export UNISOCWCN_DIR=$(shell pwd)/unisocwcn/
export UNISOC_BSP_INCLUDE=$(UNISOCWCN_DIR)/include

#ccflags-y += -DDEBUG
ccflags-y += -I$(UNISOCWCN_DIR)/../tty-sdio -I$(UNISOCWCN_DIR)/../tty-sdio/alicgnment
ccflags-y += -I$(UNISOCWCN_DIR)/boot -I$(UNISOCWCN_DIR)/include -I$(UNISOCWCN_DIR)/pcie -I$(UNISOCWCN_DIR)/boot  
ccflags-y += -I$(UNISOCWCN_DIR)/platform -I$(UNISOCWCN_DIR)/platform/rf -I$(UNISOCWCN_DIR)/platform/gnss
ccflags-y += -I$(UNISOCWCN_DIR)/usb -I$(UNISOCWCN_DIR)/pcie -I$(UNISOCWCN_DIR)/sdio
ccflags-y += -I$(UNISOCWCN_DIR)/sipc -I$(UNISOCWCN_DIR)/sleep
ccflags-y += -I$(UNISOCWCN_DIR)/../unisocwifi

export UNISOC_FW_PATH_CONFIG="/lib/firmware/"

ifneq ($(KERNELRELEASE),)

obj-$(CONFIG_AW_WIFI_DEVICE_UWE5622) += unisocwcn/
obj-$(CONFIG_WLAN_UWE5622)    += unisocwifi/
obj-$(CONFIG_TTY_OVERY_SDIO)  += tty-sdio/

# Otherwise we were called directly from the command
# line; invoke the kernel build system.
else
    KSRC ?= /lib/modules/$(shell uname -r)/build
    KERNELDIR := $(KSRC)
    PWD  := $(shell pwd)
default:
	@echo $(UNISOC_BSP_INCLUDE)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean: driver_clean

driver_clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

endif
