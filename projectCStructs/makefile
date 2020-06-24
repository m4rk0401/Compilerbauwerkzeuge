LEX  = flex
YACC = bison
CC   = gcc
LD   = gcc

all: pTest
scan.c: scan.l pTest.tab.h

pTest.tab.h: pTest.y
	$(YACC) -d -v pTest.y

pTest.tab.c: pTest.y
	$(YACC) -d -v pTest.y 

pTest.tab.o: pTest.tab.c

pTest: scan.o pTest.tab.o
	$(LD) scan.o pTest.tab.o -o pTest

clean: pTest
	$(RM) scan.o scan.c pTest.tab.o pTest.tab.c