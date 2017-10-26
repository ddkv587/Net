include DECLARE.mk

OBJECT		:= main

LIBS					:=
LIBS_DIR				:= $(OBJECT_CORE_DIR)

INCLUDE					:=
INCLUDE_DIR				:=

$(OBJECT):
	$(MAKE) -C $(OBJECT_CORE_DIR)

menuconfig:
	$(MAKE) -C $(CONFIG_DIR) menuconfig
	@cp -rf $(CONFIG_DIR)/.tmpconfig.h config.h

clean:
	@$(RM) $(CPP_OBJECT_MIDDLE)
	@$(RM) $(CPP_OBJECT_DIR)/*

.PHONY:clean menuconfig
