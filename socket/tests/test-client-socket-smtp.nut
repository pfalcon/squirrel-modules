socket = _import_("../socket", {})

s = socket.ClientSocket([127, 0, 0, 1], 25)
print(s.read(100))
