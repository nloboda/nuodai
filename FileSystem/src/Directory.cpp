/*
 * Directory.cpp
 *
 *  Created on: 02 Nov,2019
 *      Author: user
 */


#include "FileSystem/Directory.h"

Directory::Directory(const char* data): data(data){}

Directory::~Directory()
{

}

const unsigned char Directory::count_children()
{
	return reinterpret_cast<const unsigned char*>(this->data)[0];
}

const char* const Directory::child_at(unsigned char position)
{
	return static_cast<const char* const>(1 + data + (Directory::STRUCTURE_SIZE * position));
}

const char* Directory::get_name(const unsigned char i)
{
	return static_cast<const char*>(this->child_at(i) + 1);
}
const EntityType Directory::get_type(unsigned char i)
{
	int type = this->child_at(i)[0];
	return static_cast<EntityType>(type);
}

const unsigned long Directory::inode_id(unsigned char i)
{
	return 1;
}


