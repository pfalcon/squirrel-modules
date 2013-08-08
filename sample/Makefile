SQUIRREL_PATH = ../../squirrel-lang
SQMODULE_PATH = ../binmodule

CFLAGS = -I$(SQUIRREL_PATH)/include -I$(SQMODULE_PATH) $(OPT)

MODULE = sample

$(MODULE).nud: sample.o
	$(CC) $(CFLAGS) --shared -Wl,--no-undefined $^ -o $@

sample.o: sample.c

check test: $(MODULE).nud
	(cd tests; ../../test-support/run-tests.sh)
