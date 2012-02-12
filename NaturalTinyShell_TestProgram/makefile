VPATH = ../
CFLAGS = -std=c99 -I ../
includefiles := $(wildcard ../*.h) cmd.h
sourcefiles = main.c cmd.c
libfiles = ../libs/NaturalTinyShell.a 

main.exe: $(includefiles) $(sourcefiles) $(libfiles)
	$(CC) $(CFLAGS) $(sourcefiles) $(libfiles) -o $@

