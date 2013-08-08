dofile("../import.nut")

import_from("module", "func2", "foofunc")
assert(foofunc)
assert(!("func" in getroottable()))
assert(!("func2" in getroottable()))
assert(!("module" in getroottable()))
assert(foofunc() == "func2 speaking")
