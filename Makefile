

all: lib src

lib:
	make -C lib

src: lib
	make -C src

clean-lib:
	make -C lib clean

clean-src:
	make -C src clean

clean: clean-lib clean-src

install: src
	make -C src install

.PHONY: lib src clean clean-lib clean-src install

# vim: set noexpandtab:
