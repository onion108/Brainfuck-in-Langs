# cppbfi stands for C++ BrainFuck Interpreter.
TARGET_EXEC ?= bin/cppbfi

BUILD_DIR ?= ./build
SRC_DIR ?= ./src

SRCS := $(shell find $(SRC_DIR) -name *.cc)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

CXXFLAGS ?= -O2 -std=c++2b -v
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) $(BUILD_DIR) $(BUILD_DIR)/bin
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.cc.o: %.cc
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/bin:
	mkdir $(BUILD_DIR)/bin

MKDIR_P ?= mkdir -p

