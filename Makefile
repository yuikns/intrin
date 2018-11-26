all:
	gcc example/example.c -o intrin -O3 -Wall -I.

clean:
	-rm -rf *~ *.swp *.o *.exe intrin 

