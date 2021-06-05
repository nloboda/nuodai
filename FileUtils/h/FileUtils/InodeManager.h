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

	unsigned long suballocate(unsigned long inode, unsigned int size);

	unsigned long allocate(unsigned int size);

	bool resize(unsigned long inode, unsigned int size);

	void load(char* special_block);

	std::unique_ptr<char> save();

	std::unique_ptr<char> new_fat();

private:
	BlockMapper* block_mapper;
	CryptoLayer* crypto_layer;
	std::unique_ptr<char> special_block_iv;

	BlockState load(unsigned long inode);

	std::unique_ptr<char> make_block(const char* iv);
};

#endif /* FILEUTILS_H_FILEUTILS_INODEMANAGER_H_ */
