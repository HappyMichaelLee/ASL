#ifndef ASL_ASL_CORE_H
#define ASL_ASL_CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <limits.h>
#include <getopt.h>
#include <ctype.h>

#include <asl_hash.h>
#include <asl_env.h>
#include <asl_memory.h>
#include <asl_util.h>
#include <asl_command.h>
#include <asl_config.h>
#include <asl_value.h>
#include <asl_compiler.h>
#include <asl_exception.h>
#include <asl_executor.h>
#include <asl_output.h>
#include <asl_debugger.h>
#include <asl_module.h>

#define ASL_VERSION                    "1.0.0"
#define ASL_VERSION_NUM                100000
#define RET_SUCCESS                    (int) 0
#define RET_FAILURE                    (int) -1

#ifndef NULL
#define NULL						((void *)0)
#endif

#define is_empty(p)                    (p==NULL)
#define is_not_empty(p)                (p!=NULL)

#define ERROR_FD                    stderr

#define OUTPUT_FD                   stdout
#define EOL                         "\n"
#define STR(x)                      #x
#define CHAR(x)                     #x

#define TRUE_VAL                    (char) '1'
#define FALSE_VAL                   (char) '0'
#define LEN(array)                  sizeof(array)/sizeof(array[0])

#endif
