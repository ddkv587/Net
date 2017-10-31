include DECLARE.mk

OBJECT		:= main
CFLAGS 		+= 

LIBS					:= -lCore
LIBS_DIR				:= $(OBJECT_CORE_DIR)

INCLUDE					:=
INCLUDE_DIR				:= $(OBJECT_CORE_DIR)

$(OBJECT):
	$(MAKE) -C $(OBJECT_CORE_DIR)
	$(CC) $(CFLAGS) -L$(LIBS_DIR) $(LIBS) -I$(OBJECT_CORE_DIR) $@.cpp -o $@

menuconfig:
	$(MAKE) -C $(CONFIG_DIR) menuconfig
	@cp -rf $(CONFIG_DIR)/.tmpconfig.h config.h

clean:
	@$(RM) $(OBJECT_CORE_DIR)/*.o
	@$(RM) $(OBJECT_CORE_DIR)/*.so

.PHONY:clean menuconfig
