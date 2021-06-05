/*
 * BlockMapperTest.cpp
 *
 *  Created on: 07 Nov,2019
 *      Author: user
 */


#include <cppunit/TestCase.h>
#include "FileUtils/BlockSemaphore.h"
#include <iostream>

class BlockSemaphoreTest: public CppUnit::TestCase
{
private:
	BlockSemaphore* sut;
	void BlockSemaphore_shouldDeclineLock_ifAlreadyAquired();
	void BlockSemaphore_shouldAcceptLock_ifNotAquired();
public:
	void setUp()
	{
		this->sut = new BlockSemaphore();
	}

	void tearDown()
	{
		delete sut;
	}

	void runTest()
	{
		this->BlockSemaphore_shouldDeclineLock_ifAlreadyAquired();
		this->BlockSemaphore_shouldAcceptLock_ifNotAquired();
	}
};

void BlockSemaphoreTest::BlockSemaphore_shouldDeclineLock_ifAlreadyAquired()
{
	//arrange
	this->sut->acquire(1234, Access::WRITE);
	//act
	bool lock_aquired = this->sut->acquire(1234, Access::WRITE);
	//verify
	CPPUNIT_ASSERT(lock_aquired == false);
	//cleanup
	this->sut->release(1234, Access::WRITE);
}

void BlockSemaphoreTest::BlockSemaphore_shouldAcceptLock_ifNotAquired()
{
	//act
	bool lock_aquired = this->sut->acquire(1234, Access::WRITE);
	//verify
	CPPUNIT_ASSERT(lock_aquired == true);
	//cleanup
	this->sut->release(1234, Access::WRITE);
}

