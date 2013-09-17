SQUIRREL_PATH = ../squirrel-lang
SQMODULE_PATH = ../binmodule

CFLAGS = -I$(SQUIRREL_PATH)/include -I$(SQMODULE_PATH) $(OPT)
CXXFLAGS = $(CFLAGS)

MODULE = osstream

$(MODULE).nud: $(MODULE).o popen.o fdopen.o
	$(CXX) $(CXXFLAGS) --shared -Wl,--no-undefined $^ -o $@

check test: $(MODULE).nud
	(cd tests; ../../test-support/run-tests.sh)

install: $(MODULE).nud
	mkdir -p ~/.squirrel/lib/
	cp $^ ~/.squirrel/lib/
