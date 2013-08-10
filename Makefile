MODULES = sample import ffi

all: sqrl modules

sqrl:
	make -C sqrl-interp

modules:
	for m in $(MODULES); do make -C $$m; done

install: sqrl modules
	for m in $(MODULES); do make -C $$m install; done
	make -C sqrl-interp install
