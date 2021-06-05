CXX = g++
CFLAGS = -g -Wall 
PROD_DEPS = `pkg-config --cflags --libs libcurl openssl fuse3 jsoncpp`
TEST_DEPS = `pkg-config --cflags --libs cppunit`
.DEFAULT_GOAL := make

FuseCLIHandler.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS)  -c src/FuseCLIHandler.cpp -I./h -I../Configuration/h -I../FileSystem/h -I../FileUtils/h

FuseOperationsSupport.o:
	$(CXX) $(CFLAGS) $(PROD_DEPS)  ../FileSystem/Directory.o ../Configuration/ConfigManager.o  -c src/FuseOperationsSupport.cpp \
	-I./h -I../Configuration/h -I../FileSystem/h -I../FileUtils/h


make: FuseCLIHandler.o FuseOperationsSupport.o
		echo "done" 

test: make
	echo "make test"
clean:
	rm -Rf out/*
	rm -f *.o

