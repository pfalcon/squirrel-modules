all:

install: import.nut
	mkdir -p ~/.squirrel/lib/
	cp $^ ~/.squirrel/lib/

check test:
	(cd tests; ../../test-support/run-tests.sh)
