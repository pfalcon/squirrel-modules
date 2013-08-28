#ifndef _SQMODULE_HELPERS_H

#include <sqmodule.h>

// TODO: This probably should be normal, not header-only, library

static inline void sq_register_funcs(HSQUIRRELVM sqvm, SQRegFunction *obj_funcs)
{
   SQInteger i = 0;
   while (obj_funcs[i].name != 0) {
       SQRegFunction *f = &obj_funcs[i];
       sq_pushstring(sqvm, f->name, -1);
       sq_newclosure(sqvm, f->f, 0);
       sq_setparamscheck(sqvm, f->nparamscheck, f->typemask);
       sq_setnativeclosurename(sqvm, -1, f->name);
       sq_newslot(sqvm, -3, SQFalse);
       i++;
   }
}

static inline void sq_create_delegate_table(HSQUIRRELVM vm, SQRegFunction *methods, HSQOBJECT *handle)
{
    sq_newtable(vm);
    sq_register_funcs(vm, methods);
    sq_resetobject(handle);
    sq_getstackobj(vm, -1, handle);
    sq_addref(vm, handle);
}


#endif // _SQMODULE_HELPERS_H
