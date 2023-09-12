all: server client test

server: server.c 
	gcc -Wno-everything server.c -o server

client: client.c
	gcc -Wno-everything client.c -o client

test: test.c
	gcc -Wno-everything test.c -o test

clean:
	rm -f client server test
	rm -f *.txt
