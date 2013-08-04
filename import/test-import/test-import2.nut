dofile("../import.nut")

import("module", "bar")
assert(bar)
assert(!("module" in getroottable()))
