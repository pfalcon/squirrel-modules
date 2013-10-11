import("ffi")

local l = ffi.load("libc.so.6")

local function _(r, f, p) {
    this[f] = l.func(r, f, p)
}

_("i", "open", ["s", "i", "i"])
_("i", "read", ["i", "P", "I"])
_("i", "write", ["i", "P", "I"])
_("i", "close", ["i"])
_("i", "perror", ["s"])

errno = l.var("i", "errno")

function _errno() {
    return errno.get()
}
