CXX = g++
CFLAGS = -g -Wall 
PROD_DEPS = `pkg-config --cflags --libs libcurl openssl fuse3 jsoncpp`
TEST_DEPS = `pkg-config --cflags --libs cppunit`
.DEFAULT_GOAL := make

Configuration.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) ../FileUtils/CryptoBlockData.o -c src/Configuration.cpp -I../FileUtils/h -I./h -I../YdClient/h


make: Configuration.o
	#$(CXX) $(PROD_DEPS) YandexAuthenticator.o -o a.out
	echo "done"

test: make
	echo "Nothing to do"

clean:
	rm -Rf out/*

