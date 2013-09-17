#include <sqmodule.h>
#include <sqmodule_helpers.h>
#include <sqstdio.h>
#include <stdio.h>
#include <string.h>
#include <new>

struct POpenStream : public SQStream
{
    POpenStream(FILE *file) { f = file; }
    virtual ~POpenStream() { Close(); }

    void Close()
    {
        if (f) {
            pclose(f);
            f = NULL;
        }
    }

    SQInteger Read(void *buffer,SQInteger size)
        { return fread(buffer, 1, size, f); }

    SQInteger Readline(void *buffer, SQInteger size)
    {
        char *p = fgets((char*)buffer, size, f);
        if (!p)
            return 0;
        return strlen(p);
    }

    SQInteger Write(void *buffer,SQInteger size)
        { return fwrite(buffer,1,size, f); }

    SQInteger Flush()
        { return fflush(f); }

    // Cannot seek a pipe: http://pubs.opengroup.org/onlinepubs/9699919799/functions/fseek.html
    SQInteger Tell()
        { return -1; }
    SQInteger Len()
        { return -1; }
    SQInteger Seek(SQInteger offset, SQInteger origin)
        { return -1; }

    bool IsValid()
        { return f ? true : false; }

    bool EOS()
        { return feof(f); }

private:
        FILE *f;
};

static const SQChar popen_class[] = _SC("popen");

static SQInteger popen_close(HSQUIRRELVM v)
{
    POpenStream *self = NULL;
    if (SQ_SUCCEEDED(sq_getinstanceup(v, 1, (SQUserPointer*)&self, (SQUserPointer)popen_class))) {
        if (self)
            self->Close();
    }
    return 0;
}

static SQInteger popen_releasehook(SQUserPointer p, SQInteger size)
{
    printf("in release hook\n");
    POpenStream *self = (POpenStream*)p;
    self->~POpenStream();
    sq_free(self, sizeof(POpenStream));
    return 1;
}

static SQInteger popen_constructor(HSQUIRRELVM v)
{
printf("In constructor\n");
    const SQChar *cmd;
    const SQChar *mode;
    sq_getstring(v, 2, &cmd);
    sq_getstring(v, 3, &mode);

    FILE *f = popen(cmd, mode);
    printf("popen=%p\n", f);

    POpenStream *s = new (sq_malloc(sizeof(POpenStream)))POpenStream(f);

    if (SQ_FAILED(sq_setinstanceup(v, 1, s))) {
        s->~POpenStream();
        sq_free(s, sizeof(POpenStream));
        return sq_throwerror(v, _SC("cannot create popen"));
    }
    sq_setreleasehook(v, 1, popen_releasehook);
    return 0;
}


static SQRegFunction popen_methods[] = {
    {"constructor", popen_constructor, 3, _SC("xss")},
    {"close", popen_close, 1, _SC("x")},
    {NULL}
};

SQRESULT init_popen(HSQUIRRELVM v)
{
    return declare_stream(v, popen_class, popen_methods);
}
