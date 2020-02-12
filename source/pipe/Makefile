IDIR = ../common
ODIR = obj
cc = gcc
CFLAGS = -Wall -g -O3
LIBS = -lrt -lm

_DEPS = argument_parse.h benchmark.h process.h signals.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = argument_parse.o benchmark.o process.o signals.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
	$(cc) -c -o $@ $< $(CFLAGS) 

#shm.o: $(DEPS)
#client.o: $(DEPS)
#server.o:$(DEPS)

all: pipe

$(ODIR)/%.o: %.c $(DEPS)
	$(cc) -c -o $@ $< $(CFLAG) $(LIBS)

pipe: $(OBJ) $(ODIR)/pipe.o
	$(cc) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *.o pipe
