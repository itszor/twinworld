# Static recompilation of Twin World (copied from ../../lemmings/recomp; the
# runtime is generic RISC OS code, game.c holds what is Twin World's own)
CC      ?= gcc
CFLAGS  ?= -O2 -g -w -fno-strict-aliasing $(if $(TRACE),-DRECOMP_TRACE)
GEN     := $(wildcard gen/code*.c) gen/entries.c
OBJ     := $(GEN:.c=.o) runtime.o game.o vdu.o display.o sound.o
LIBS    := $(shell pkg-config --libs sdl3) -lm -lpthread
SDLCF   := $(shell pkg-config --cflags sdl3)

twinworld: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)

gen/%.o: gen/%.c runtime.h gen/gen.h
	$(CC) $(CFLAGS) -I. -c -o $@ $<

runtime.o: runtime.c runtime.h os.h game.h
	$(CC) $(CFLAGS) -c -o $@ $<

game.o: game.c game.h
	$(CC) $(CFLAGS) -c -o $@ $<

vdu.o: vdu.c runtime.h os.h
	$(CC) $(CFLAGS) -c -o $@ $<

sound.o: sound.c runtime.h os.h
	$(CC) $(CFLAGS) $(SDLCF) -c -o $@ $<

display.o: display.c os.h game.h shaders/crt.frag.spv.h
	$(CC) $(CFLAGS) $(SDLCF) -c -o $@ $<

# Coverage build (~/stuff/superspider/phase0): "make cov" builds twinworld-cov
# from separate *.cov.o objects; it writes $$RECOMP_COV (default cov.out).
COVDIR  ?= $(HOME)/stuff/superspider/phase0/cov
COVOBJ  := $(OBJ:.o=.cov.o) cov.cov.o

cov: twinworld-cov

twinworld-cov: $(COVOBJ)
	$(CC) $(CFLAGS) -o $@ $(COVOBJ) $(LIBS)

%.cov.o: %.c runtime.h os.h game.h $(COVDIR)/cov.h
	$(CC) $(CFLAGS) $(SDLCF) -DRECOMP_COV -I. -I$(COVDIR) -c -o $@ $<

cov.cov.o: $(COVDIR)/cov.c $(COVDIR)/cov.h
	$(CC) $(CFLAGS) -c -o $@ $<

regen:
	cd ../analysis && ./run.sh
	python3 gen_c.py

clean:
	rm -f gen/*.o runtime.o game.o vdu.o display.o sound.o twinworld twinworld-cov *.cov.o gen/*.cov.o
