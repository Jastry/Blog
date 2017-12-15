.PHONY: all
all : server client
server : server.c
	gcc -o $@ $^
client : client.c
	gcc -o $@ $^

.PHNONY: clean
clean:
	rm -f server client
#pipe:pipe.c
#	gcc -o $@ $^
#.PHONY:clean
#clean:
#	rm -f pipe
#test:TestPipeSize.c
#	gcc -o $@ $^
#.PHONY:clean
#clean:
#	rm -f test
