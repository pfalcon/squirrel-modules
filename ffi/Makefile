SQUIRREL_PATH = ../../squirrel-lang
SQMODULE_PATH = ../binmodule

CFLAGS = -I$(SQUIRREL_PATH)/include -I$(SQMODULE_PATH) $(OPT)

ffi.nud: ffi.o
	$(CC) $(CFLAGS) --shared -Wl,--no-undefined $^ -o $@ -ldl -lffi
