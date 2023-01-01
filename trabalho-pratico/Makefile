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

# Paths
DATA_DIR := dataset1
TEST_DIR := tests/test_outputs1
INPUT := input1.txt

# Get sources, headers and object files
SRCS = $(call rwildcard,src,*.c)
HEADERS = $(call rwildcard,includes,*.h)
OBJS = $(SRCS:src/%.c=$(OBJ_DIR)/%.o) #$(SRCS:.c=.o)

# Other stuff to compile with
LIBS = -lm -lglib-2.0
INCLUDES = -Iincludes
PKG_CONFIG = `pkg-config --cflags --libs glib-2.0`
MAIN_TARGET = programa-principal
TESTS_TARGET = unidade-testes

# Pretty stuff (using %b in printf to interpret escape sequences as special characters) (alternatively echo -e "string" can be used)
NO_COLOR = \033[m
LINKING_COLOR = \033[0;33m
COMPILING_COLOR = \033[0;33m
DELETING_COLOR = \033[0;31m
OK_COLOR = \033[0;32m
COMPILING_STRING = "[COMPILING]"
LINKING_STRING = "[LINKING]"
DELETING_STRING = "[DELETING]"
OK_STRING = "[OK]"

# Link files
$(OBJ_DIR)/%.o: src/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@printf "%b" "$(LINKING_COLOR)$(LINKING_STRING) $(NO_COLOR)$@\n"
	@$(CC) $(FLAGS) -c -o $@ $(INCLUDES) $(PKG_CONFIG) $< $(LIBS)

# Create executable
$(MAIN_TARGET): $(OBJS)
	@printf "%b" "$(COMPILING_COLOR)$(COMPILING_STRING) $(NO_COLOR)$@\n"
	@$(CC) $(FLAGS) -o $@ $(INCLUDES) $(PKG_CONFIG) $^ $(LIBS)
	@mkdir -p $(RESULT_DIR)
	@printf "%b" "$(OK_COLOR)$(OK_STRING) $(NO_COLOR)\n"

# Run main program in batch mode (make run) or interactive mode (make run i)
run: $(MAIN_TARGET)
	@if [ $(words $(MAKECMDGOALS)) -eq 1 ]; then \
		printf "%b" "$(COMPILING_COLOR)Running batch mode ...$(NO_COLOR)\n"; \
		./$< $(DATA_DIR) $(INPUT); \
		printf "%b" "$(COMPILING_COLOR)Done!$(NO_COLOR)\n"; \
	elif [ $(words $(MAKECMDGOALS)) -eq 2 ] && [ $(word 2, $(MAKECMDGOALS)) = i ]; then \
		printf "%b" "$(COMPILING_COLOR)Running interactive mode!$(NO_COLOR)\n"; \
		./$<; \
		printf "%b" "$(COMPILING_COLOR)Done!$(NO_COLOR)\n"; \
	else \
		printf "%b" "$(COMPILING_COLOR)Invalid target!$(NO_COLOR)\n"; \
	fi

# Supress "make: *** No rule to make target 'i'. Stop."
# Supress "make: Nothing to be done for 'i'." with shell no-op (:)
# I believe there are better ways to do this but I really want to use "make run" and "make run i" 
# Solutions: 
#	- Create another rule "make interactive" instead of "make run i"
#	- Create a variable to check interactive mode -> INTERACTIVE = 0 -> "make run INTERACTIVE=1"
#	- ...
# Problems with this solution: "make i" does not throw the error "make: *** No rule to make target 'i'. Stop."
# Any other rule throws the error (could standardize this by suppressing all of them using "%:" instead of "i:")
.PHONY: i
i:
	@:

# Create the unit testing executable and run it (if RESULT_DIR exists)
test: tests/test-unit.c
	@printf "%b" "$(COMPILING_COLOR)$(COMPILING_STRING) $(NO_COLOR)$(TESTS_TARGET)\n"
	@$(CC) $(FLAGS) $< -o $(TESTS_TARGET) -lm
	@printf "%b" "$(OK_COLOR)$(OK_STRING) $(NO_COLOR)\n"
	@if [ -d $(RESULT_DIR) ]; then \
		./$(TESTS_TARGET) $(TEST_DIR) $(RESULT_DIR); \
	else \
		printf "%b" "$(COMPILING_COLOR)$(RESULT_DIR) directory does not exist!$(NO_COLOR)\n"; \
	fi

# Remove the object directory (and its contents), main-program and test-unit (if it exists)
.PHONY: clean
clean:
	@printf "%b" "$(DELETING_COLOR)$(DELETING_STRING) $(NO_COLOR)$(OBJ_DIR) directory\n"
	@printf "%b" "$(DELETING_COLOR)$(DELETING_STRING) $(NO_COLOR)$(RESULT_DIR) directory\n"
	@printf "%b" "$(DELETING_COLOR)$(DELETING_STRING) $(NO_COLOR)$(MAIN_TARGET)\n"
	@if [ -f "$(TESTS_TARGET)" ]; then \
		printf "%b" "$(DELETING_COLOR)$(DELETING_STRING) $(NO_COLOR)$(TESTS_TARGET)\n"; \
		rm -rf $(TESTS_TARGET); \
	fi
	@rm -rf $(MAIN_TARGET) $(OBJ_DIR) $(RESULT_DIR)
