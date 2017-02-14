all:
	$(MAKE) -C src && $(MAKE) -C tests

clean:
	$(MAKE) clean -C src && $(MAKE) clean -C tests

.phony: tags

tags:
	ctags -R *

