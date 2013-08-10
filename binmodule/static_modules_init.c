#include <sqimport.h>

SQRESULT init_static_modules(HSQUIRRELVM v)
{
    return sqrat_register_importlib(v);
}
