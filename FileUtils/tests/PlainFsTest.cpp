/*
 * PlainFsTest.cpp
 *
 *  Created on: 16 Nov,2019
 *      Author: user
 */

#include <cppunit/TestCase.h>
#include "FileUtils/PlainFs.h"
#include <iostream>


class PlainFsTest: public CppUnit::TestCase
{
private:
	PlainFs* sut;
	char* generate_buf()
	{
		char* c = new char[32 * 1024];
		for(int i=0; i < 32 * 1024; i++) c[i] = i % 256;
		return c;
	}
	void plainFs_shouldReturnProperSha256Hash_whenBlockIsSaved();
	void plainFs_shouldReadDataThatWasWritten_whenBlockIsTheSame();
public:
	void setUp()
	{
		//TODO: this is just retadred! should be replaced with somethng meaningful
		this->sut = new PlainFs("/home/wer/yad_tmp/");
	}

	void tearDown()
	{
		delete sut;
	}



	void runTest()
	{
		this->plainFs_shouldReturnProperSha256Hash_whenBlockIsSaved();
		this->plainFs_shouldReadDataThatWasWritten_whenBlockIsTheSame();
	}
};


void PlainFsTest::plainFs_shouldReturnProperSha256Hash_whenBlockIsSaved()
{
	//arrange
	const char* buffer_to_be_written = generate_buf();
	char actual_hash[32]; // 256 bits == 32 bytes
	const char expected_hash[]{
			static_cast<char>(0xe1), static_cast<char>(0x13),
			static_cast<char>(0x60), static_cast<char>(0x25),
			static_cast<char>(0x1d), static_cast<char>(0x11),
			static_cast<char>(0x73), static_cast<char>(0x65),
			static_cast<char>(0x0c), static_cast<char>(0xdc),
			static_cast<char>(0xd2), static_cast<char>(0x0f),
			static_cast<char>(0x11), static_cast<char>(0x1d),
			static_cast<char>(0x8f), static_cast<char>(0x1c),
			static_cast<char>(0xa2), static_cast<char>(0xe4),
			static_cast<char>(0x12), static_cast<char>(0xf5),
			static_cast<char>(0x72), static_cast<char>(0xe8),
			static_cast<char>(0xb3), static_cast<char>(0x6a),
			static_cast<char>(0x4d), static_cast<char>(0xc0),
			static_cast<char>(0x67), static_cast<char>(0x12),
			static_cast<char>(0x1c), static_cast<char>(0x17),
			static_cast<char>(0x99), static_cast<char>(0xb8)
	};
	//act
	sut->write_plain(buffer_to_be_written, actual_hash);
	//verify
	for(int i=0; i<32; i++) CPPUNIT_ASSERT(expected_hash[i] == actual_hash[i]);
}


void PlainFsTest::plainFs_shouldReadDataThatWasWritten_whenBlockIsTheSame()
{
	//arrange
	const char* in_buffer = generate_buf();
	char* out_buffer = new char[32 * 1024];
	char block[32];
	sut->write_plain(in_buffer, block);
	//act
	sut->read_plain(block, out_buffer);
	//verify
	for(int i=0; i < 32 * 1024; i++) CPPUNIT_ASSERT(in_buffer[i] == out_buffer[i]);
	delete out_buffer;
	delete in_buffer;
}
