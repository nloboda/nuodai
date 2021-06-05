/*
 * InodeManager.cpp
 *
 *  Created on: 01 Dec,2019
 *      Author: user
 */


#include "FileUtils/InodeManager.h"
#include "FileUtils/FsConstants.h"
#include "FileUtils/FsBlock.h"
#include <stdexcept>
#include <memory>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <iostream>
#include <climits>

namespace inode_manager {
	thread_local char *buffer = new char[FsConstants::BLOCK_SIZE];
	thread_local unsigned long current_inode = ULONG_MAX;

	void rand_bytes(const unsigned int size, char* data)
	{
		RAND_bytes(reinterpret_cast<unsigned char*>(data), size);
	}
}

static constexpr unsigned long clear_suballocation_bits(const unsigned long inode) noexcept
{
	return ((inode >> 8) << 8);
}

static bool inode_loaded(const unsigned long inode) noexcept
{
	return inode_manager::current_inode == clear_suballocation_bits(inode);
}

InodeManager::InodeManager(BlockMapper* block_mapper, CryptoLayer* crypto_layer, char* iv):
	block_mapper(block_mapper),
	crypto_layer(crypto_layer),
	special_block_iv(new char[16])
{
	std::memcpy(this->special_block_iv.get(), iv, 16);
}

BlockState inline InodeManager::load(const unsigned long inode)
{
	if(inode_loaded(inode)) {
		std::cout << "Inode :" << inode << " is already loaded" << std::endl;
		return BlockState::ALLOCATED;//we're working on this block, it must be allocated
	}
	std::cout << "Loading inode :" << inode << std::endl;

	unsigned char* hash;
	unsigned char* iv;

	BlockState bs = block_mapper->find_mapping(inode, &hash, &iv);
	if(bs == BlockState::ALLOCATED)
	{
		crypto_layer->read_decrypted(
				reinterpret_cast<char *>(hash),
				inode_manager::buffer,
				reinterpret_cast<char *>(iv));
		inode_manager::current_inode = inode;
	}
	return bs;
}

char* InodeManager::read(const unsigned long inode)
{
	BlockState bs = this->load(clear_suballocation_bits(inode));
	if(bs != BlockState::ALLOCATED) throw std::runtime_error("Block not allocated");

	FsBlock block(inode_manager::buffer);

	return block.read_chunk(inode & 0xff);
}

void InodeManager::flush(const unsigned long inode)
{
	if(!inode_loaded(inode)) throw new std::runtime_error("You already loaded the next block, stupid!");
	std::cout << "Flushing inode: " << inode << std::endl;
	char* hash = new char[32];
	char* iv = new char[16];

	inode_manager::rand_bytes(16, iv);
	crypto_layer->write_encrypted(inode_manager::buffer, hash, iv);
	block_mapper->update_mapping(inode, reinterpret_cast<unsigned char*>(hash), reinterpret_cast<unsigned char*>(iv));

	delete hash;
	delete iv;
}

unsigned long InodeManager::suballocate(unsigned long int inode, unsigned int size)
{
	BlockState bs = this->load(clear_suballocation_bits(inode));

	switch(bs)
	{
		case BlockState::ALLOCATED:
		{
			std::cout << "Suballocating in allocated block" << std::endl;
			FsBlock block(inode_manager::buffer);

			if(block.free_bytes() >= size)
			{
				return ((inode >> 8) << 8) | block.insert_chunk(size);
			} else {
				return 0L;
			}
		}
		case BlockState::NOT_EXISTS:
			throw std::runtime_error("Operation not supported");
		case BlockState::FREE:
		{
			std::cout << "Allocating new block before suballocation" << std::endl;
			std::unique_ptr<char> hash =  this->make_block("0123456789abcdef");
			this->block_mapper->update_mapping(clear_suballocation_bits(inode),
					reinterpret_cast<const unsigned char*>(hash.get()),
					reinterpret_cast<const unsigned char*>("0123456789abcdef"));
			this->load(clear_suballocation_bits(inode));
			FsBlock block(inode_manager::buffer);

			if(block.free_bytes() >= size)
			{
				return ((inode >> 8) << 8) | block.insert_chunk(size);
			} else {
				return 0L;
			}
		}
		case BlockState::LOCKED:
			throw std::runtime_error("Operation Not Supported");
		default:
			throw std::runtime_error("Oh snap should not have happened");
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

void InodeManager::load(char* special_block)
{
	char* block = new char[FsConstants::BLOCK_SIZE];
	this->crypto_layer->read_decrypted(special_block, block, this->special_block_iv.get());
	this->block_mapper->append_block(block);

}
std::unique_ptr<char> InodeManager::save()
{
	const char* special_block_data = this->block_mapper->get_fat_data();
	std::unique_ptr<char> new_special_block_hash = std::unique_ptr<char>(new char[32]);
	this->crypto_layer->write_encrypted(special_block_data, new_special_block_hash.get(), this->special_block_iv.get());
	return new_special_block_hash;

}

std::unique_ptr<char> InodeManager::make_block(const char* iv)
{
	char* in = new char[FsConstants::BLOCK_SIZE];
	for(unsigned long i = 0; i < FsConstants::BLOCK_SIZE; i++) in[i] = 0;

	std::unique_ptr<char> block = std::unique_ptr<char>(new char[32]);
	this->crypto_layer->write_encrypted(in, block.get(), iv);
	return std::move(block);
}

std::unique_ptr<char> InodeManager::new_fat()
{
	std::unique_ptr<char> special_node_hash =  this->make_block(this->special_block_iv.get());
	char* block = new char[FsConstants::BLOCK_SIZE];
	this->crypto_layer->read_decrypted(special_node_hash.get(), block, this->special_block_iv.get());
	this->block_mapper->append_block(block);
	return std::unique_ptr<char>(block);
}

InodeManager::~InodeManager()
{

}

