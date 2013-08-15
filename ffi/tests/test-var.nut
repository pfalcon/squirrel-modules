_import_("../ffi")

libc = load("libc.so.6")

errno = libc.var("i", "errno")
print("errno: " + errno.get() + "\n")
#assert(errno.get() == 0)

function wrap(rett, funcn, paramt) {
    getroottable()[funcn] = libc.func(rett, funcn, paramt)
}

wrap("i", "socket", ["i", "i", "i"])
wrap("i", "perror", ["s"])

# Random, diffenitily unsupported values
s = socket(100, 100, 100)
print("errno: " + errno.get() + "\n")
perror("perror")
# EINVAL
assert(errno.get() == 22)
