CXX = g++
CFLAGS = -g -Wall 
PROD_DEPS = `pkg-config --cflags --libs libcurl openssl fuse3 jsoncpp`
TEST_DEPS = `pkg-config --cflags --libs cppunit`
.DEFAULT_GOAL := make

FuseAdapter.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/FuseAdapter.cpp -I./h 


make: FuseAdapter.o
	echo "done"
clean:
	rm -Rf out/*
	rm -f *.o

