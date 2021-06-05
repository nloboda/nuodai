/*
 * DirectoryTest.cpp
 *
 *  Created on: 24 Dec,2019
 *      Author: user
 */


#include "FileSystem/Directory.h"
#include <cppunit/TestCase.h>
#include <iostream>
#include <cstring>
#include <memory>


class DirectoryTest: public CppUnit::TestCase
{
private:
	Directory* sut;
	void directory_shouldIncreaseNumberOfChildren_whenChildIsAdded();
	void directory_shouldReturnChildByName_ifExists();
	void directory_shouldNotReturnChildByName_ifItHasBeenRemoved();
	void directory_shouldReturnChildByIndex_ifExists();
	void directory_shouldBeReadFromBlob_afterBeingWritten();
public:
	void setUp()
	{
		this->sut = new Directory();
	}

	void tearDown()
	{
		delete this->sut;
	}

	void runTest()
	{
		this->directory_shouldIncreaseNumberOfChildren_whenChildIsAdded();
		this->directory_shouldReturnChildByName_ifExists();
		this->directory_shouldNotReturnChildByName_ifItHasBeenRemoved();
		this->directory_shouldReturnChildByIndex_ifExists();
		this->directory_shouldBeReadFromBlob_afterBeingWritten();
	}
};

void DirectoryTest::directory_shouldIncreaseNumberOfChildren_whenChildIsAdded()
{
	std::unique_ptr<Directory> sut = std::make_unique<Directory>();

	sut->mkdir("whatever", 2);

	CPPUNIT_ASSERT(sut->count_children() == 1);
}

void DirectoryTest::directory_shouldReturnChildByName_ifExists()
{
	std::unique_ptr<Directory> sut = std::make_unique<Directory>();

	sut->mkdir("whatever", 2);

	CPPUNIT_ASSERT(sut->get_inode_by_name((char*)std::string("whatever").c_str()) == 2);
}
void DirectoryTest::directory_shouldNotReturnChildByName_ifItHasBeenRemoved()
{
	std::unique_ptr<Directory> sut = std::make_unique<Directory>();

	sut->mkdir("whatever", 2);
	sut->mkdir("I masturbate to comatose girls", 3);
	sut->mkdir("I will defeat you in the name of the Moon.", 4);
	sut->unlink("I masturbate to comatose girls");

	CPPUNIT_ASSERT(sut->get_inode_by_name("I masturbate to comatose girls") == 0);
	CPPUNIT_ASSERT(sut->count_children() == 2);
}

void DirectoryTest::directory_shouldReturnChildByIndex_ifExists()
{
	std::unique_ptr<Directory> sut = std::make_unique<Directory>();

	sut->mkdir("whatever", 2);
	const Dirchild dc = sut->get_child(0);
	sut->mkdir("whatever1", 3);

	CPPUNIT_ASSERT(std::strcmp(dc.name, "whatever") == 0);
}

void DirectoryTest::directory_shouldBeReadFromBlob_afterBeingWritten()
{
	std::unique_ptr<Directory> sut = std::make_unique<Directory>();
	const char* dir1 = "I am Sailor Moon, champion of justice";
	const char* dir2 = "Girls have to be strong to protect the men they love";
	sut->mkdir(dir1, 2);
	sut->mkdir(dir2, 3);
	std::unique_ptr<unsigned char> data = std::unique_ptr<unsigned char>(new unsigned char[2048]);

	directory::write_directory(sut.get(), data.get());
	std::unique_ptr<Directory> copy = directory::read_directory(data.get());

	CPPUNIT_ASSERT(std::strcmp(copy->get_child(0).name, dir1) == 0);
	CPPUNIT_ASSERT(std::strcmp(copy->get_child(1).name, dir2) == 0);
}
