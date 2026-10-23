target ?= debug

CC = gcc
ifeq ($(target), release)
	CFLAGS := -Wall -O3
	LDFLAGS := $(CFLAGS)
else
	CFLAGS := -Wall -O0 -g
	LDFLAGS := $(CFLAGS)
endif

EXE = main.out

BUILD_DIR := ./build
SRC_DIR := ./src
SEARCH_DEPTH := 10
EXTRA_CFLAGS := 
EXTRA_LDFLAGS := -lm 

SRCS := $(shell find $(SRC_DIR) -maxdepth $(SEARCH_DEPTH) -name "*.c") # find source
OBJS := $(SRCS:%.c=$(BUILD_DIR)/$(target)/%.o) # gen object list
DEPS := $(OBJS:.o=.d) # map autogen dependencies to object

CFLAGS += -MMD -MP $(EXTRA_CFLAGS)
LDFLAGS += $(EXTRA_LDFLAGS)

.PHONY: all
all: $(EXE)

.PHONY: $(EXE)
$(EXE): $(BUILD_DIR)/$(target)/$(EXE) | CHECK_TARGET
	ln -sf $< $@
	@echo "--------------------------------------------------------------------------------"
	@echo "done!"

# linker
$(BUILD_DIR)/$(target)/$(EXE): $(OBJS) | CHECK_TARGET
	$(CC) -o $@ $(LDFLAGS) $^ 

# compile objects
$(BUILD_DIR)/$(target)/%.o: %.c | CHECK_TARGET
	@mkdir -p $(BUILD_DIR)/$(target)/$(shell dirname $<)
	$(CC) -o $@ $(CFLAGS) -c $<

-include $(DEPS)

.PHONY: CHECK_TARGET
CHECK_TARGET:
ifneq ($(target),release) 
ifneq ($(target),debug)
	@echo "Invalid configuration target: '"$(target)"'"
	@echo "Available targets are: 'release' and 'debug'"
	@exit 1
endif
endif
	@echo "Configuration target: "$(target)
	@echo "--------------------------------------------------------------------------------"
	

.PHONY: clean
clean:
	rm -fr $(BUILD_DIR)/$(target) $(EXE)
	-rmdir $(BUILD_DIR)

.PHONY: clean_all
clean_all:
	rm -fr $(BUILD_DIR) $(EXE)

.PHONY: run
run: $(EXE)
	./$(EXE) 

.PHONY: help
help:
	@echo "Makefile to build C projects"
	@echo "Automaticaly search all the .c file in "$(SRC_DIR)" dir to a max depth of "$(SEARCH_DEPTH)" and build the project in the "$(BUILD_DIR)"/[target] dir"
	@echo ""
	@echo "Usage: make [target=(debug|release)] COMMAND"
	@echo "    target              target of the compilation"
	@echo ""
	@echo "    COMMAND"
	@echo "        all             build the target"
	@echo "        clean           remove the target build dir"
	@echo "        clean_all       remove all the build dir (release and debug)"
	@echo "        build_clean     build the target and remove the target buld dir"
	@echo "        run             build and run the target"
	@echo "        help            print this target"

