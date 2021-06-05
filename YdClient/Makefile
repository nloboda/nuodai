CXX = g++
CFLAGS = -g -Wall 
PROD_DEPS = `pkg-config --cflags --libs libcurl openssl fuse3 jsoncpp`
TEST_DEPS = `pkg-config --cflags --libs cppunit`
.DEFAULT_GOAL := make

YandexAuthenticator.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/YandexAuthenticator.cpp -I../FileUtils/h -I../Configuration/h -I./h 

YandexDiskClient.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/YandexDiskClient.cpp -I./h -I../FileUtils/h -I../Configuration/h

Auth.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/Auth.cpp -I./h

make: Auth.o YandexAuthenticator.o YandexDiskClient.o
	#$(CXX) $(PROD_DEPS) YandexAuthenticator.o -o a.out
	echo "done"

test: make
	echo "Nothing to do"

clean:
	find . -name \*.o -exec rm {} \;

