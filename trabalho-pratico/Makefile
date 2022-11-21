# Recursive wildcard 
# The first parameter ($1) is a list of directories, and the second ($2) is a list of patterns to match
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

# Default to release build
# Override with `make BUILD=debug` or `make BUILD=sanitizer`
BUILD := release

# Compiler
CC = gcc

# Flags
FLAGS.release = -Wall -Wextra -Wdouble-promotion -Werror=pedantic -Werror=vla -pedantic-errors -Wfatal-errors -flto -O2 -march=native -mtune=native
FLAGS.sanitizer = -Wall -Wextra -Wdouble-promotion -Werror=pedantic -Werror=vla -pedantic-errors -Wfatal-errors -g -fsanitize=address
FLAGS.debug = -Wall -Wextra -Wdouble-promotion -Werror=pedantic -Werror=vla -pedantic-errors -Wfatal-errors -O0 -g
FLAGS := $(FLAGS.$(BUILD))

# Directories
OBJ_DIR := build
RESULT_DIR := Resultados

# Get sources, headers and object files
SRCS = $(call rwildcard,src,*.c)
HEADERS = $(call rwildcard,includes,*.h)
OBJS = $(SRCS:src/%.c=$(OBJ_DIR)/%.o) #$(SRCS:.c=.o)

# Other stuff to compile with
LIBS = -lm -lglib-2.0
INCLUDES = -Iincludes
PKG_CONFIG = `pkg-config --cflags --libs glib-2.0`
TARGET = programa-principal

# Pretty stuff
NO_COLOR = \033[m
LINKING_COLOR = \033[0;33m
COMPILING_COLOR = \033[0;33m
OK_COLOR = \033[0;32m
DELETING_COLOR = \033[0;31m
COMPILING_STRING = "[COMPILING]"
LINKING_STRING = "[LINKING]"
DELETING_STRING = "[DELETING]"
OK_STRING = "[OK]"


$(OBJ_DIR)/%.o: src/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@printf "%b" "$(LINKING_COLOR)$(LINKING_STRING) $(NO_COLOR)$@\n";
	@$(CC) $(FLAGS) -c -o $@ $(INCLUDES) $(PKG_CONFIG) $< $(LIBS)


$(TARGET): $(OBJS)
	@printf "%b" "$(COMPILING_COLOR)$(COMPILING_STRING) $(NO_COLOR)$@\n";
	@$(CC) $(FLAGS) -o $@ $(INCLUDES) $(PKG_CONFIG) $^ $(LIBS)
	@mkdir -p $(RESULT_DIR)
	@printf "%b" "$(OK_COLOR)$(OK_STRING) $(NO_COLOR)\n";

# Remove the object directory (and its contents) and main-program
.PHONY: clean
clean:
	@printf "%b" "$(DELETING_COLOR)$(DELETING_STRING) $(NO_COLOR)$(OBJ_DIR) directory\n"
	@printf "%b" "$(DELETING_COLOR)$(DELETING_STRING) $(NO_COLOR)$(RESULT_DIR) directory\n"
	@printf "%b" "$(DELETING_COLOR)$(DELETING_STRING) $(NO_COLOR)$(TARGET)\n"
	@rm -rf $(TARGET) $(OBJ_DIR) $(RESULT_DIR)
