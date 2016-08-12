CC=clang++
CCLEGACY=clang
CSTD=-std=c++14
CWARN=-Wall -Wextra -pedantic
CEXTRA=-g

CFLAGS=$(CSTD) $(CWARN) $(CEXTRA)
LDFLAGS=-lgnutls -lopendht -lreadline -pthread

EXEC=dhtpim

all: genopt $(EXEC)

dhtpim: io_tools tools opt
	$(CC) $(LDFLAGS) $(CFLAGS) *.o dhtpim.cpp -o dhtpim

io_tools:
	$(CC) $(CFLAGS) -c io_tools.cpp

tools:
	$(CC) $(CFLAGS) -c tools.cpp

opt:
	$(CC) $(CFLAGS) -c opt.cpp

genopt:
	gengetopt -i dhtpim.ggo
	$(CCLEGACY) -c cmdline.c

clean-genopt:
	rm -v -f cmdline.h cmdline.c

clean: clean-genopt
	rm -v -f *.o *.gch *.gch.*

mr_proper: clean
	rm -v -f *.out
	rm -v -f $(EXEC)
