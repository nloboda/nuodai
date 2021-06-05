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

namespace inode_manager{
	thread_local char *buffer = new char[FsConstants::BLOCK_SIZE];

}

InodeManager::InodeManager(BlockMapper* block_mapper, CryptoLayer* crypto_layer):
	block_mapper(block_mapper),
	crypto_layer(crypto_layer)
{
	//intentionally left blank
}

void inline InodeManager::load(const unsigned long inode)
{
	unsigned char* hash = new unsigned char[32];
	unsigned char* iv = new unsigned char[16];

	block_mapper->find_mapping(inode, &hash, &iv);
	crypto_layer->read_decrypted(
			reinterpret_cast<char *>(hash),
			inode_manager::buffer,
			reinterpret_cast<char *>(iv));
	delete hash;
	delete iv;
}

const char* InodeManager::read(const unsigned long inode)
{
	this->load(inode);

	FsBlock block(inode_manager::buffer);

	return block.read_chunk(inode & 0xff);
}

InodeManager::~InodeManager()
{

}

