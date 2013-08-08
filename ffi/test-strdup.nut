_import_("ffi")

mod = dlopen("libc.so.6")
print(mod + "\n")

s_addr = dlsym(mod, "strdup")
print(s_addr + "\n")

strdup = ffi(s_addr, "s", ["s"])
print(strdup + "\n")

out = strdup("test string")
assert(out == "test string")
