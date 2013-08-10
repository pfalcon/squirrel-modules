//
// SqImport: Supports importing of squirrel modules
//

//
// Copyright (c) 2009 Brandon Jones
// Copyright (c) 2013 Paul Sokolovsky
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source
//  distribution.
//

#include "sqimport.h"
#include "sqmodule.h"

//#include "sqratlib/sqratBase.h"
#include <sqstdio.h>
#include <sqstdblob.h>
#include <string>
// getenv
#include <stdlib.h>

#if defined(_WIN32)

#include <windows.h>

#elif defined(__unix)

#include <dlfcn.h>

#endif

typedef SQRESULT (*SQMODULELOAD)(HSQUIRRELVM v, HSQAPI sq);

static HSQAPI sqapi = NULL;

// Create and populate the HSQAPI structure with function pointers
// If new functions are added to the Squirrel API, they should be added here too
static HSQAPI sqrat_newapi() {
    HSQAPI sq = (HSQAPI)sq_malloc(sizeof(sq_api));

    sq->version = SQMODULE_API_VERSION;
    /*vm*/
    sq->open = sq_open;
    sq->newthread = sq_newthread;
    sq->seterrorhandler = sq_seterrorhandler;
    sq->close = sq_close;
    sq->setforeignptr = sq_setforeignptr;
    sq->getforeignptr = sq_getforeignptr;
    sq->setprintfunc = sq_setprintfunc;
    sq->getprintfunc = sq_getprintfunc;
    sq->suspendvm = sq_suspendvm;
    sq->wakeupvm = sq_wakeupvm;
    sq->getvmstate = sq_getvmstate;

    /*compiler*/
    sq->compile = sq_compile;
    sq->compilebuffer = sq_compilebuffer;
    sq->enabledebuginfo = sq_enabledebuginfo;
    sq->notifyallexceptions = sq_notifyallexceptions;
    sq->setcompilererrorhandler = sq_setcompilererrorhandler;

    /*stack operations*/
    sq->push = sq_push;
    sq->pop = sq_pop;
    sq->poptop = sq_poptop;
    sq->remove = sq_remove;
    sq->gettop = sq_gettop;
    sq->settop = sq_settop;
    sq->reservestack = sq_reservestack;
    sq->cmp = sq_cmp;
    sq->move = sq_move;

    /*object creation handling*/
    sq->newuserdata = sq_newuserdata;
    sq->newtable = sq_newtable;
    sq->newarray = sq_newarray;
    sq->newclosure = sq_newclosure;
    sq->setparamscheck = sq_setparamscheck;
    sq->bindenv = sq_bindenv;
    sq->pushstring = sq_pushstring;
    sq->pushfloat = sq_pushfloat;
    sq->pushinteger = sq_pushinteger;
    sq->pushbool = sq_pushbool;
    sq->pushuserpointer = sq_pushuserpointer;
    sq->pushnull = sq_pushnull;
    sq->gettype = sq_gettype;
    sq->getsize = sq_getsize;
    sq->getbase = sq_getbase;
    sq->instanceof = sq_instanceof;
    sq->tostring = sq_tostring;
    sq->tobool = sq_tobool;
    sq->getstring = sq_getstring;
    sq->getinteger = sq_getinteger;
    sq->getthread = sq_getthread;
    sq->getbool = sq_getbool;
    sq->getuserpointer = sq_getuserpointer;
    sq->getuserdata = sq_getuserdata;
    sq->settypetag = sq_settypetag;
    sq->gettypetag = sq_gettypetag;
    sq->setreleasehook = sq_setreleasehook;
    sq->getscratchpad = sq_getscratchpad;
    sq->getclosureinfo = sq_getclosureinfo;
    sq->setnativeclosurename = sq_setnativeclosurename;
    sq->setinstanceup = sq_setinstanceup;
    sq->getinstanceup = sq_getinstanceup;
    sq->setclassudsize = sq_setclassudsize;
    sq->newclass = sq_newclass;
    sq->createinstance = sq_createinstance;
    sq->setattributes = sq_setattributes;
    sq->getattributes = sq_getattributes;
    sq->getclass = sq_getclass;
    sq->weakref = sq_weakref;
    sq->getdefaultdelegate = sq_getdefaultdelegate;

    /*object manipulation*/
    sq->pushroottable = sq_pushroottable;
    sq->pushregistrytable = sq_pushregistrytable;
    sq->pushconsttable = sq_pushconsttable;
    sq->setroottable = sq_setroottable;
    sq->setconsttable = sq_setconsttable;
    sq->newslot = sq_newslot;
    sq->deleteslot = sq_deleteslot;
    sq->set = sq_set;
    sq->get = sq_get;
    sq->rawset = sq_rawset;
    sq->rawget = sq_rawget;
    sq->rawdeleteslot = sq_rawdeleteslot;
    sq->arrayappend = sq_arrayappend;
    sq->arraypop = sq_arraypop;
    sq->arrayresize = sq_arrayresize;
    sq->arrayreverse = sq_arrayreverse;
    sq->arrayremove = sq_arrayremove;
    sq->arrayinsert = sq_arrayinsert;
    sq->setdelegate = sq_setdelegate;
    sq->getdelegate = sq_getdelegate;
    sq->clone = sq_clone;
    sq->setfreevariable = sq_setfreevariable;
    sq->next = sq_next;
    sq->getweakrefval = sq_getweakrefval;
    sq->clear = sq_clear;

    /*calls*/
    sq->call = sq_call;
    sq->resume = sq_resume;
    sq->getlocal = sq_getlocal;
    sq->getfreevariable = sq_getfreevariable;
    sq->throwerror = sq_throwerror;
    sq->reseterror = sq_reseterror;
    sq->getlasterror = sq_getlasterror;

    /*raw object handling*/
    sq->getstackobj = sq_getstackobj;
    sq->pushobject = sq_pushobject;
    sq->addref = sq_addref;
    sq->release = sq_release;
    sq->resetobject = sq_resetobject;
    sq->objtostring = sq_objtostring;
    sq->objtobool = sq_objtobool;
    sq->objtointeger = sq_objtointeger;
    sq->objtofloat = sq_objtofloat;
    sq->getobjtypetag = sq_getobjtypetag;

    /*GC*/
    sq->collectgarbage = sq_collectgarbage;

    /*serialization*/
    sq->writeclosure = sq_writeclosure;
    sq->readclosure = sq_readclosure;

    /*mem allocation*/
    sq->malloc = sq_malloc;
    sq->realloc = sq_realloc;
    sq->free = sq_free;

    /*debug*/
    sq->stackinfos = sq_stackinfos;
    sq->setdebughook = sq_setdebughook;

    /*stdlib*/
    sq->getblob = sqstd_getblob;

    return sq;
}


static SQRESULT sqrat_importscript(HSQUIRRELVM v, const SQChar* filename) {
    if(SQ_FAILED(sqstd_loadfile(v, filename, true))) {
        return SQ_ERROR;
    }
    sq_push(v, -2);
    sq_call(v, 1, false, true);
    return SQ_OK;
}

static SQRESULT sqrat_importbin(HSQUIRRELVM v, const SQChar* moduleName) {
#ifdef SQUNICODE
#warning sqrat_importbin() Not Implemented
    return SQ_ERROR;
#else
    SQMODULELOAD modLoad = 0;

#if defined(_WIN32)
    HMODULE mod;
    mod = LoadLibrary(moduleName);
    if (mod == NULL)
        return sq_throwerror(v, _SC("Could not load module"));

    modLoad = (SQMODULELOAD)GetProcAddress(mod, "sqmodule_load");
    if(modLoad == NULL) {
        FreeLibrary(mod);
        return sq_throwerror(v, _SC("Could not get module init routine"));
    }
#elif defined(__unix)
    void *mod = dlopen(moduleName, RTLD_NOW | RTLD_LOCAL);
    if (mod == NULL)
        return sq_throwerror(v, _SC("Could not load module"));

    modLoad = (SQMODULELOAD)dlsym(mod, "sqmodule_load");
    if (modLoad == NULL) {
        dlclose(mod);
        return sq_throwerror(v, _SC("Could not get module init routine"));
    }
#endif

    if(sqapi == NULL) {
        sqapi = sqrat_newapi(); // Caching this for multiple imports is probably a very good idea
    }

    SQRESULT res = modLoad(v, sqapi);

    return res;
#endif
}

bool file_exists(const SQChar* filename)
{
    return access(filename, F_OK) == 0;
}

#define NOT_FOUND 100

SQRESULT try_import_path(HSQUIRRELVM v, std::basic_string<SQChar>& fname)
{
    // Try to import module at given path. This tries
    // different module types in order.

    // Check binary module
    fname[fname.size() - 1] = 'd';
    if (file_exists(fname.c_str()))
        return sqrat_importbin(v, fname.c_str());
    // Check bytecode module
    fname[fname.size() - 1] = 'c';
    if (file_exists(fname.c_str()))
        return sqrat_importscript(v, fname.c_str());
    // Check source module
    fname[fname.size() - 1] = 't';
    if (file_exists(fname.c_str()))
        return sqrat_importscript(v, fname.c_str());

    return NOT_FOUND;
}

SQInteger sq_import(HSQUIRRELVM v) {
    const SQChar* moduleName;
    HSQOBJECT table;
    SQRESULT res = SQ_OK;


    sq_getstring(v, -2, &moduleName);
    sq_getstackobj(v, -1, &table);
    sq_addref(v, &table);

    sq_settop(v, 0); // Clear Stack
    sq_pushobject(v, table); // Push the target table onto the stack

    std::basic_string<SQChar> fname;
    // Explicit path is needed for dlopen() later, and won't hurt
    // for other cases
    fname = "./";
    fname += moduleName;
    fname += ".nut";
    res = try_import_path(v, fname);
    if (res == NOT_FOUND) {
        const char *path = getenv("SQPATH");
        if (path)
            fname = path;
        else {
            path = getenv("HOME");
            if (path) {
                fname = path;
                fname += "/.squirrel/lib";
            } else {
                fname = "/usr/lib/squirrel";
            }
        }
        fname += "/";
        fname += moduleName;
        fname += ".nut";
        res = try_import_path(v, fname);
        if (res == NOT_FOUND) {
            sq_settop(v, 0); // Clean up the stack (just in case the module load leaves it messy)
            sq_release(v, &table);
            return sq_throwerror(v, _SC("Module not found"));
        }
    }

    sq_settop(v, 0); // Clean up the stack (just in case the module load leaves it messy)
    sq_pushobject(v, table); // return the target table
    sq_release(v, &table);

    // If we didn't thro exception, then we return value
    if (res != SQ_ERROR)
        return 1;
}

static SQInteger sqratbase_import(HSQUIRRELVM v) {
    SQInteger args = sq_gettop(v);
    switch(args) {
    case 2:
        sq_pushroottable(v);
        break;
    case 3:
        // should already have the desired table pushed onto the stack
        break;
    default:
        // Error, unexpected number of arguments
        break;
    }

    return sq_import(v);
}

SQRESULT sqrat_register_importlib(HSQUIRRELVM v) {
    sq_pushroottable(v);

    sq_pushstring(v, _SC("_import_"), -1);
    sq_newclosure(v, &sqratbase_import, 0);
    sq_newslot(v, -3, 0);

    sq_pop(v, 1); // pop sqrat table

    return SQ_OK;
}
