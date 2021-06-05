/*
 * InodeManager.h
 *
 *  Created on: 01 Dec,2019
 *      Author: user
 */

#ifndef FILEUTILS_H_FILEUTILS_INODEMANAGER_H_
#define FILEUTILS_H_FILEUTILS_INODEMANAGER_H_

#include "FileUtils/BlockMapper.h"
#include "FileUtils/CryptoLayer.h"
#include <memory>

class InodeManager
{
public:
	InodeManager(BlockMapper* block_mapper, CryptoLayer* crypto_layer, char* iv);
	~InodeManager();

	char* read(unsigned long inode);

	void flush(unsigned long inode);

	/**
	 * will suballocate a chunk in block. return chunk size or zero if suballocation failed
	 * @param inode inode in which we want to suballocate
	 * @param size suballocation size in bytes
	 * @return inode of suballocation
	 */
	unsigned long suballocate(unsigned long inode, unsigned int size);

	unsigned long allocate(unsigned int size);

	/**
	 *	attempt to resize block
	 *	@param inode inote to be resized
	 *	@param size new size
	 *	@return if resize succeed
	 */
	bool resize(unsigned long inode, unsigned int size);

	void load(char* special_block);

	std::unique_ptr<char> save();

	void new_fat();
	void reset();

private:
	BlockMapper* block_mapper;
	CryptoLayer* crypto_layer;
	std::unique_ptr<char> special_block_iv;

	BlockState load(unsigned long inode);
	void set_block_state(unsigned long inode);

	std::unique_ptr<char> make_block(const char* iv);
};

#endif /* FILEUTILS_H_FILEUTILS_INODEMANAGER_H_ */
