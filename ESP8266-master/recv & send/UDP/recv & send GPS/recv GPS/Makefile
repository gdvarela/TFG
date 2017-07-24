# Makefile with variables and rules
P=recvGPS
OBJECTS= recvGPS.o comm.o
CFLAGS = -g -Wall -O3
LDLIBS=
CC=gcc
all: $(P)
# Generate application
$(P): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(P) $(OBJECTS)
# Generate object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o$@
clean:
	rm *.o $(P)
