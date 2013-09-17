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

static inline SQRESULT declare_stream(HSQUIRRELVM v, const SQChar *cls, SQRegFunction *methods)
{
    SQInteger top = sq_gettop(v);

    sq_pushregistrytable(v);
    // Push class name much beforehand, because it should go before value
    // for sq_newslot()
    sq_pushstring(v, cls, -1);

    // Lookup stream base class in registry table
    sq_pushstring(v, _SC("std_stream"), -1);
    if (SQ_SUCCEEDED(sq_get(v, -3))) {
        // Inherit from stream class
        sq_newclass(v, SQTrue);
        // As a type tag, we use address of (constant, unique) class string
        sq_settypetag(v, -1, (SQUserPointer)cls);

        sq_register_funcs(v, methods);

        // Associate created class with its name in namespace table
        sq_newslot(v, -4, SQFalse);
        sq_settop(v, top);
        return SQ_OK;
    }
    sq_settop(v,top);
    return SQ_ERROR;
}

#endif // _SQMODULE_HELPERS_H
