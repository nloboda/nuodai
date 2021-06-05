CXX = g++
CFLAGS = -g -Wall 
PROD_DEPS = `pkg-config --cflags --libs libcurl openssl fuse3 jsoncpp`
TEST_DEPS = `pkg-config --cflags --libs cppunit`
.DEFAULT_GOAL := make

OBJM = $(SRCM:.cpp=.o)

FileUtils.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/FileUtils.cpp -I./h

PlainFs.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/PlainFs.cpp -I./h

CryptoLayer.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/CryptoLayer.cpp -I./h

FsBlock.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/FsBlock.cpp -I./h -I../h
	
CryptoBlockData.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/CryptoBlockData.cpp -I./h

BlockMapper.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/BlockMapper.cpp -I./h -I../h

BlockSemaphore.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/BlockSemaphore.cpp -I./h
	
Allocation.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/Allocation.cpp -I./h -I../h

InodeManager.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS) -c src/InodeManager.cpp -I./h

make: FileUtils.o PlainFs.o CryptoLayer.o FsBlock.o CryptoBlockData.o BlockMapper.o BlockSemaphore.o Allocation.o InodeManager.o
	echo "done"
	
test: make
	$(CXX) $(CFLAGS) -o Tests  *.o tests/Suite.cpp -I./tests/ -I./h $(PROD_DEPS) $(TEST_DEPS)
	mkdir tmp
	./Tests

clean:
	find . -name \*.o -exec rm {} \;
	rm -Rf tmp
	rm -f Tests

