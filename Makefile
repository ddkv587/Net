include DECLARE.mk

OBJECT		:= main
CFLAGS 		+= 

CUR_DIR					:= $(shell pwd)
OBJECT_CORE_DIR			:= $(CUR_DIR)/Core
OBJECT_PROTOCOL_DIR		:= $(CUR_DIR)/Protocol
OBJECT_SERVER_DIR		:= $(CUR_DIR)/Server
OBJECT_THREADPOOL_DIR	:= $(CUR_DIR)/ThreadPool
CONFIG_DIR				:= $(CUR_DIR)/Menuconfig

TARGET_OBJECT_MIDDLE	:= $(CUR_DIR)/im_Net/
TARGET_OBJECT_DIR		:= $(CUR_DIR)/target/
	
export TARGET_OBJECT_DIR TARGET_OBJECT_MIDDLE

INCLUDE					:=
INCLUDE_DIR				:= $(OBJECT_CORE_DIR)

LIBS					:= -lCore
LIBS_DIR				:= -L$(TARGET_OBJECT_DIR)

$(OBJECT):
	$(MKDIR) $(TARGET_OBJECT_DIR)
	$(MKDIR) $(TARGET_OBJECT_MIDDLE)
	$(MAKE) -C $(OBJECT_CORE_DIR) -e
	$(CC) $(CFLAGS) -Wl,-rpath=.  $(LIBS_DIR) -I$(OBJECT_CORE_DIR) $@.cpp -o $@ $(LIBS)
	$(MV) $(OBJECT) $(TARGET_OBJECT_DIR) 

menuconfig:
	$(MAKE) -C $(CONFIG_DIR) menuconfig
	$(CP) $(CONFIG_DIR)/.tmpconfig.h config.h

clean:
	@$(RM) $(TARGET_OBJECT_DIR)
	@$(RM) $(TARGET_OBJECT_MIDDLE)
	@$(RM) $(OBJECT_CORE_DIR)/*.o
	@$(RM) $(OBJECT_CORE_DIR)/*.so

.PHONY:clean menuconfig
