#define MODULE sample
#include <sqmodule.h>
#include <stdio.h>

DECLARE_SQAPI

static SQInteger func(HSQUIRRELVM v)
{
    static char s[] = "Hello, modules!";
    SQAPI(pushstring)(v, s, sizeof(s) - 1);
    return 1;
}

// Module init function

SQRESULT MODULE_INIT(HSQUIRRELVM v, HSQAPI api)
{
    printf("in sqmodule_load\n");

    INIT_SQAPI(api);

    SQAPI(pushstring)(v, _SC("func"), -1);
    SQAPI(newclosure)(v, func, 0);
    SQAPI(newslot)(v, -3, SQFalse);

    printf("out sqmodule_load\n");

    return SQ_OK;
}
