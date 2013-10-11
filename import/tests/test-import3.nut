dofile("../import.nut")

import("module3")
assert(module3)
bar = module3.Bar()
assert(bar.val == "foo")
