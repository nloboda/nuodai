/*
 * BlockSemaphore.cpp
 *
 *  Created on: 01 Dec,2019
 *      Author: user
 */
#include "FileUtils/BlockSemaphore.h"

BlockSemaphore::BlockSemaphore()
{
	for(unsigned int i = 0; i < this->LOCK_GRANULARITY; i++) this->bits[i] = 0;

}
BlockSemaphore::~BlockSemaphore()
{

}

bool BlockSemaphore::get_bit(long chunk_id)
{
	unsigned long bit_offset = (chunk_id & 0x3ff);
	int bucket_id = 0;
	while(bit_offset >= 64)
	{
		bit_offset = bit_offset >> 8;
		bucket_id++;
	}
	unsigned long bucket = this->bits[bucket_id];
	return (bucket >> bit_offset) & 0x1;
}

void BlockSemaphore::set_bit(long chunk_id, bool bit)
{
	unsigned long bit_offset = (chunk_id & 0x3ff);
	int bucket_id = 0;
	while(bit_offset >= 64)
	{
		bit_offset = bit_offset >> 8;
		bucket_id++;
	}
	unsigned long val = bit;
	if(bit)
	{
		this->bits[bucket_id] |= val << bit_offset;
	}else{
		this->bits[bucket_id] &= val << bit_offset;
	}
}


bool BlockSemaphore::acquire(long chunk_id, Access access)
{
	/*
	 * TODO: use access. the implementation should distinguish
	 * between read and write. So we can have many reader
	 * or one writer
	 */
	std::lock_guard<std::mutex> g(this->mutex);
	bool locked = this->get_bit(chunk_id);
	if(locked) return false;
	this->set_bit(chunk_id, true);
	return true;

}

bool BlockSemaphore::release(long chunk_id, Access access)
{
	/*
	 * TODO: use access. the implementation should distinguish
	 * between read and write. So we can have many reader
	 * or one writer
	 */
	/*
	 * TODO: in which cases release may fail?
	 * should return type be changed to void
	 */
	std::lock_guard<std::mutex> g(this->mutex);
	this->set_bit(chunk_id, false);
	return true;
}


