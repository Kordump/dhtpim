CC=clang++
CCLEGACY=clang
CSTD=-std=c++14
CWARN=-Wall -Wextra -pedantic
CEXTRA=-g

CFLAGS=$(CSTD) $(CWARN) $(CEXTRA)
LDFLAGS=-lgnutls -lopendht -lreadline -pthread

SRC=./src/
BIN=./bin/

EXEC=dhtpim

all: genopt $(EXEC)

dhtpim: io_tools tools opt
	$(CC) $(LDFLAGS) $(CFLAGS) *.o $(SRC)dhtpim.cpp -o $(BIN)dhtpim

io_tools:
	$(CC) $(CFLAGS) -c $(SRC)io_tools.cpp

tools:
	$(CC) $(CFLAGS) -c $(SRC)tools.cpp

opt:
	$(CC) $(CFLAGS) -c $(SRC)opt.cpp

genopt:
	gengetopt -i $(SRC)dhtpim.ggo --output-dir $(SRC)
	$(CCLEGACY) -c $(SRC)cmdline.c

clean-genopt:
	rm -v -f $(SRC)cmdline.h $(SRC)cmdline.c

clean: clean-genopt
	rm -v -f *.o *.gch *.gch.*

mr_proper: clean
	rm -v -f *.out
	rm -v -f $(BIN)$(EXEC)
