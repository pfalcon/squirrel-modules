#define MODULE osstream
#include <sqmodule.h>
#include <sqmodule_helpers.h>
//#include <sqstdio.h>
//#include <stdio.h>
//#include <string.h>
#include <new>

DECLARE_SQAPI

SQRESULT init_popen(HSQUIRRELVM v);
SQRESULT init_fdopen(HSQUIRRELVM v);

SQRESULT MODULE_INIT(HSQUIRRELVM v, HSQAPI api)
{
    INIT_SQAPI(v, api);
    init_popen(v);
    init_fdopen(v);
}
