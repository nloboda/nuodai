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

class InodeManager
{
public:
	InodeManager(BlockMapper* block_mapper, CryptoLayer* crypto_layer);
	~InodeManager();

	const char* read(unsigned long inode);

	unsigned long allocate(unsigned long inode, unsigned int size);

private:
	BlockMapper* block_mapper;
	CryptoLayer* crypto_layer;

	void load(unsigned long inode);
};

#endif /* FILEUTILS_H_FILEUTILS_INODEMANAGER_H_ */
