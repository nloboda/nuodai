/*
 * BlockMapperTest.cpp
 *
 *  Created on: 07 Nov,2019
 *      Author: user
 */


#include <cppunit/TestCase.h>
#include "FileUtils/FsBlock.h"
#include <iostream>

class FsBlockTest: public CppUnit::TestCase
{
private:
	FsBlock* sut;
	char* data;
	void FsBlock_shouldContainZeroChunks_afterClear();
	void FsBlock_shouldContainOneChunk_ifInserted();
	void FsBlock_shouldReadSameDataFromChunk_afterDataIsWritten();
public:
	void setUp()
	{
		this->data = new char[32 * 1024];
		this->sut = new FsBlock(this->data);
	}

	void tearDown()
	{
		delete sut;
		delete data;
	}

	void runTest()
	{
		this->FsBlock_shouldContainZeroChunks_afterClear();
		this->FsBlock_shouldContainOneChunk_ifInserted();
		this->FsBlock_shouldReadSameDataFromChunk_afterDataIsWritten();
	}
};


void FsBlockTest::FsBlock_shouldContainZeroChunks_afterClear()
{
	this->sut->clear();

	unsigned char chunks_count = this->sut->count_chunks();

	CPPUNIT_ASSERT(chunks_count == 0);
}

void FsBlockTest::FsBlock_shouldContainOneChunk_ifInserted()
{
	this->sut->clear();
	this->sut->insert_chunk(100);

	unsigned char chunks_count = this->sut->count_chunks();

	CPPUNIT_ASSERT(chunks_count == 1);
}

void FsBlockTest::FsBlock_shouldReadSameDataFromChunk_afterDataIsWritten()
{
	this->sut->clear();
	char* data_to_insert = this->sut->insert_chunk(100);
	for(int i = 0; i < 100; i++) data_to_insert[i] = 37;

	const char* data_to_read = this->sut->read_chunk(0);

	for(int i = 0; i < 100; i++) CPPUNIT_ASSERT(data_to_read[i] == 37);

}


