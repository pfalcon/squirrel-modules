import("ffi")

local l = ffi.load("libc.so.6")

local function _(r, f, p) {
    this[f] = l.func(r, f, p)
}

AF_INET = 2
SOCK_STREAM = 1

_("i", "socket", ["i", "i", "i"])
_("i", "connect", ["i", "P", "I"])

function sockaddr_in(sin_addr, sin_port) {
    sin_family = AF_INET
    addr = blob()
    addr.writen(sin_family, 'w')
    addr.writen(swap2(sin_port), 'w')
    addr.writen(swap4(sin_addr), 'i')
    // padding
    addr.writen(0, 'i')
    addr.writen(0, 'i')
    return addr
}
