all: cl
	g++ -o daemon ASDaemon.cpp -std=c++11 -lcurl -lpthread -lzmq
	
cl:
	g++ -o cl myprog.cpp -std=c++11 -lzmq 