/*
 * AllocationTest.cpp
 *
 *  Created on: 01 Dec,2019
 *      Author: user
 */



#include <cppunit/TestCase.h>
#include "FileUtils/Allocation.h"
#include <cstring>


class AllocationTest: public CppUnit::TestCase
{
private:
	Allocation* sut;
	void blockMapper_shouldIgnoreLast8BitsOfInode_whenMappingABlock();
public:
	void setUp()
	{
		unsigned char* data = new unsigned char[]{
				1,
		};
		this->sut = new Allocation(data, 0);
	}

	void tearDown()
	{
		delete this->sut;
	}

	void runTest()
	{
		this->blockMapper_shouldIgnoreLast8BitsOfInode_whenMappingABlock();
	}
}
