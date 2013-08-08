ffi = _import_("../ffi", {})

mod = ffi.load("libc.so.6")
print(mod + "\n")

s_addr = ffi.sym(mod, "strdup")
print(s_addr + "\n")

strdup = ffi.ffi(s_addr, "s", [ffi.pointer])
print(strdup + "\n")

out = strdup("test string")
assert(out == "test string")
