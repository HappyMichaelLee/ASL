#include <asl_core.h>
#include <asl_module.h>

extern module_info_t MOD_MOD_NAME(standard);

module_info_t *global_mods[] =
        {
                &MOD_MOD_NAME(standard),
                NULL
        };