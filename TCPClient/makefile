#######################################################
# makefile for compiling and running TCPClient program
# hprakash@iu.edu, hitenderprakash@gmail.com
#######################################################
CC=g++ -std=c++11
OPTS=
INCS=
LIBS=

%.o: %.cpp
	$(CC) $(OPTS) $(INCS) -c $<

TCPClient: TCPClient.o
	$(CC) $(OPTS) $(INCS) -o $@ $^  $(LIBS)

run: TCPClient
	@./TCPClient

clean:
	rm -f TCPClient *.o
