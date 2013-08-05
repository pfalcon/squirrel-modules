SQUIRREL_PATH = ../../squirrel-lang
SQMODULE_PATH = ../binmodule

CFLAGS = -I$(SQUIRREL_PATH)/include -I$(SQMODULE_PATH) $(OPT)

sample.nud: sample.o
	$(CC) $(CFLAGS) --shared $^ -o $@

sample.o: sample.c
