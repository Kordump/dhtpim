CC=clang++
CSTD=-std=c++14
CWARN=-Wall -Wextra -pedantic
CEXTRA=-g

CFLAGS=$(CSTD) $(CWARN) $(CEXTRA)
LDFLAGS=-lgnutls -lopendht -lreadline -pthread

EXEC=dhtpim

all: $(EXEC)

dhtpim:
	$(CC) $(LDFLAGS) $(CFLAGS) dhtpim.cpp -o dhtpim

clean:
	rm -v -f *.o *.gch *.gch.*

mr_proper: clean
	rm -v -f *.out
	rm -v -f $(EXEC)
