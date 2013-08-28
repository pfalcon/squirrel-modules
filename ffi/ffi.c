#define MODULE ffi
#include <sqmodule.h>
#include <sqmodule_helpers.h>
#include <stdio.h>
#include <dlfcn.h>
#include <ffi.h>

DECLARE_SQAPI

static HSQOBJECT lib_method_table;
static HSQOBJECT func_method_table;
static HSQOBJECT var_method_table;

static SQInteger m_dlopen(HSQUIRRELVM v)
{
    const SQChar *fname;
    sq_getstring(v, 2, &fname);
    void *mod = dlopen(fname, RTLD_NOW | RTLD_LOCAL);
    if (!mod)
        return sq_throwerror(v, "Cannot load library");
    sq_pushuserpointer(v, mod);
    void **p = (void*)sq_newuserdata(v, sizeof(mod));
    *p = mod;
    sq_pushobject(v, lib_method_table);
    sq_setdelegate(v, -2);
    return 1;
}

static SQInteger m_dlsym(HSQUIRRELVM v)
{
    void *mod;
    const SQChar *symname;
    sq_getuserpointer(v, 2, &mod);
    sq_getstring(v, 3, &symname);
    void *sym = dlsym(mod, symname);
    printf("dlsym(%s) = %p\n", symname, sym);
    if (!sym)
        return sq_throwerror(v, "Cannot find symbol");
    void **p = (void*)sq_newuserdata(v, sizeof(sym));
    *p = sym;
    return 1;
}

static ffi_type *char2ffi_type(char c)
{
    switch (c) {
    case 'b':
        return &ffi_type_schar;
    case 'B':
        return &ffi_type_uchar;
    case 'i':
        return &ffi_type_sint;
    case 'I':
        return &ffi_type_uint;
    case 'l':
        return &ffi_type_slong;
    case 'L':
        return &ffi_type_ulong;
    case 'P':
    case 's':
        return &ffi_type_pointer;
    default:
        return NULL;
    }
}

static ffi_type *get_ffi_type(HSQUIRRELVM v, int idx)
{
    int type = sq_gettype(v, idx);
    void *p;
    const SQChar *s;
    ffi_type *t;

    switch (type) {
    case OT_USERPOINTER:
        sq_getuserpointer(v, idx, &p);
        return p;
    case OT_STRING:
        sq_getstring(v, idx, &s);
        t = char2ffi_type(*s);
        if (t)
            return t;
    default:
        sq_throwerror(v, "Type spec must be string or ffi_type");
        return NULL;
    }
}

typedef struct FFIFunc
{
    void *func;
    char rettype;
    ffi_cif cif;
    ffi_type *params[0];
} FFIFunc;

typedef struct FFIVar
{
    void *var;
    char type;
} FFIVar;

static SQInteger m_lib_func(HSQUIRRELVM v)
{
    void **modbuf;
    void *mod;
    void *sym;
    const SQChar *symname;
    const char *rettype;

    sq_getuserdata(v, 1, (void**)&modbuf, NULL);
    mod = *modbuf;

    sq_getstring(v, 2, &rettype);

    sq_getstring(v, 3, &symname);
    sym = dlsym(mod, symname);
    if (!sym)
        return sq_throwerror(v, "Cannot find symbol");

    int nparam = sq_getsize(v, 4);

    int size = sizeof(FFIFunc) + sizeof(ffi_type*) * nparam;
    FFIFunc *ffibuf = (FFIFunc*)sq_newuserdata(v, size);
    sq_pushobject(v, func_method_table);
    sq_setdelegate(v, -2);

//    printf("Allocated %d bytes at %p\n", size, ffibuf);
    ffibuf->func = sym;
    ffibuf->rettype = *rettype;

    int i;
    for (i = 0; i < nparam; i++) {
        const char *paramtype;
        sq_pushinteger(v, i);
        sq_get(v, 4);
        ffibuf->params[i] = get_ffi_type(v, -1);
        if (!ffibuf->params[i])
            return SQ_ERROR;
        sq_poptop(v);
    }
    int res = ffi_prep_cif(&ffibuf->cif, FFI_DEFAULT_ABI, nparam, char2ffi_type(*rettype), ffibuf->params);
    if (res != FFI_OK)
        return sq_throwerror(v, "Error in ffi_prep_cif");
    return 1;
}

static SQInteger m_lib_var(HSQUIRRELVM v)
{
    void **modbuf;
    void *mod;
    void *sym;
    const SQChar *symname;
    const char *type;

    sq_getuserdata(v, 1, (void**)&modbuf, NULL);
    mod = *modbuf;

    sq_getstring(v, 2, &type);

    sq_getstring(v, 3, &symname);
    sym = dlsym(mod, symname);
    if (!sym)
        return sq_throwerror(v, "Cannot find symbol");

    FFIVar *ffibuf = (FFIVar*)sq_newuserdata(v, sizeof(FFIVar));
    sq_pushobject(v, var_method_table);
    sq_setdelegate(v, -2);

    ffibuf->var = sym;
    ffibuf->type = *type;

    return 1;
}

#define METAMETHOD
#ifdef METAMETHOD
// For metamethod, we have userdata, then delegate table
#define EXTRA_PARAMS 2
#else
// For normal method, there's only userdata
#define EXTRA_PARAMS 1
#endif

static void return_ffi_value(HSQUIRRELVM v, int val, char type);
static SQInteger m_func_call(HSQUIRRELVM v)
{
    FFIFunc *ffibuf;
    sq_getuserdata(v, 1, (void**)&ffibuf, NULL);
    int top = sq_gettop(v);
//    printf("ffibuf %p top %d\n", ffibuf, top);

    if (ffibuf->cif.nargs != top - EXTRA_PARAMS)
        return sq_throwerror(v, "Wrong number of args");

    int values[top - EXTRA_PARAMS];
    void *valueptrs[top - EXTRA_PARAMS];
    int i;
    for (i = EXTRA_PARAMS + 1; i <= top; i++) {
        #define pi (i - (EXTRA_PARAMS + 1))
        switch (sq_gettype(v, i)) {
        case OT_INTEGER:
            sq_getinteger(v, i, (int*)&values[pi]);
            break;
        case OT_STRING:
            sq_getstring(v, i, (const char**)&values[pi]);
            break;
        case OT_INSTANCE: {
            if (SQ_FAILED(sqstd_getblob(v, i, (SQUserPointer*)&values[pi])))
                return SQ_ERROR;
            break;
        }
        default:
            return sq_throwerror(v, "Unimplemented type");
        }
        valueptrs[pi] = &values[pi];
    }

    int rc;
//    printf("Before call, %p\n", ffibuf->func);
    ffi_call(&ffibuf->cif, ffibuf->func, &rc, valueptrs);
    return_ffi_value(v, rc, ffibuf->rettype);
    return 1;
}

static void return_ffi_value(HSQUIRRELVM v, int val, char type)
{
    switch (type) {
    case 's':
        sq_pushstring(v, (char*)val, -1);
        break;
    default:
        sq_pushinteger(v, val);
    }
}

static SQInteger m_var_get(HSQUIRRELVM v)
{
    FFIVar *ffibuf;
    sq_getuserdata(v, 1, (void**)&ffibuf, NULL);
    void *pval = ffibuf->var;
    switch (ffibuf->type) {
    case 's':
        sq_pushstring(v, *(char**)pval, -1);
        break;
    case 'i':
        sq_pushinteger(v, *(int*)pval);
        break;
    case 'I':
        sq_pushinteger(v, *(unsigned int*)pval);
        break;
    case 'h':
        sq_pushinteger(v, *(short*)pval);
        break;
    case 'H':
        sq_pushinteger(v, *(unsigned short*)pval);
        break;
    case 'l':
        sq_pushinteger(v, *(long*)pval);
        break;
    case 'L':
        sq_pushinteger(v, *(unsigned long*)pval);
        break;
    case 'b':
        sq_pushinteger(v, *(char*)pval);
        break;
    case 'B':
        sq_pushinteger(v, *(unsigned char*)pval);
        break;
    default:
        sq_pushinteger(v, *(int*)pval);
    }
    return 1;
}

static SQInteger m_set_dlg(HSQUIRRELVM v)
{
    if (SQ_FAILED(sq_setdelegate(v, 2)))
        return SQ_ERROR;
    return 0;
}

static SQRegFunction funcs[] = {
    {"load", m_dlopen, 2, _SC(".s")},
    {"sym", m_dlsym, 3, _SC(".ps")},
    {"setdelegate",  m_set_dlg, 3, _SC("..t")},
    {NULL}
};

static SQRegFunction lib_methods[] = {
    {"func", m_lib_func, 4, _SC("ussa")},
    {"var",  m_lib_var,  3, _SC("uss")},
    {NULL}
};

static SQRegFunction func_methods[] = {
    {"_call",  m_func_call, -1, _SC("u")},
    {NULL}
};

static SQRegFunction var_methods[] = {
    {"get",  m_var_get, 1, _SC("u")},
    {NULL}
};

struct FFI_type_name {
    const char* name;
    ffi_type *type;
};

static struct FFI_type_name ffi_types_wrap[] = {
    {"void",    &ffi_type_void},
    {"schar",   &ffi_type_schar},
    {"uchar",   &ffi_type_uchar},
    {"sshort",  &ffi_type_sshort},
    {"ushort",  &ffi_type_ushort},
    {"sint",    &ffi_type_sint},
    {"uint",    &ffi_type_uint},
    {"slong",   &ffi_type_slong},
    {"ulong",   &ffi_type_ulong},
    {"float",   &ffi_type_float},
    {"double",  &ffi_type_double},
    {"pointer", &ffi_type_pointer},
    {NULL}
};

SQRESULT MODULE_INIT(HSQUIRRELVM v, HSQAPI api)
{
    INIT_SQAPI(v, api);

    sq_register_funcs(v, funcs);

    int i;
    for (i = 0; ffi_types_wrap[i].name != 0; i++) {
        struct FFI_type_name *e = &ffi_types_wrap[i];
        sq_pushstring(v, e->name, -1);
        sq_pushuserpointer(v, e->type);
        sq_newslot(v, -3, SQFalse);
    }

    sq_create_delegate_table(v, lib_methods, &lib_method_table);
    sq_create_delegate_table(v, func_methods, &func_method_table);
    sq_create_delegate_table(v, var_methods, &var_method_table);

    return SQ_OK;
}
