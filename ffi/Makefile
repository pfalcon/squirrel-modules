SQUIRREL_PATH = ../squirrel-lang
SQMODULE_PATH = ../binmodule

CFLAGS = -I$(SQUIRREL_PATH)/include -I$(SQMODULE_PATH) $(OPT)

MODULE = ffi

$(MODULE).nud: ffi.o
	$(CC) $(CFLAGS) --shared -Wl,--no-undefined $^ -o $@ -ldl -lffi

check test: $(MODULE).nud
	(cd tests; ../../test-support/run-tests.sh)

install: $(MODULE).nud
	mkdir -p ~/.squirrel/lib/
	cp $^ ~/.squirrel/lib/
