TOP = ../squirrel-lang
OPT_EXTRA = -DEXTRA_STATIC_MODULES
SQIMPORT = ../binmodule
INCLUDE_EXTRA = -I$(SQIMPORT)
VPATH = $(TOP)
SQ_OBJS_EXTRA = $(SQIMPORT)/sqimport.o static_modules_init.o
LDLIBS = -ldl
# For backtrace support
# -rdynamic

# For VPATH to work as expected, there should be NO compiled
# objects in VPATH directory, so let's force clean them.
dummy := $(shell cd $(TOP); make -f makefile clean)

include $(TOP)/makefile

install: sqrl sqrl.rlwrap stdlibinit.nut
	mkdir -p ~/.squirrel/lib/
	cp stdlibinit.nut ~/.squirrel/lib/
	mkdir -p ~/bin
	cp sqrl ~/bin/sqrl.bin
	cp sqrl.rlwrap ~/bin/sqrl
	@echo "=================================="
	@echo "Squirrel interpreter with import support was installed in ~/bin/ ."
	@echo "Make sure that you have that directory in your PATH and start"
	@echo "interpreter using 'sqrl' command. 'rlwrap' package is recommended"
	@echo "for comfortable interactive usage."
	@echo "=================================="
