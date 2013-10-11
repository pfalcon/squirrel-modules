import("socket")

s = socket.ServerSocket([127, 0, 0, 1], 8480)
print("Waiting for connection, hit me with browser on http://localhost:8480\n")
ls = s.accept()
print(ls.read(300))
