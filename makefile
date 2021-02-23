TARGET = asl
CC = gcc
PWD_PATH = $(shell pwd)
BIN_PATH = ${PWD_PATH}/bin/
OBJS_PATH = ${PWD_PATH}/objs/
SRC_CORE_PATH = ${PWD_PATH}/src/core/
SRC_MOD_PATH = ${PWD_PATH}/src/mods/
SRC_LIB_PATH = ${PWD_PATH}/src/libs/
SCRIPT_PATH = ${PWD_PATH}/script/
SCRIPT_FILES = $(foreach dir,$(SCRIPT_PATH),$(wildcard $(dir)*.asl))

INCLUDES = -I$(SRC_CORE_PATH) -I$(SRC_MOD_PATH) -I$(SRC_LIB_PATH)

OBJS = \
	$(OBJS_PATH)asl.o \
	$(OBJS_PATH)asl_language_scanner.o \
	$(OBJS_PATH)asl_language_parser.o \
	$(OBJS_PATH)asl_compiler.o \
	$(OBJS_PATH)asl_hash.o \
	$(OBJS_PATH)asl_config.o \
	$(OBJS_PATH)asl_library.o \
	$(OBJS_PATH)asl_env.o \
	$(OBJS_PATH)asl_command.o \
	$(OBJS_PATH)asl_exception.o \
	$(OBJS_PATH)asl_memory.o \
	$(OBJS_PATH)asl_util.o \
	$(OBJS_PATH)asl_executor.o \
	$(OBJS_PATH)asl_executor_exp.o \
	$(OBJS_PATH)asl_executor_bin.o \
	$(OBJS_PATH)asl_executor_func.o \
	$(OBJS_PATH)asl_executor_control.o \
	$(OBJS_PATH)asl_executor_loop.o \
	$(OBJS_PATH)asl_output.o \
	$(OBJS_PATH)asl_debugger.o \
	$(OBJS_PATH)asl_value.o \
	$(OBJS_PATH)asl_module.o \
	$(OBJS_PATH)mod_register.o \
	$(OBJS_PATH)mod_curl.o \
	$(OBJS_PATH)mod_standard.o


OBJ_FLAGS =  -c -g -pedantic -DDEBUG $(INCLUDES)
$(BIN_PATH)$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $@ -lm

clean:
	rm -rf $(OBJS_PATH)*.o
	rm -f $(BIN_PATH)$(TARGET)
	rm -Rf /tmp/core.*
	clear

build-lang:
	cd $(SRC_CORE_PATH);bison --yacc -dv \
	--defines=asl_language_parser.h \
	--output=asl_language_parser.c \
	asl_language_parser.y
	cd $(SRC_CORE_PATH);flex --outfile=asl_language_scanner.c \
	asl_language_scanner.l

build:
	make clean
	make build-lang
	make

test:
	for file in ${SCRIPT_FILES};do \
	    echo "Execute $(BIN_PATH)$(TARGET) -f $$file"; \
		$(BIN_PATH)$(TARGET) -f $$file; \
	done


$(OBJS_PATH)asl_language_parser.o: $(SRC_CORE_PATH)asl_language_parser.c
	$(CC) $< -o $(OBJS_PATH)asl_language_parser.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_language_scanner.o: $(SRC_CORE_PATH)asl_language_scanner.c
	$(CC) $< -o $(OBJS_PATH)asl_language_scanner.o $(OBJ_FLAGS)

$(OBJS_PATH)asl.o: $(SRC_CORE_PATH)asl.c
	$(CC) $< -o $(OBJS_PATH)asl.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_command.o: $(SRC_CORE_PATH)asl_command.c
	$(CC) $< -o $(OBJS_PATH)asl_command.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_compiler.o: $(SRC_CORE_PATH)asl_compiler.c
	$(CC) $< -o $(OBJS_PATH)asl_compiler.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_env.o: $(SRC_CORE_PATH)asl_env.c
	$(CC) $< -o $(OBJS_PATH)asl_env.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_config.o: $(SRC_CORE_PATH)asl_config.c
	$(CC) $< -o $(OBJS_PATH)asl_config.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_library.o: $(SRC_CORE_PATH)asl_library.c
	$(CC) $< -o $(OBJS_PATH)asl_library.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_exception.o: $(SRC_CORE_PATH)asl_exception.c
	$(CC) $< -o $(OBJS_PATH)asl_exception.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_util.o: $(SRC_CORE_PATH)asl_util.c
	$(CC) $< -o $(OBJS_PATH)asl_util.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_memory.o: $(SRC_CORE_PATH)asl_memory.c
	$(CC) $< -o $(OBJS_PATH)asl_memory.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_hash.o: $(SRC_CORE_PATH)asl_hash.c
	$(CC) $< -o $(OBJS_PATH)asl_hash.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_executor.o: $(SRC_CORE_PATH)asl_executor.c
	$(CC) $< -o $(OBJS_PATH)asl_executor.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_executor_exp.o: $(SRC_CORE_PATH)asl_executor_exp.c
	$(CC) $< -o $(OBJS_PATH)asl_executor_exp.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_executor_bin.o: $(SRC_CORE_PATH)asl_executor_bin.c
	$(CC) $< -o $(OBJS_PATH)asl_executor_bin.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_executor_func.o: $(SRC_CORE_PATH)asl_executor_func.c
	$(CC) $< -o $(OBJS_PATH)asl_executor_func.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_executor_control.o: $(SRC_CORE_PATH)asl_executor_control.c
	$(CC) $< -o $(OBJS_PATH)asl_executor_control.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_executor_loop.o: $(SRC_CORE_PATH)asl_executor_loop.c
	$(CC) $< -o $(OBJS_PATH)asl_executor_loop.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_output.o: $(SRC_CORE_PATH)asl_output.c
	$(CC) $< -o $(OBJS_PATH)asl_output.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_debugger.o: $(SRC_CORE_PATH)asl_debugger.c
	$(CC) $< -o $(OBJS_PATH)asl_debugger.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_value.o: $(SRC_CORE_PATH)asl_value.c
	$(CC) $< -o $(OBJS_PATH)asl_value.o $(OBJ_FLAGS)

$(OBJS_PATH)asl_module.o: $(SRC_CORE_PATH)asl_module.c
	$(CC) $< -o $(OBJS_PATH)asl_module.o $(OBJ_FLAGS)

$(OBJS_PATH)mod_register.o: $(SRC_MOD_PATH)mod_register.c
	$(CC) $< -o $(OBJS_PATH)mod_register.o $(OBJ_FLAGS)

$(OBJS_PATH)mod_curl.o: $(SRC_MOD_PATH)mod_curl.c
	$(CC) $< -o $(OBJS_PATH)mod_curl.o $(OBJ_FLAGS)

$(OBJS_PATH)mod_standard.o: $(SRC_MOD_PATH)mod_standard.c
	$(CC) $< -o $(OBJS_PATH)mod_standard.o $(OBJ_FLAGS)