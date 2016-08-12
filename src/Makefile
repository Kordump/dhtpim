CC=clang++
CCLEGACY=clang
CSTD=-std=c++14
CWARN=-Wall -Wextra -pedantic
CEXTRA=-g

CFLAGS=$(CSTD) $(CWARN) $(CEXTRA)
LDFLAGS=-lgnutls -lopendht -lreadline -pthread

EXEC=../bin/dhtpim

all: cmdline.o $(EXEC)

$(EXEC): io_tools.o tools.o opt.o
	$(CC) $(LDFLAGS) $(CFLAGS) *.o dhtpim.cpp -o $@

io_tools.o: io_tools.cpp
	$(CC) $(CFLAGS) -c $^

tools.o: tools.cpp
	$(CC) $(CFLAGS) -c $^

opt.o: opt.cpp
	$(CC) $(CFLAGS) -c $^

cmdline.o: cmdline.c dhtpim.ggo
	gengetopt -i dhtpim.ggo
	$(CCLEGACY) -c cmdline.c

clean-genopt:
	rm -v -f cmdline.h cmdline.c

clean: clean-genopt
	rm -v -f *.o *.gch *.gch.*

mr_proper: clean
	rm -v -f *.out
	rm -v -f $(EXEC)
