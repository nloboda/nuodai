#include "FileUtils/FsBlock.h"
#include "FileUtils/PlainFs.h"
#include "FileUtils/FsConstants.h"
#include <stdexcept>
#include <iostream>

/**
    data structure is following:
    [number of chunks, 1 char] (number of chunks) of  ints for each chunk offset
    block size 1 int
    then there are chunks with appropriate sizes
*/
FsBlock::FsBlock(char* data)
{
	this->data = data;
};

FsBlock::~FsBlock()
{

};

void FsBlock::clear()
{
	for(unsigned int i = 0; i < FsConstants::BLOCK_SIZE; i++)
		this->data[i] = 0;
	//TODO: should implement
	this->set_chunk_offset(0, 1 + 256 * sizeof(int));
}

unsigned char FsBlock::count_chunks() noexcept
{
	return reinterpret_cast<unsigned char*>(this->data)[0];
};

unsigned int FsBlock::chunk_size(unsigned char n) noexcept
{
	return this->get_chunk_offset(n + 1) - this->get_chunk_offset(n);
};

static inline unsigned int readint(unsigned char* data, unsigned int offset) noexcept
{
	unsigned int a = data[offset];
	unsigned int b = data[offset + 1];
	unsigned int c = data[offset + 2];
	unsigned int d = data[offset + 3];
	return a << 24|b << 16|c << 8| d;
}

static inline void write_int(unsigned char* data, unsigned int offset, unsigned int value) noexcept
{
	data[offset] = static_cast<unsigned char>((value & 0xff000000) >> 24);
	data[offset + 1] = static_cast<unsigned char>((value & 0x00ff0000) >> 16);
	data[offset + 2] = static_cast<unsigned char>((value & 0x0000ff00) >> 8);
	data[offset + 3] = static_cast<unsigned char>(value & 0x000000ff);
}

unsigned int FsBlock::get_chunk_offset(unsigned char n) noexcept
{
	unsigned int i = readint(reinterpret_cast<unsigned char *>(this->data), 1 + sizeof(int) * n);
	return i;
}

void FsBlock::set_chunk_offset(unsigned char n, unsigned int value) noexcept
{
	unsigned int offset = 1 + sizeof(int) * n;
	write_int(reinterpret_cast<unsigned char*>(this->data), offset, value);
}

char* FsBlock::read_chunk(unsigned char n)
{
	unsigned int offset = this->get_chunk_offset(n);
	if(offset > FsConstants::BLOCK_SIZE) throw std::runtime_error("outside of chunk pointer");
	return this->data + this->header_size() + offset;
};

void FsBlock::remove_chunk(unsigned char n)
{
	unsigned char number_of_chunks = count_chunks();
	bool is_last_chunk = (n == number_of_chunks - 1);

	if(is_last_chunk)
	{
		this->data[0] = static_cast<unsigned char>(number_of_chunks - 1);
	}
	else
	{
		unsigned int* chunk_position = reinterpret_cast<unsigned int *>(this->data + 1 + sizeof(int) * n);
		unsigned int* next_chunk_position = reinterpret_cast<unsigned int *>(this->data + 1 + sizeof(int) * (n + 1));
		unsigned int diff = (*next_chunk_position) - (*chunk_position);

		for(unsigned int i = (*next_chunk_position); i < FsConstants::BLOCK_SIZE; i++)
		{
			this->data[i - diff] = this->data[i];//todo: come up with a better way
		}

		for(int i=n; i < number_of_chunks + 1; i++)
		{
			unsigned int* ptr = reinterpret_cast<unsigned int *>(this->data + 1 + sizeof(int) * n);
			*ptr = *ptr + chunk_size(n);
		}
	}
};

int FsBlock::get_first_skipped_chunk() noexcept
{
	for(int i = 0; i < this->count_chunks(); i++)
		if(this->chunk_size(i) == 0) return i;
	return -1;
};

unsigned int FsBlock::free_bytes() noexcept
{
	unsigned char chunks = this->count_chunks();
	return FsConstants::BLOCK_SIZE - this->chunk_size(chunks);
}

unsigned char FsBlock::insert_chunk(unsigned int chunk_size)
{
	if(count_chunks() >= 0xff - 0x1) throw std::invalid_argument("all chunks are occupied");
	int first_skipped_chunk = this->get_first_skipped_chunk();
	if (first_skipped_chunk >= 0) throw std::runtime_error("not implemented");

	unsigned char next_chunk = this->count_chunks();
	unsigned int next_chunk_offset = this->get_chunk_offset(next_chunk);
	unsigned int chunk_end_offset = next_chunk_offset + chunk_size;

	if(chunk_end_offset > FsConstants::BLOCK_SIZE) throw std::invalid_argument("block exhausted");

	this->set_chunk_offset(next_chunk + 1, chunk_end_offset);
	this->data[0]++;

	return next_chunk;
};

void FsBlock::resize_chunk(unsigned char n, unsigned int new_size) noexcept
{
	unsigned int old_size = this->chunk_size(n);
	int size_diff = new_size - old_size;
	unsigned int next_chunk_offset = this->get_chunk_offset(n + 1);

	//TODO: we should use memcpy here instead if setting bytes manually
	if(size_diff > 0)
	{
		for(unsigned long i = FsConstants::BLOCK_SIZE - 1 ;i >= next_chunk_offset + size_diff; i--)
			this->data[i] = this->data[i - size_diff];
	} else {
		for(unsigned long i = next_chunk_offset + size_diff; i < FsConstants::BLOCK_SIZE + size_diff;i++)
				this->data[i] = this->data[i - size_diff];
	}

	for(int i = n + 1;i < this->count_chunks(); i++)
		this->set_chunk_offset(i, this->get_chunk_offset(i) + size_diff);
}

unsigned int FsBlock::header_size() noexcept
{
	unsigned char all_chunks = this->count_chunks();
	return 1 + (all_chunks + 1)*sizeof(int);
}
