#######################################################
# makefile for compiling and running HTTP server program
# hprakash@iu.edu, hitenderprakash@gmail.com
#######################################################
CC=g++ -std=c++11
OPTS=
INCS=
LIBS= -lpthread

%.o: %.cpp
	$(CC) $(OPTS) $(INCS) -c $<

server: main.o Server.o TCPServer.o UDPServer.o
	$(CC) $(OPTS) $(INCS) -o $@ $^  $(LIBS)

run: server
	@./server

clean:
	rm -f server *.o
