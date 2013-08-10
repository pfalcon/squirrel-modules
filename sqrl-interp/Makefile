TOP = ../squirrel-lang
OPT_EXTRA = -DEXTRA_STATIC_MODULES
SQIMPORT = ../binmodule
INCLUDE_EXTRA = -I$(SQIMPORT)
VPATH = $(TOP)
SQ_OBJS_EXTRA = $(SQIMPORT)/sqimport.o static_modules_init.o
LDLIBS = -ldl

# For VPATH to work as expected, there should be NO compiled
# objects in VPATH directory, so let's force clean them.
dummy := $(shell cd $(TOP); make -f makefile clean)

include $(TOP)/makefile

install: stdlibinit.nut
	mkdir -p ~/.squirrel/lib/
	cp $^ ~/.squirrel/lib/
