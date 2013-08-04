dofile("../import.nut")

import_from("module", "func")
assert(func)
assert(!("func2" in getroottable()))
