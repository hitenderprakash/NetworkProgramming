#######################################################
# makefile for compiling and running UDPClient program
# hprakash@iu.edu, hitenderprakash@gmail.com
#######################################################
CC=g++ -std=c++11
OPTS=
INCS=
LIBS=

%.o: %.cpp
	$(CC) $(OPTS) $(INCS) -c $<

UDPClient: UDPClient.o
	$(CC) $(OPTS) $(INCS) -o $@ $^  $(LIBS)

run: UDPClient
	@./UDPClient

clean:
	rm -f UDPClient *.o
