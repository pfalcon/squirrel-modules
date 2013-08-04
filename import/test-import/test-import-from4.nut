dofile("../import.nut")

import_from("module", "func2", "foofunc")
assert(foofunc)
assert(!("func" in getroottable()))
assert(!("func2" in getroottable()))
