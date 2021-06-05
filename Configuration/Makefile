CXX = g++
CFLAGS = -g -Wall 
PROD_DEPS = `pkg-config --cflags --libs libcurl openssl fuse3 jsoncpp`
TEST_DEPS = `pkg-config --cflags --libs cppunit`
.DEFAULT_GOAL := make

Config.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/Config.cpp -I./h

ConfigManager.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/ConfigManager.cpp -I./h

make: Config.o ConfigManager.o

test: make
	$(CXX) $(CFLAGS)  *.o tests/Suite.cpp -I../h -I./tests/ -I./h -I../FileUtils/h -I./h -I../YdClient/h  -o Tests $(PROD_DEPS) $(TEST_DEPS)
	./Tests

clean:
	find . -name \*.o -exec rm {} \;
	rm -f Tests

