#include <sqmodule.h>
#include <sqmodule_helpers.h>
#include <sqstdio.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <new>

struct FdOpenStream : public SQStream
{
    FdOpenStream(int fd) { _fd = fd; }
    virtual ~FdOpenStream() { Close(); }

    void Close()
    {
        if (_fd != -1) {
            close(_fd);
            _fd = -1;
        }
    }

    SQInteger Read(void *buffer, SQInteger size)
        { return read(_fd, buffer, size); }

    SQInteger Readline(void *buffer, SQInteger size)
        { return -1; }

    SQInteger Write(void *buffer,SQInteger size)
        { return write(_fd, buffer, size); }

    SQInteger Flush()
        { return fsync(_fd); }

    SQInteger Tell()
        { return lseek(_fd, 0, SEEK_CUR); }
    SQInteger Len() {
        // TODO: use stat
        return -1;
    }
    SQInteger Seek(SQInteger offset, SQInteger origin)
        { return lseek(_fd, offset, origin); }

    bool IsValid()
        { return _fd != -1; }

    bool EOS()
    {
        // Not implemented - use Read() result
        return false;
    }

private:
        int _fd;
};

static const SQChar fdopen_class[] = _SC("fdopen");

static SQInteger fdopen_close(HSQUIRRELVM v)
{
    FdOpenStream *self = NULL;
    if (SQ_SUCCEEDED(sq_getinstanceup(v, 1, (SQUserPointer*)&self, (SQUserPointer)fdopen_class))) {
        if (self)
            self->Close();
    }
    return 0;
}

static SQInteger fdopen_releasehook(SQUserPointer p, SQInteger size)
{
    printf("in release hook\n");
    FdOpenStream *self = (FdOpenStream*)p;
    self->~FdOpenStream();
    sq_free(self, sizeof(FdOpenStream));
    return 1;
}

static SQInteger fdopen_constructor(HSQUIRRELVM v)
{
printf("In fdopen constructor\n");
    SQInteger fd;
    sq_getinteger(v, 2, &fd);

    FdOpenStream *s = new (sq_malloc(sizeof(FdOpenStream)))FdOpenStream(fd);

    if (SQ_FAILED(sq_setinstanceup(v, 1, s))) {
        s->~FdOpenStream();
        sq_free(s, sizeof(FdOpenStream));
        return sq_throwerror(v, _SC("cannot create fdopen"));
    }
    sq_setreleasehook(v, 1, fdopen_releasehook);
    return 0;
}


static SQRegFunction fdopen_methods[] = {
    {"constructor", fdopen_constructor, 2, _SC("xi")},
    {"close", fdopen_close, 1, _SC("x")},
    {NULL}
};

SQRESULT init_fdopen(HSQUIRRELVM v)
{
    return declare_stream(v, fdopen_class, fdopen_methods);
}
