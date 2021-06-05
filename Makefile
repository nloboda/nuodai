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

YdClient:
	$(MAKE) -C YdClient

FileUtils:
	$(MAKE) -C FileUtils

Configuration:
	$(MAKE) -C Configuration

Fuse: 
	$(MAKE) -C Fuse
	
FileSystem:
	$(MAKE) -C FileSystem

Main.o: Fuse FileUtils Configuration FileSystem
	$(CXX) $(CFLAGS) $(PROD_DEPS) FileSystem/Directory.o  Fuse/FuseAdapter.o FileUtils/FileUtils.o  FileUtils/BlockMapper.o -I./Configuration/h YdClient/YandexAuthenticator.o -c src/Main.cpp -I./FileSystem/h -I./Fuse/h -I./YdClient/h -I./FileUtils/h

all: Fuse FileUtils YdClient FileSystem Main.o
	$(CXX) $(PROD_DEPS) FileSystem/Directory.o Fuse/FuseAdapter.o FileUtils/CryptoLayer.o FileUtils/BlockMapper.o FileUtils/CryptoBlockData.o FileUtils/InodeManager.o Configuration/Configuration.o FileUtils/FsBlock.o FileUtils/FileUtils.o YdClient/YandexAuthenticator.o YdClient/YandexDiskClient.o FileUtils/PlainFs.o YdClient/Auth.o Main.o -o a.out -I./FileSystem/h -I./YdClient/h -I./FileUtils/h -I./Fuse/h 

clean:
	find . -name \*.o -exec rm {} \;
	rm -Rf out/*
	rm -f a.out
	rm -Rf docs
test:
	$(MAKE) -C YdClient test
	$(MAKE) -C FileUtils test
	$(MAKE) -C Configuration test
	$(MAKE) -C Fuse test
	$(MAKE) -C FileSystem test

docs:
	doxygen doxygen.cfg
