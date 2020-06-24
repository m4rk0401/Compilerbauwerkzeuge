LEX=flex
scanner: scan.o
	$(CC) scan.o -o	scanner
scan.o: scan.c
scan.c: scan.l
clean: scanner	
	$(RM) scan.o scan.c 