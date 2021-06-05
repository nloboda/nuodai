CXX = g++
CFLAGS = -g -Wall 
PROD_DEPS = `pkg-config --cflags --libs libcurl openssl fuse3 jsoncpp`
TEST_DEPS = `pkg-config --cflags --libs cppunit`
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

Main.o: Fuse FileUtils Configuration FileSystem
	$(CXX) $(CFLAGS) $(PROD_DEPS) FileSystem/Directory.o  FileUtils/FileUtils.o  FileUtils/BlockMapper.o \
	Fuse/FuseCLIHandler.o Fuse/FuseOperationsSupport.o YdClient/YandexAuthenticator.o -c src/Main.cpp \
	-I./FileSystem/h -I./Fuse/h -I./YdClient/h -I./Configuration/h  -I./FileUtils/h
	
mkfs.o: Fuse FileUtils Configuration FileSystem
	$(CXX) $(CFLAGS) $(PROD_DEPS) FileSystem/Directory.o  FileUtils/FileUtils.o  FileUtils/BlockMapper.o \
	Fuse/FuseCLIHandler.o Fuse/FuseOperationsSupport.o -c src/mkfs.cpp \
	-I./FileSystem/h -I./Fuse/h -I./YdClient/h -I./Configuration/h  -I./FileUtils/h
	
mount.o: Fuse FileUtils Configuration FileSystem
	$(CXX) $(CFLAGS) $(PROD_DEPS) FileSystem/Directory.o  FileUtils/FileUtils.o  FileUtils/BlockMapper.o \
	Fuse/FuseCLIHandler.o Fuse/FuseOperationsSupport.o -c src/mount.cpp \
	-I./FileSystem/h -I./Fuse/h  -I./Configuration/h  -I./FileUtils/h

mkfs: Configuration Fuse FileUtils YdClient FileSystem mkfs.o
	$(CXX) $(PROD_DEPS) Fuse/FuseCLIHandler.o Fuse/FuseOperationsSupport.o FileSystem/Directory.o FileUtils/CryptoLayer.o \
	FileUtils/BlockMapper.o FileUtils/CryptoBlockData.o FileUtils/InodeManager.o FileUtils/FsBlock.o FileUtils/FileUtils.o \
	YdClient/YandexAuthenticator.o YdClient/YandexDiskClient.o FileUtils/PlainFs.o \
	YdClient/Auth.o Configuration/ConfigManager.o Configuration/Config.o mkfs.o -o mkfs -I./FileSystem/h -I./YdClient/h -I./FileUtils/h -I./Fuse/h 

mount: Configuration Fuse FileUtils FileSystem mount.o
	$(CXX) $(CFLAGS) $(PROD_DEPS) Fuse/*.o FileSystem/Directory.o  \
	FileUtils/*.o  \
	Configuration/ConfigManager.o Configuration/Config.o mount.o -o mount -I./FileSystem/h  -I./FileUtils/h -I./Fuse/h 


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

test:
	$(MAKE) -C YdClient test
	$(MAKE) -C FileUtils test
	$(MAKE) -C Configuration test
	$(MAKE) -C Fuse test
	$(MAKE) -C FileSystem test

docs:
	doxygen doxygen.cfg
