CXX = g++
CFLAGS = -g -Wall -D_FILE_OFFSET_BITS=64
PROD_DEPS = `pkg-config --cflags --libs libcurl openssl fuse3 jsoncpp`
TEST_DEPS = `pkg-config --cflags --libs cppunit`
.DEFAULT_GOAL := make
include ../build/conanbuildinfo.mak
CPPFLAGS += $(addprefix -I, $(CONAN_INCLUDE_DIRS))
LDFLAGS += $(addprefix -L, $(CONAN_LIB_DIRS))
LDLIBS += $(addprefix -l, $(CONAN_LIBS))

FuseCLIHandler.o:
	$(CXX) $(CFLAGS) $(addprefix -I, $(CONAN_INCLUDE_DIRS))  -c src/FuseCLIHandler.cpp -I./h -I../Configuration/h -I../FileSystem/h -I../FileUtils/h

FuseOperationsSupport.o:
	$(CXX) $(CFLAGS) $(addprefix -I, $(CONAN_INCLUDE_DIRS))  ../FileSystem/Directory.o ../Configuration/ConfigManager.o  -c src/FuseOperationsSupport.cpp \
	-I./h -I../Configuration/h -I../FileSystem/h -I../FileUtils/h


make: FuseCLIHandler.o FuseOperationsSupport.o
		echo "done" 

test: make
	echo "make test"
clean:
	rm -Rf out/*
	rm -f *.o

