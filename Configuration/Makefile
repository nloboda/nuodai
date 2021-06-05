CXX = g++
CFLAGS = -g -Wall 

TEST_DEPS = `pkg-config --cflags --libs cppunit`
.DEFAULT_GOAL := make

include ../build/conanbuildinfo.mak
CPPFLAGS += $(addprefix -I, $(CONAN_INCLUDE_DIRS))
LDFLAGS += $(addprefix -L, $(CONAN_LIB_DIRS))
LDLIBS += $(addprefix -l, $(CONAN_LIBS))


Config.o: 
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c src/Config.cpp -I./h

ConfigManager.o:
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c src/ConfigManager.cpp -I./h

make: Config.o ConfigManager.o

test: make
	$(CXX) *.o tests/Suite.cpp -I../h -I./tests/ -I./h -I../FileUtils/h -I./h -I../YdClient/h  $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)  -o Tests 
	./Tests

clean:
	find . -name \*.o -exec rm {} \;
	rm -f Tests

