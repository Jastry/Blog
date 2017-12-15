#不加 comm.c make 时只检查 client.c 和 server.c 不会检查 comm.c 是否有修改.

.PHONY:all
all:client server
client : client.c comm.c 
	gcc -g -o $@ $^
server : server.c comm.c
	gcc -g -o $@ $^
.PHONY:clean
clean:
	rm -f server client

