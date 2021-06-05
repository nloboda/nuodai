CXX = g++
CFLAGS = -g -Wall -D_FILE_OFFSET_BITS=64
#PROD_DEPS = `pkg-config --cflags --libs libcurl openssl fuse3 jsoncpp`
#TEST_DEPS = `pkg-config --cflags --libs cppunit`
include build/conanbuildinfo.mak
CPPFLAGS += $(addprefix -I, $(CONAN_INCLUDE_DIRS))
LDFLAGS += $(addprefix -L, $(CONAN_LIB_DIRS))
LDLIBS += $(addprefix -l, $(CONAN_LIBS))

 
.PHONY: YdClient;
.PHONY: FileUtils;
.PHONY: Configuration
.PHONY: Fuse
.PHONY: FileSystem
.DEFAULT_GOAL := all

FileUtils:
	$(MAKE) -C FileUtils

Configuration:
	$(MAKE) -C Configuration
	
FileSystem:
	$(MAKE) -C FileSystem

Configuration:
	$(MAKE) -C Configuration

YdClient:
	$(MAKE) -C YdClient

Fuse: Configuration FileSystem
	$(MAKE) -C Fuse
	
mkfs.o: Fuse FileUtils Configuration FileSystem
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) FileSystem/Directory.o  FileUtils/FileUtils.o  FileUtils/BlockMapper.o \
	Fuse/FuseCLIHandler.o Fuse/FuseOperationsSupport.o -c src/mkfs.cpp \
	-I./FileSystem/h -I./Fuse/h -I./YdClient/h -I./Configuration/h  -I./FileUtils/h
	
mount.o: Fuse FileUtils Configuration FileSystem
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) FileSystem/Directory.o  FileUtils/FileUtils.o  FileUtils/BlockMapper.o \
	Fuse/FuseCLIHandler.o Fuse/FuseOperationsSupport.o -c src/mount.cpp \
	-I./FileSystem/h -I./Fuse/h  -I./Configuration/h  -I./FileUtils/h

mkfs: Configuration Fuse FileUtils YdClient FileSystem mkfs.o
	$(CXX)  Fuse/FuseCLIHandler.o Fuse/FuseOperationsSupport.o FileSystem/Directory.o FileUtils/CryptoLayer.o \
	FileUtils/BlockMapper.o FileUtils/CryptoBlockData.o FileUtils/InodeManager.o FileUtils/FsBlock.o FileUtils/FileUtils.o \
	YdClient/YandexAuthenticator.o YdClient/YandexDiskClient.o FileUtils/PlainFs.o \
	YdClient/Auth.o Configuration/ConfigManager.o Configuration/Config.o mkfs.o $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) -o mkfs -I./FileSystem/h -I./YdClient/h -I./FileUtils/h -I./Fuse/h 

mount: Configuration Fuse FileUtils FileSystem mount.o
	$(CXX) $(CFLAGS) Fuse/*.o FileSystem/Directory.o  \
	FileUtils/*.o  \
	Configuration/ConfigManager.o Configuration/Config.o mount.o  $(CPPFLAGS) $(LDLIBS) $(LDFLAGS) -o mount -I./FileSystem/h  -I./FileUtils/h -I./Fuse/h 


all: mkfs mount 


clean:
	$(MAKE) -C YdClient clean
	$(MAKE) -C FileUtils clean
	$(MAKE) -C Configuration clean
	$(MAKE) -C Fuse clean
	$(MAKE) -C FileSystem clean
	rm -f a.out
	rm -Rf docs
	rm -Rf *.o
	rm -f mount
	rm -f mkfs

test:
	$(MAKE) -C YdClient test
	$(MAKE) -C FileUtils test
	$(MAKE) -C Configuration test
	$(MAKE) -C Fuse test
	$(MAKE) -C FileSystem test

docs:
	doxygen doxygen.cfg
