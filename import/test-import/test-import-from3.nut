dofile("../import.nut")

import_from("module", ["func2"])
assert(func2)
assert(!("func" in getroottable()))
