make: bbserver bbclient
	gcc -o bbserver bbserver.o
	gcc -pthread -o bbclient bbclient.o
bbclient.o: bbclient.c
	gcc -g -Wall -c bbclient.c
bbserver.o: bbserver.c
	gcc -g -Wall -c bbserver.c
clean: 
	clear
	rm bbserver bbclient bbserver.o bbclient.o .bbclient.c.swp .bbserver.c.swp
