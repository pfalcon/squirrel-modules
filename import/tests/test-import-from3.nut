dofile("../import.nut")

import_from("module", ["func2"])
assert(func2)
assert(!("func" in getroottable()))
assert(!("module" in getroottable()))
assert(func2() == "func2 speaking")
