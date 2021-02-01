hashtabledir = ../hashtable/
holdalldir = ../holdall/
lwordsdir = ../lwords/
CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic -I$(hashtabledir) -I$(holdalldir) -I$(lwordsdir) -O2
LDFLAGS =
VPATH = $(hashtabledir):$(holdalldir):$(lwordsdir)
objects = $(hashtabledir)hashtable.o $(holdalldir)holdall.o $(lwordsdir)lwords.o  main.o
executable = main

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)

$(hashtabledir)hashtable.o: hashtable.c hashtable.h
$(holdalldir)holdall.o: holdall.c holdall.h
$(lwordsdir)lwords.o: lwords.c lwords.h
main.o: main.c hashtable.h holdall.h lwords.h
