/*
 * CryptoLayerTest.cpp
 *
 *  Created on: 17 Nov,2019
 *      Author: user
 */




#include <cppunit/TestCase.h>
#include "FileUtils/CryptoLayer.h"
#include "FileUtils/PlainFs.h"
#include <iostream>


class CryptoLayerTest:  public CppUnit::TestCase
{
private:
	CryptoLayer* sut;
	PlainFs* plain_fs;
	char* generate_buf()
	{
		char* c = new char[32 * 1024];
		for(int i=0; i < 32 * 1024; i++) c[i] = (i * 13) % 256;
		return c;
	}
	void cryptoLayer_shouldReturnSameData_whenDecrypted();

public:
	void setUp()
	{
		const unsigned char* key = reinterpret_cast<const unsigned char*>("any_randomKey goesHere$%@#$%@^%@");

		this->plain_fs = new PlainFs("/home/user/yad_tmp/");
		this->sut = new CryptoLayer(plain_fs, key);
	}

	void tearDown()
	{
		delete this->sut;
		delete this->plain_fs;
	}

	void runTest()
	{
		this->cryptoLayer_shouldReturnSameData_whenDecrypted();
	}
};


void CryptoLayerTest::cryptoLayer_shouldReturnSameData_whenDecrypted()
{
	//arrange
	const char* buffer_to_be_written = generate_buf();
	const char* iv = "01234567890123456789";
	char block[32];
	this->sut->write_encrypted(buffer_to_be_written, block, iv);
	//act
	char* buffer_to_be_read = new char[32 * 1024];
	this->sut->read_decrypted(block, buffer_to_be_read, iv);
	//verify
	for(int i = 0; i < 32 * 1024; i++) CPPUNIT_ASSERT(buffer_to_be_written[i] == buffer_to_be_read[i]);
}
