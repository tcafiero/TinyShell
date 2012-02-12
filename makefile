VPATH = ./
CFLAGS = -std=c99
includefiles := $(wildcard ./*.h)
sourcefiles := $(wildcard ./*.c)
objectfiles := $(patsubst %.c,%.o,$(wildcard ./*.c))

all: $(objectfiles)

$(objectfiles):%.o:%.c $(includefiles)
	$(CC) -c $(CFLAGS) $< -o $@
	ar rcs ./libs/NaturalTinyShell.a $@
	rm $@

