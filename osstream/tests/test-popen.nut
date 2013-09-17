osstream = _import_("../osstream", {})

l = file("../popen.cpp", "r").readline()

f = osstream.popen("cat ../popen.cpp", "r")
#print("f = " + f + "\n")
l_popen = f.readline()
#print("*" + l_popen  + "*\n")
assert(l_popen == l)
