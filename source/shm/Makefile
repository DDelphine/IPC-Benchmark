IDIR = ../common
ODIR = obj
cc = gcc
CFLAGS = -Wall -g -O3
LIBS = -lrt -lm

_DEPS = argument_parse.h benchmark.h process.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = argument_parse.o benchmark.o process.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
	$(cc) -c -o $@ $< $(CFLAGS) 

#shm.o: $(DEPS)
#client.o: $(DEPS)
#server.o:$(DEPS)

all: shm client server

$(ODIR)/%.o: %.c $(DEPS)
	$(cc) -c -o $@ $< $(CFLAG) $(LIBS)

shm: $(OBJ) $(ODIR)/shm.o
	$(cc) -o $@ $^ $(CFLAGS) $(LIBS)

client: $(OBJ) $(ODIR)/client.o
	$(cc) -o $@ $^ $(CFLAGS) $(LIBS)

server: $(OBJ) $(ODIR)/server.o
	$(cc) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *.o shm client server
