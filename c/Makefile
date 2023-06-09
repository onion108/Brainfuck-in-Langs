# cbfi stands for C BrainFuck Interpreter.
TARGET_EXEC ?= bin/cbfi

BUILD_DIR ?= ./build
SRC_DIR ?= ./src

SRCS := $(shell find $(SRC_DIR) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

CFLAGS ?= -O2 -std=c11
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) $(BUILD_DIR) $(BUILD_DIR)/bin
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/bin:
	mkdir $(BUILD_DIR)/bin

MKDIR_P ?= mkdir -p

