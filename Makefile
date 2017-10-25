CC=g++
RM=rm -rf
CFLAGS=-g -o3

CPP_OBJECT_MIDDLE=im_Net/
CPP_OBJECT_DIR=target/
CONFIG_DIR=./Menuconfig

LIBS_DIR=
INCLUDE_DIR=

menuconfig:
	$(MAKE) -C $(CONFIG_DIR) menuconfig
	@cp -rf $(CONFIG_DIR)/.tmpconfig.h config.h

clean:
	@$(RM) $(CPP_OBJECT_MIDDLE)
	@$(RM) $(CPP_OBJECT_DIR)/*

.PHONY:clean menuconfig
