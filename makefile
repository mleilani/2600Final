CC = gcc
MAIN = src/tictactoe.c
# WINDOWS: use ~ mingw32-make [ command ]
# MAC/LINUS: use ~ make [command]



ifeq ($(OS), Windows_NT)
	DELETE = del
	EXT=.exe
else 
	UNAME_S:=$(shell uname -s)
	ifeq ($(UNAME_S), Darwin)
		DELETE = rm -f
		EXT=.out
	endif
endif

all: program

program:

	$(CC) -o bin/program$(EXT) $(MAIN) -lpaho-mqtt3c

run: bin/program$(EXT)
	bin/program$(EXT)

clean: 
	$(DELETE) bin\*$(EXT)