CXX = g++
CFLAGS = -g -Wall 
PROD_DEPS = `pkg-config --cflags --libs libcurl openssl fuse3 jsoncpp`
TEST_DEPS = `pkg-config --cflags --libs cppunit`
.DEFAULT_GOAL := make
include ../build/conanbuildinfo.mak
CPPFLAGS += $(addprefix -I, $(CONAN_INCLUDE_DIRS))
LDFLAGS += $(addprefix -L, $(CONAN_LIB_DIRS))
LDLIBS += $(addprefix -l, $(CONAN_LIBS))


Directory.o:
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c src/Directory.cpp -I./h

make:  Directory.o
	echo "done"

test: make
	echo "nothing to do"

test: make
	$(CXX) $(CFLAGS) -o Tests  *.o tests/Suite.cpp -I./tests/ -I./h $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)
	./Tests

clean:
	find . -name \*.o -exec rm {} \;
	rm -f Tests
