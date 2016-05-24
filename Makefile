all:
	g++ daemon.cpp alert.cpp bd.cpp email.cpp -lcurl -lzmq -o main
