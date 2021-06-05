/*
 * Directory.h
 *
 *  Created on: 23 Dec,2019
 *      Author: user
 */

#ifndef FILESYSTEM_H_FILESYSTEM_DIRECTORY_H_
#define FILESYSTEM_H_FILESYSTEM_DIRECTORY_H_

#include <vector>
#include <memory>
#include "FileSystem/InodeTypes.h"

struct Dirchild {
	char name[256];
	filesystem::InodeType type;
	unsigned long inode;
};

namespace dirchild
{
	unsigned int dirchild_fs_size = 321;
	unsigned int filename_buffer_len = 256;
}

class Directory
{
public:
	Directory();
	~Directory();
	void mkdir(const char* name, unsigned long inode);
	void unlink(const char* name);
	unsigned int count_children();
	unsigned long get_inode_by_name(const char* name);
	unsigned int size();
	const Dirchild get_child(unsigned int index);
private:
	std::vector<Dirchild> children;
};


namespace directory
{
/**
 * directory format:
 * [char: magic byte] [char: size]
 * for i<size:
 * [char{256}: name] [64 bits long: inode]w
 */
	std::unique_ptr<Directory> read_directory(const unsigned char* data);
	void write_directory(Directory* dir, unsigned char* out);

}



#endif /* FILESYSTEM_H_FILESYSTEM_DIRECTORY_H_ */
