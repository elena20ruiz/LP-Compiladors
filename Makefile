
all: antlr dlg lego

antlr: lego.g
		antlr -gt lego.g

dlg: parser.dlg
		dlg -ci parser.dlg scan.c

lego: lego.c scan.c err.c
		g++ -o lego lego.c scan.c err.c



clean:
	rm -f *.o example1.c scan.c err.c parser.dlg tokens.h mode.h
