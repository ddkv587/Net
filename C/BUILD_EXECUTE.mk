#build execute

BUILD_MIDDLE_PATH 	:= $(IM_PATH)/$(TARGET_MODULE)

CPP_APP_OBJECT		:= $(foreach filename, $(CPP_SRC:%.cpp=%.o), $(subst $(CPP_SRCDIR), $(BUILD_MIDDLE_PATH), $(filename)))

all: $(TARGET_MODULE)

$(CPP_APP_OBJECT): $(BUILD_MIDDLE_PATH)%.o: $(CPP_SRCDIR)%.cpp
	@echo $(notdir $<)
	@$(MKDIR) $(dir $@)
	$(CXX) -c $(CPP_INCLUDE) $(CPPFLAGS) -o $@ $<

$(TARGET_MODULE): $(OBJ_MODULE) 
	$(CXX) -o $@ $(OBJ_MODULE) $(CPP_SRC) $(CPP_INCLUDE) $(CPP_FLAGS) $(LIBS_DIR) $(LIBS) -Wl,-rpath,.

clean:
	@$(RM) $(BUILD_MIDDLE_PATH)
	@$(RM) $(TARGET_MODULE)

.PHONY: all clean debug release
