/*
 * Directory.h
 *
 *  Created on: 22 Oct,2019
 *      Author: user
 */

#ifndef FILEUTILS_H_FILEUTILS_DIRECTORY_H_
#define FILEUTILS_H_FILEUTILS_DIRECTORY_H_


#include "FileSystem/AllocationTable.h"
/**
 * This is Directory. it contains a list of entities (files or directories)
 * TODO: size for now is fixed. we need to ad support for dir blob: additional blobs with data
 * if directory size is greater then chunk
 *
 * Directory is entity. And it supposed to be stored in allocation table.
 * Here are the bytes:
 * 0: number of children
 * 1 - 16165: 61 slot for children (named references)
 * 16165 - 16173 - unsigned long points to continuation (not implemented!!! not implemented!!!)
 *
 *
 */
class Directory
{
public:
	Directory(const char* data);
	virtual ~Directory();
	const unsigned char count_children();
	const char* const child_at(unsigned char position);

	const char* get_name(unsigned char i);
	const EntityType get_type(unsigned char i);
	const unsigned long inode_id(unsigned char i);

private:
	const char* data;
	//size is name(256 bytes) + entity type (byte) + inode_id (64 bits aka 8 bytes)
	static unsigned const int STRUCTURE_SIZE = 265;
};



#endif /* FILEUTILS_H_FILEUTILS_DIRECTORY_H_ */
