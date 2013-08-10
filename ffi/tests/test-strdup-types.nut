ffi = _import_("../ffi", {})

libc = ffi.load("libc.so.6")
print("libc: " + libc + "\n")

strdup = libc.func("s", "strdup", [ffi.pointer])
print(strdup + "\n")

out = strdup("test string")
assert(out == "test string")
