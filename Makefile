# Makefile

FILESPARS	= $(wildcard src/parser/src/*.c)
FILESSRC	= $(wildcard src/main/*.c)
FILESBICON	= $(wildcard src/BiConProblem/*.c)
CC			= gcc
CFLAGS		= -g -Iinclude/main -Iinclude/BiConProblem -Isrc/parser/include -Isrc/parser
LDLIBS		= -lz3
OBJPARS		= $(FILESPARS:parser/src/%.c=build/%.o)
OBJSRC		= $(FILESSRC:src/main/%.c=build/%.o) $(FILESBICON:src/BiConProblem/%.c=build/%.o)
OBJ 		= build/Parser.o build/Lexer.o $(OBJPARS) $(OBJSRC) 

.PHONY: all
all: graphProblemSolver

graphProblemSolver: $(OBJ) 
		$(CC) $(CFLAGS) $(OBJ) $(LDLIBS) -o graphProblemSolver

build/Lexer.o: src/parser/Lexer.c src/parser/Parser.c
		mkdir -p build
		$(CC) -c $(CFLAGS) $< -o $@

build/Parser.o: src/parser/Parser.c src/parser/Lexer.c
		mkdir -p build
		$(CC) -c $(CFLAGS) $< -o $@

src/parser/Lexer.c:	src/parser/Lexer.l 
		flex --header-file=src/parser/Lexer.h -o src/parser/Lexer.c src/parser/Lexer.l


src/parser/Parser.c:	src/parser/Parser.y src/parser/Lexer.c
		bison --defines=src/parser/Parser.h -o src/parser/Parser.c src/parser/Parser.y

build/%.o:	src/main/%.c 
		mkdir -p build
		$(CC) -c $(CFLAGS) $^ -o $@

build/%.o:	src/BiConProblem/%.c 
		mkdir -p build
		$(CC) -c $(CFLAGS) $^ -o $@

build/%.o:	src/parser/src/%.c
		mkdir -p build
		$(CC) -c $(CFLAGS) $^ -o $@

build/graphUsage.o: examples/graphUsage.c 
		mkdir -p build
		$(CC) -c $(CFLAGS) $^ -o $@

graphParser: build/Lexer.o build/Parser.o $(OBJPARS) build/Graph.o build/graphUsage.o
		$(CC) $(CFLAGS) $^ -o $@

build/Z3Example.o: examples/Z3Example.c 
		mkdir -p build
		$(CC) -c $(CFLAGS) $^ -o $@

Z3Example: build/Z3Example.o build/Z3Tools.o
		$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

.PHONY: doc
doc:
		doxygen doxygen.config
		rm -f doc.html
		ln -s doc/html/files.html doc.html

.PHONY: clean
clean:
		rm -f build/*.o *~ src/parser/Lexer.c src/parser/Lexer.h src/parser/Parser.c src/parser/Parser.h graphProblemSolver graphParser Z3Example doc.html
		rm -rf doc
