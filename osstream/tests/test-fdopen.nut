osstream = _import_("../osstream", {})

f = osstream.fdopen(1)
f.write("hello stdout\n")

#stdin = osstream.fdopen(0)
#print(stdin.read(100))
