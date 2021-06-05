CXX = g++
CFLAGS = -g -Wall 
.DEFAULT_GOAL := make
include ../build/conanbuildinfo.mak
CPPFLAGS += $(addprefix -I, $(CONAN_INCLUDE_DIRS))
LDFLAGS += $(addprefix -L, $(CONAN_LIB_DIRS))
LDLIBS += $(addprefix -l, $(CONAN_LIBS))
OBJM = $(SRCM:.cpp=.o)


FileUtils.o:
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c src/FileUtils.cpp -I./h

PlainFs.o:
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c src/PlainFs.cpp -I./h

CryptoLayer.o:
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c src/CryptoLayer.cpp -I./h

FsBlock.o:
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c src/FsBlock.cpp -I./h -I../h
	
CryptoBlockData.o:
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c src/CryptoBlockData.cpp -I./h

BlockMapper.o:
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c src/BlockMapper.cpp -I./h -I../h

BlockSemaphore.o:
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c src/BlockSemaphore.cpp -I./h
	
Allocation.o:
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c src/Allocation.cpp -I./h -I../h

InodeManager.o:
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -c src/InodeManager.cpp -I./h

make: FileUtils.o PlainFs.o CryptoLayer.o FsBlock.o CryptoBlockData.o BlockMapper.o BlockSemaphore.o Allocation.o InodeManager.o
	echo "done"
	
test: make
	$(CXX) $(CFLAGS) -o Tests  *.o tests/Suite.cpp -I./tests/ -I./h $(CPPFLAGS) $(LDFLAGS) $(LDLIBS)
	mkdir tmp
	./Tests

clean:
	find . -name \*.o -exec rm {} \;
	rm -Rf tmp
	rm -f Tests

