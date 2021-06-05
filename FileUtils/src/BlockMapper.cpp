#include "FileUtils/BlockMapper.h"
#include <vector>
#include "FileUtils/PlainFs.h"
#include <iostream>
#include <cstdint>
#include <cstring>
/*
 * I feel like i need to leave this comment here.
 *
 * There are chunks: physical files on your local filesystem.
 * there are also Inodes. they're chunks of data. Could be of
 * several types: dir, file or just chunk of data (file is a list of such chunks)
 * but this could change.
 * Inode id is 64 bits due to fuse interfaces and there are 256 inodes per chunk
 * which gives 56 bits for chunk id. that still a lot.
 */
BlockMapper::BlockMapper()
{
	//TODO: map should also contain free space in blocks and possibly number of allocations
	/*
	 * The idea behind this is fast search for free place.
	 *
	 * Also since we can't have more than 255 suballocations in block
	 *  inode with id having last byte 0x00 may mean that this block is just a piece of data
	 *  cou
	 */
	this->map = std::vector<char *>();
}

BlockMapper::~BlockMapper()
{

}


/**
 * remove last 8 bits
 */
static unsigned long get_chunk_id(const unsigned long inode_id) noexcept
{
	return inode_id / 256;
}
/**
 *
 */
static unsigned long find_chunk_number(const unsigned long inode_id) noexcept
{
	return get_chunk_id(inode_id) / Blocks::REFERENCES_IN_BLOCK;
}

static unsigned long find_chunk_in_block(const unsigned long inode_id) noexcept
{
	return (get_chunk_id(inode_id) % Blocks::REFERENCES_IN_BLOCK) * Blocks::REFERENCE_SIZE;
}

/**
 * This will populate  hash key and iv with
 * @param id id of the block
 * @param hash will set param to hash of block to search for
 * @param key an encryption key
 * @param iv initialization vector
 */
BlockState BlockMapper::find_mapping(const unsigned long inode_id,
		unsigned char** hash,
		unsigned char** iv)
{
	unsigned long chunk_number = find_chunk_number(inode_id);
	unsigned long offset_in_block = find_chunk_in_block(inode_id);

	if(this->map.size() <= chunk_number) return BlockState::NOT_EXISTS;

	char* allocation_data = this->map.at(chunk_number) + offset_in_block;

	unsigned char* block = reinterpret_cast<unsigned char*>(allocation_data);
	*hash = block;
	*iv = block + BLOCK_MAPPER_HASH_SIZE;

	BlockState state = static_cast<BlockState>((allocation_data + BLOCK_MAPPER_HASH_SIZE + BLOCK_MAPPER_IV_SIZE)[0]);
	return state;
}

/**
 * @param id id of the block
 * @param hash will set hash of block to search for
 * @param key an encryption key
 * @param iv initialization vector
 */
bool BlockMapper::update_mapping(const unsigned long inode_id,
		const unsigned char* hash,
		const unsigned char* iv)
{
	unsigned int block_number = find_chunk_number(inode_id);
	unsigned int offset_in_block = find_chunk_in_block(inode_id);

	if(this->map.size() <= block_number) return false;

	unsigned char* block = reinterpret_cast<unsigned char*>(this->map.at(block_number) + offset_in_block);
	std::memcpy(block, hash, BLOCK_MAPPER_HASH_SIZE);
	std::memcpy(block + BLOCK_MAPPER_HASH_SIZE , iv, BLOCK_MAPPER_IV_SIZE);
	reinterpret_cast<char*>(block + BLOCK_MAPPER_HASH_SIZE + BLOCK_MAPPER_IV_SIZE)[0] = static_cast<char>(BlockState::ALLOCATED);
	return true;
}

/**
 * Adds an empty block
 * @param block blocks data
 */
void BlockMapper::append_block(char* block)
{
	for(int i=0; i < Blocks::REFERENCES_IN_BLOCK; i++) {
		unsigned int offset = find_chunk_in_block(i);
		char* state_pointer = block + offset + BLOCK_MAPPER_HASH_SIZE + BLOCK_MAPPER_IV_SIZE;
		state_pointer[0] = static_cast<char>(BlockState::FREE);
	}
	this->map.push_back(block);
}

char* BlockMapper::get_block()
{
	return this->map[0];
}
