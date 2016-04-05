.PHONY: lib clean
BINS=admin client freecoind miner testing

default: all
all: $(BINS)
dist: default
	make dist -C lib

lib:
	make -C lib

$(BINS): lib
	make -C $@

clean:
	$(foreach dir,$(BINS), make clean -C $(dir);)
	make clean -C lib
