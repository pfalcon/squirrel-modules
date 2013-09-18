import("ffi")
import_from("osstream", "fdopen")

local l = ffi.load("libc.so.6")

local function _(r, f, p) {
    this[f] = l.func(r, f, p)
}

AF_INET = 2
SOCK_STREAM = 1

_("i", "socket", ["i", "i", "i"])
_("i", "connect", ["i", "P", "I"])

function sockaddr_in(sin_addr, sin_port) {
    // Layout is specific to x86 ABI
    sin_family = AF_INET
    addr = blob()
    addr.writen(sin_family, 'w')
    addr.writen(swap2(sin_port), 'w')
    //addr.writen(swap4(sin_addr), 'i')
    addr.writen(sin_addr[0], 'b')
    addr.writen(sin_addr[1], 'b')
    addr.writen(sin_addr[2], 'b')
    addr.writen(sin_addr[3], 'b')
    // padding
    addr.writen(0, 'i')
    addr.writen(0, 'i')
    return addr
}

// Factory function for higher-level (connected) socket
//object implementing stream interface.
function Socket(addr, port) {
    assert(type(addr) == "array")
    assert(addr.len() == 4)
    sa = sockaddr_in(addr, port)
    fd = socket(AF_INET, SOCK_STREAM, 0)
    r = connect(fd, sa, sa.len())
    if (r == -1)
        throw "error in connect()"
    return fdopen(fd)
}
