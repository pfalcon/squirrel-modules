dofile("../import.nut")

import_from("module", "*")
assert(!("module" in getroottable()))
assert(func)
assert(func2)
assert(func() == "This is func")
assert(func2() == "func2 speaking")
