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
	void FsBlock_shouldContainNPlusOneChunks_ifInserted();
	void FsBlock_shouldMoveUpperChunks_ifLoweIsReszed();
	void FsBlock_shouldEditWholeChunk_afterResize();

	void fillChunkWithValue(unsigned char chunk_id, char value) {
		char* data = this->sut->read_chunk(chunk_id);
		for(int i=0;i< this->sut->chunk_size(chunk_id);i++){
			data[i] = value;
		}
	}
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
		this->FsBlock_shouldContainNPlusOneChunks_ifInserted();
		this->FsBlock_shouldMoveUpperChunks_ifLoweIsReszed();
		this->FsBlock_shouldEditWholeChunk_afterResize();
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

void FsBlockTest::FsBlock_shouldContainNPlusOneChunks_ifInserted()
{
	this->sut->clear();
	this->sut->insert_chunk(100);
	this->sut->insert_chunk(100);

	unsigned char chunks_count = this->sut->count_chunks();

	CPPUNIT_ASSERT(chunks_count == 2);
}

void FsBlockTest::FsBlock_shouldMoveUpperChunks_ifLoweIsReszed()
{
	this->sut->clear();
	this->sut->insert_chunk(100);
	this->sut->insert_chunk(100);

	this->fillChunkWithValue(1, 66);
	this->fillChunkWithValue(0, 77);

	unsigned int offset_before = this->sut->get_chunk_offset(1);

	this->sut->resize_chunk(0, 150);

	unsigned int chunk_after = this->sut->get_chunk_offset(1);
	unsigned int size_diff = chunk_after - offset_before;
	for(int i = 0; i < 100; i++)
		CPPUNIT_ASSERT(this->sut->read_chunk(1)[i] == 66);

	for(int i = 0; i < 100; i++)
		CPPUNIT_ASSERT(this->sut->read_chunk(0)[i] == 77);

	CPPUNIT_ASSERT( size_diff == 50);
}

void FsBlockTest::FsBlock_shouldEditWholeChunk_afterResize()
{
	this->sut->clear();
	this->sut->insert_chunk(100);
	this->sut->insert_chunk(100);
	this->fillChunkWithValue(0, 'a');
	this->fillChunkWithValue(1, 'b');

	this->sut->resize_chunk(0, 300);
	this->fillChunkWithValue(0, 'c');

	for(int i = 0;i < 300; i++)
		CPPUNIT_ASSERT(this->sut->read_chunk(0)[i] == 'c');

	for(int i = 0;i< 100;i++)
		CPPUNIT_ASSERT(this->sut->read_chunk(1)[i] == 'b');
}

void FsBlockTest::FsBlock_shouldReadSameDataFromChunk_afterDataIsWritten()
{
	this->sut->clear();
	char* data_to_insert = this->sut->insert_chunk(100);
	for(int i = 0; i < 100; i++) data_to_insert[i] = 37;

	const char* data_to_read = this->sut->read_chunk(0);

	for(int i = 0; i < 100; i++) CPPUNIT_ASSERT(data_to_read[i] == 37);
}


