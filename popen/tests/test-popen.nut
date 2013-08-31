popen = _import_("../popen", {})

l = file("../popen.cpp", "r").readline()

f = popen.popen("cat ../popen.cpp", "r")
#print("f = " + f + "\n")
l_popen = f.readline()
#print("*" + l_popen  + "*\n")
assert(l_popen == l)
