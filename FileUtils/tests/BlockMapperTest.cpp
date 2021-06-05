/*
 * BlockMapperTest.cpp
 *
 *  Created on: 08 Nov,2019
 *      Author: user
 */
#include <cppunit/TestCase.h>
#include "FileUtils/BlockMapper.h"
#include <cstring>


class BlockMapperTest: public CppUnit::TestCase
{
private:
	BlockMapper* sut;
	void blockMapper_shouldIgnoreLast8BitsOfInode_whenMappingABlock();
public:
	void setUp()
	{
		this->sut = new BlockMapper();
	}

	void tearDown()
	{
		delete this->sut;
	}

	void runTest()
	{
		this->blockMapper_shouldIgnoreLast8BitsOfInode_whenMappingABlock();
	}
};

//we don't really care what exact content is here we just want to have 256 bit of some data for key and 128 bits for iv
#define HASH_1 reinterpret_cast<const unsigned char*>("2zERDJqpP0CDPkbQz426qLF9LcwfQw4mxUSEMsCIOZ6H40vYglJRC3D6kG9Vzk0O")
#define IV_1 reinterpret_cast<const unsigned char*>("N5cS0c5T3NRifh/37XVnlVNMNOkNwyI9")

#define HASH_2 reinterpret_cast<const unsigned char*>("h9XVJiI0Xex9wZQ9hFRqxTxthYY42R4Ez3iLbXl0v1Ehrh2tY1OoiRW86SYHFtHJ")
#define IV_2 reinterpret_cast<const unsigned char*>("rdXiG0fbTr2t0XKLuSpQiZD72pVtuCFq")

void BlockMapperTest::blockMapper_shouldIgnoreLast8BitsOfInode_whenMappingABlock()
{
	//prepare
	unsigned long block_size = Blocks::REFERENCES_IN_BLOCK * Blocks::REFERENCE_SIZE;
	char* block = new char[block_size];
	std::memset(block, 0, block_size);
	this->sut->append_block(block);
	//act
	this->sut->update_mapping(0, HASH_1, IV_1);
	this->sut->update_mapping(256, HASH_2, IV_2);
	//verify

	unsigned char* hash = new unsigned char[32];
	unsigned char* iv = new unsigned char[16];

	//todo: should we do free hash and free iv at the end? Should we allocate this in the first place?
	for(unsigned long int i = 0; i < 256; i++)
	{
		this->sut->find_mapping(i, &hash, &iv);
		CPPUNIT_ASSERT(memcmp(iv, IV_1, 16) == 0);
		CPPUNIT_ASSERT(memcmp(hash, HASH_1, 32) == 0);
	}

	for(unsigned long int i = 256; i < 512; i++)
	{
		this->sut->find_mapping(i, &hash, &iv);
		CPPUNIT_ASSERT(memcmp(iv, IV_2, 16) == 0);
		CPPUNIT_ASSERT(memcmp(hash, HASH_2, 32) == 0);
	}

}

