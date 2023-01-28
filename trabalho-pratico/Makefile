# Recursive wildcard 
# The first parameter ($1) is a list of directories, and the second ($2) is a list of patterns to match
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

# Default to release build
# Override with `make BUILD=debug` or `make BUILD=sanitizer`
BUILD := release

# Compiler
CC = gcc

# Flags
FLAGS.release = -Wall -Wextra -Wdouble-promotion -Werror=pedantic -Werror=vla -pedantic-errors -Wfatal-errors -flto -O2 -march=native -mtune=native -lncurses
FLAGS.sanitizer = -Wall -Wextra -Wdouble-promotion -Werror=pedantic -Werror=vla -pedantic-errors -Wfatal-errors -g -fsanitize=address -lncurses
FLAGS.debug = -Wall -Wextra -Wdouble-promotion -Werror=pedantic -Werror=vla -pedantic-errors -Wfatal-errors -O0 -g -lncurses
FLAGS := $(FLAGS.$(BUILD))

# Makefile Flags
MAKEFLAGS += --no-print-directory

# Directories
OBJ_DIR := build
RESULT_DIR := Resultados

# Paths
DATA_DIR :=
TEST_DIR :=
INPUT :=
MODE := NULL

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
RESETTING_STRING = "[RESETTING]"
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

# Selects the dataset that will be used when the program is run by changing the path variables
# It has 4 possible sets (sv, si, lv, li)
# sv - small dataset with only valid entries | si - small dataset with some invalid entries
# lv - large dataset with only valid entries | li - large dataset with some invalid entries
.PHONY: set
set:
	@if [ $(words $(MAKECMDGOALS)) -eq 2 ] && [ $(word 2, $(MAKECMDGOALS)) = sv ]; then \
		printf "%b" "$(COMPILING_COLOR)Changing mode from $(MODE) to SV ...$(NO_COLOR)\n"; \
		sed -i '0,/DATA_DIR :=.*/{s/DATA_DIR :=.*/DATA_DIR := datasets\/small-valid-dataset/}' Makefile; \
		sed -i '0,/TEST_DIR :=.*/{s/TEST_DIR :=.*/TEST_DIR := tests\/sv-outputs/}' Makefile; \
		sed -i '0,/INPUT :=.*/{s/INPUT :=.*/INPUT := inputs\/sv-input.txt/}' Makefile; \
		sed -i '0,/MODE :=.*/{s/MODE :=.*/MODE := SV/}' Makefile; \
		printf "%b" "$(COMPILING_COLOR)Done!$(NO_COLOR)\n"; \
	elif [ $(words $(MAKECMDGOALS)) -eq 2 ] && [ $(word 2, $(MAKECMDGOALS)) = si ]; then \
		printf "%b" "$(COMPILING_COLOR)Changing mode from $(MODE) to SI ...$(NO_COLOR)\n"; \
		sed -i '0,/DATA_DIR :=.*/{s/DATA_DIR :=.*/DATA_DIR := datasets\/small-invalid-dataset/}' Makefile; \
		sed -i '0,/TEST_DIR :=.*/{s/TEST_DIR :=.*/TEST_DIR := tests\/si-outputs/}' Makefile; \
		sed -i '0,/INPUT :=.*/{s/INPUT :=.*/INPUT := inputs\/si-input.txt/}' Makefile; \
		sed -i '0,/MODE :=.*/{s/MODE :=.*/MODE := SI/}' Makefile; \
		printf "%b" "$(COMPILING_COLOR)Done!$(NO_COLOR)\n"; \
	elif [ $(words $(MAKECMDGOALS)) -eq 2 ] && [ $(word 2, $(MAKECMDGOALS)) = lv ]; then \
		printf "%b" "$(COMPILING_COLOR)Changing mode from $(MODE) to LV ...$(NO_COLOR)\n"; \
		sed -i '0,/DATA_DIR :=.*/{s/DATA_DIR :=.*/DATA_DIR := datasets\/large-valid-dataset/}' Makefile; \
		sed -i '0,/TEST_DIR :=.*/{s/TEST_DIR :=.*/TEST_DIR := tests\/lv-outputs/}' Makefile; \
		sed -i '0,/INPUT :=.*/{s/INPUT :=.*/INPUT := inputs\/lv-li-input.txt/}' Makefile; \
		sed -i '0,/MODE :=.*/{s/MODE :=.*/MODE := LV/}' Makefile; \
		printf "%b" "$(COMPILING_COLOR)Done!$(NO_COLOR)\n"; \
	elif [ $(words $(MAKECMDGOALS)) -eq 2 ] && [ $(word 2, $(MAKECMDGOALS)) = li ]; then \
		printf "%b" "$(COMPILING_COLOR)Changing mode from $(MODE) to LI ...$(NO_COLOR)\n"; \
		sed -i '0,/DATA_DIR :=.*/{s/DATA_DIR :=.*/DATA_DIR := datasets\/large-invalid-dataset/}' Makefile; \
		sed -i '0,/TEST_DIR :=.*/{s/TEST_DIR :=.*/TEST_DIR := tests\/li-outputs/}' Makefile; \
		sed -i '0,/INPUT :=.*/{s/INPUT :=.*/INPUT := inputs\/lv-li-input.txt/}' Makefile; \
		sed -i '0,/MODE :=.*/{s/MODE :=.*/MODE := LI/}' Makefile; \
		printf "%b" "$(COMPILING_COLOR)Done!$(NO_COLOR)\n"; \
	else \
		printf "%b" "$(COMPILING_COLOR)Invalid dataset!$(NO_COLOR)\n"; \
	fi

# This is explained below after the run rule. It's the same circumstance
.PHONY: sv si lv li
sv:
	@:

si:
	@:

lv:
	@:

li:
	@:

# Run main program in batch mode (make run) or interactive mode (make run i)
run: $(MAIN_TARGET)
	@if [ $(words $(MAKECMDGOALS)) -eq 1 ]; then \
		if [ -n "$(strip $(DATA_DIR))" ]; then \
			printf "%b" "$(COMPILING_COLOR)Running batch mode with $(DATA_DIR) ...$(NO_COLOR)\n"; \
			./$< $(DATA_DIR) $(INPUT); \
			printf "%b" "$(COMPILING_COLOR)Done!$(NO_COLOR)\n"; \
		else \
			printf "%b" "$(COMPILING_COLOR)Dataset is not defined!$(NO_COLOR)\n"; \
		fi \
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

# Run gdb (clean up any existing build and build debug mode)
.PHONY: gdb
gdb:
	@if [ -d $(RESULT_DIR) ]; then \
		printf "%b" "$(COMPILING_COLOR)Deleting old build!$(NO_COLOR)\n\n"; \
		make clean; \
		printf "%b" "\n"; \
	fi
	@printf "%b" "$(COMPILING_COLOR)Building debug mode!$(NO_COLOR)\n\n"
	@make BUILD=debug
	@gdb --args $(MAIN_TARGET) $(DATA_DIR) $(INPUT)

# Run valgrind (clean up any existing build and build debug mode)
.PHONY: valgrind
valgrind:
	@if [ -d $(RESULT_DIR) ]; then \
		printf "%b" "$(COMPILING_COLOR)Deleting old build!$(NO_COLOR)\n\n"; \
		make clean; \
		printf "%b" "\n"; \
	fi
	@printf "%b" "$(COMPILING_COLOR)Building debug mode!$(NO_COLOR)\n\n"
	@make BUILD=debug
	@valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -s ./$(MAIN_TARGET) $(DATA_DIR) $(INPUT)

# Remove the object directory (and its contents), main-program and test-unit (if it exists) and clean path variables
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
	@printf "%b" "$(DELETING_COLOR)$(RESETTING_STRING) $(NO_COLOR)paths (MODE = NULL)\n"
	@sed -i '0,/DATA_DIR :=.*/{s/DATA_DIR :=.*/DATA_DIR :=/}' Makefile
	@sed -i '0,/TEST_DIR :=.*/{s/TEST_DIR :=.*/TEST_DIR :=/}' Makefile
	@sed -i '0,/INPUT :=.*/{s/INPUT :=.*/INPUT :=/}' Makefile
	@sed -i '0,/MODE :=.*/{s/MODE :=.*/MODE := NULL/}' Makefile; \
