CC=clang++
CSTD=-std=c++14
CWARN=-Wall -Wextra -pedantic
CEXTRA=-g

CFLAGS=$(CSTD) $(CWARN) $(CEXTRA)
LDFLAGS=-lgnutls -lopendht -lreadline -pthread

EXEC=dhtpim

all: $(EXEC)

dhtpim: io_tools tools
	$(CC) $(LDFLAGS) $(CFLAGS) *.o dhtpim.cpp -o dhtpim

io_tools:
	$(CC) $(CFLAGS) -c io_tools.cpp

tools:
	$(CC) $(CFLAGS) -c tools.cpp

clean:
	rm -v -f *.o *.gch *.gch.*

mr_proper: clean
	rm -v -f *.out
	rm -v -f $(EXEC)
