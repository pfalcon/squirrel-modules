dofile("../import.nut")

import("module")
assert(module)
assert(module.func)
assert(module.func2)
assert(module.func() == "This is func")
assert(module.func2() == "func2 speaking")
