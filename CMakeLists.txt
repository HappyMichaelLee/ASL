cmake_minimum_required(VERSION 3.17)
project(asl C)

set(CMAKE_C_STANDARD 11)

include_directories(src/core)
include_directories(src/mods)
include_directories(src/libs)

add_executable(asl
        src/core/asl.c
        src/core/asl.h
        src/core/asl_compiler.c
        src/core/asl_compiler.h
        src/core/asl_core.h
        src/core/asl_debugger.c
        src/core/asl_debugger.h
        src/core/asl_exception.c
        src/core/asl_exception.h
        src/core/asl_executor.c
        src/core/asl_executor.h
        src/core/asl_hash.c
        src/core/asl_hash.h
        src/core/asl_language_parser.c
        src/core/asl_language_parser.h
        src/core/asl_language_scanner.c
        src/core/asl_memory.c
        src/core/asl_memory.h
        src/core/asl_module.c
        src/core/asl_module.h
        src/core/asl_output.c
        src/core/asl_output.h
        src/core/asl_util.c
        src/core/asl_util.h
        src/mods/mod_curl.c
        src/mods/mod_curl.h
        src/mods/mod_standard.c
        src/mods/mod_standard.h
        )
