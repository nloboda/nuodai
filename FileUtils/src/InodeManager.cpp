/*
 * InodeManager.cpp
 *
 *  Created on: 01 Dec,2019
 *      Author: user
 */


#include "FileUtils/InodeManager.h"
#include "FileUtils/FsConstants.h"
#include "FileUtils/FsBlock.h"
#include <memory>
#include <stdexcept>

namespace inode_manager{
	thread_local char *buffer = new char[FsConstants::BLOCK_SIZE];
	thread_local unsigned long current_inode = 0L;

}

InodeManager::InodeManager(BlockMapper* block_mapper, CryptoLayer* crypto_layer):
	block_mapper(block_mapper),
	crypto_layer(crypto_layer)
{
	//intentionally left blank
}

void inline InodeManager::load(const unsigned long inode)
{
	if(inode_manager::current_inode == inode) return;

	unsigned char* hash = new unsigned char[32];
	unsigned char* iv = new unsigned char[16];

	block_mapper->find_mapping(inode, &hash, &iv);
	crypto_layer->read_decrypted(
			reinterpret_cast<char *>(hash),
			inode_manager::buffer,
			reinterpret_cast<char *>(iv));
	delete hash;
	delete iv;
	inode_manager::current_inode = inode;
}

char* InodeManager::read(const unsigned long inode)
{
	this->load(inode);

	FsBlock block(inode_manager::buffer);

	return block.read_chunk(inode & 0xff);
}

unsigned long InodeManager::suballocate(unsigned long int inode, unsigned int size)
{
	this->load(inode);
	FsBlock block(inode_manager::buffer);

	if(block.free_bytes() >= size)
	{
		return ((inode >> 8) << 8) | block.insert_chunk(size);
	} else {
		return 0L;
	}
}

unsigned long InodeManager::allocate(unsigned int size)
{
	throw std::runtime_error("Not implemented");
}

bool InodeManager::resize(unsigned long inode, unsigned int size) {
	this->load(inode);
	FsBlock block(inode_manager::buffer);
	if(block.free_bytes() < size) return false;
	block.resize_chunk(static_cast<unsigned char>(inode & 0xff), size);
	return true;
}

InodeManager::~InodeManager()
{

}

