CXX = g++
CFLAGS = -g -Wall 
PROD_DEPS = `pkg-config --cflags --libs libcurl openssl fuse3 jsoncpp`
TEST_DEPS = `pkg-config --cflags --libs cppunit`
.DEFAULT_GOAL := make

Directory.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/Directory.cpp -I./h

make:  Directory.o
	echo "done"

test: make
	echo "nothing to do"

clean:
	rm -Rf *.o