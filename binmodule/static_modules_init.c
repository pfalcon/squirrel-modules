#include <stdio.h>
#include <sqimport.h>

SQRESULT init_static_modules(HSQUIRRELVM v)
{
    if (SQ_FAILED(sqrat_register_importlib(v)))
        return SQ_ERROR;
    sq_pushstring(v, _SC("stdlibinit"), -1);
    sq_pushroottable(v);
    if (SQ_FAILED(sq_import(v))) {
        printf("WARNING: Error occurred while importing stdlibinit module:\n");
        sq_pushroottable(v);
        sq_getlasterror(v);
        sqstd_aux_printerror(v);
    }
    return SQ_OK;
}
