dofile("../import.nut")

import("module", "bar")
assert(bar)
assert(!("module" in getroottable()))
assert(bar.func() == "This is func")
assert(bar.func2() == "func2 speaking")
