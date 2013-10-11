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
_("i", "bind", ["i", "P", "I"])
_("i", "listen", ["i", "i"])
_("i", "accept", ["i", "P", "P"])

function sockaddr_in(sin_addr, sin_port) {
    assert(type(sin_addr) == "array")
    assert(sin_addr.len() == 4)
    // Layout is specific to x86 ABI
    local sin_family = AF_INET
    local addr = blob()
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
// object implementing stream interface.
function ClientSocket(addr, port) {
    sa = sockaddr_in(addr, port)
    fd = socket(AF_INET, SOCK_STREAM, 0)
    r = connect(fd, sa, sa.len())
    if (r == -1)
        throw "error in connect()"
    return fdopen(fd)
}


class ServerSocket {

    fd = -1

    constructor(addr, port, queue_sz = 5) {
        local sa = sockaddr_in(addr, port)
        fd = socket(AF_INET, SOCK_STREAM, 0)
        local r = bind(fd, sa, sa.len())
        if (r == -1)
            throw "error in bind()"
        r = listen(fd, queue_sz)
        if (r == -1)
            throw "error in listen()"
    }

    function accept() {
        local sa = sockaddr_in([0, 0, 0, 0], 0)
        local lenbuf = blob(4)
        lenbuf.writen(sa.len(), 'i')
        local lfd = ::accept(fd, sa, lenbuf);
        if (lfd == -1)
            throw "error in accept()"
        return fdopen(lfd)
    }
}
