/*
 * Directory.cpp
 *
 *  Created on: 24 Dec,2019
 *      Author: user
 */

#include <FileSystem/Directory.h>
#include <memory>
#include <stdexcept>
#include <cstring>
#include <vector>
#include <bits/stdc++.h>



static inline unsigned long readlong(const unsigned char* data, const unsigned int offset) noexcept
{
	unsigned long a = data[offset];
	unsigned long b = data[offset + 1];
	unsigned long c = data[offset + 2];
	unsigned long d = data[offset + 3];

	unsigned long e = data[offset + 4];
	unsigned long f = data[offset + 5];
	unsigned long g = data[offset + 6];
	unsigned long h = data[offset + 7];

	return a << 56 | b << 48 | c << 40 | d << 32 | e << 24 | f << 16 | g << 8 | h;
}

static inline void write_long(unsigned char* data, unsigned int offset, unsigned long value) noexcept
{
	data[offset] = static_cast<unsigned char>((value & 0xff00000000000000) >> 56);
	data[offset + 1] = static_cast<unsigned char>((value & 0x00ff000000000000) >> 48);
	data[offset + 2] = static_cast<unsigned char>((value & 0x0000ff0000000000) >> 40);
	data[offset + 3] = static_cast<unsigned char>((value & 0x000000ff00000000) >> 32);
	data[offset + 4] = static_cast<unsigned char>((value & 0x00000000ff000000) >> 24);
	data[offset + 5] = static_cast<unsigned char>((value & 0x0000000000ff0000) >> 16);
	data[offset + 6] = static_cast<unsigned char>((value & 0x000000000000ff00) >> 8);
	data[offset + 7] = static_cast<unsigned char>((value & 0x00000000000000ff));

}

Directory::Directory(): children(std::vector<Dirchild>{})
{

};
void Directory::mkdir(const char* name, 	unsigned long inode)
{
	Dirchild dc;
	std::strcpy(dc.name, name);
	dc.inode = inode;
	dc.type = filesystem::InodeType::Dir;
	this->children.push_back(dc);

}

void Directory::unlink(const char* name)
{
	auto name_matches = [&name](Dirchild &dc) {
		return std::strcmp(name, dc.name) == 0;
	};

	auto iterator = std::remove_if(children.begin(), children.end(), name_matches);
	children.erase(iterator, children.end());
}

unsigned int Directory::count_children()
{
	this->children.shrink_to_fit();
	return this->children.size();
}

unsigned long Directory::get_inode_by_name(const char* name)
{
	for(int i=0; i < this->children.size();i++)
		if(std::strcmp(name, children[i].name) == 0)
			return children[i].inode;
	return 0;
}

unsigned int Directory::size()
{
	return this->children.size() * sizeof(Dirchild);
}

const Dirchild Directory::get_child(unsigned int index)
{
	return this->children[index];
}

Directory::~Directory() {

}
/**
 * directory format:
 * [char: magic byte] [char: size]
 * for i<size:
 * [char{256}: name] [64 bits long: inode][char: inode type]
 */
std::unique_ptr<Directory> directory::read_directory(const unsigned char* data)
{
	std::unique_ptr<Directory> dir = std::make_unique<Directory>();
	const unsigned char size = data[1];

	for(int i = 0; i < size; i++)
	{
		int offset = 2 + i*(256 + 64 + 1);
		unsigned long inode = readlong(data, offset + 256);
		unsigned char inode_type = (data + offset + 256 + 64)[0];
		if(static_cast<unsigned char>(filesystem::InodeType::Dir)==inode_type)
		{
			dir->mkdir(reinterpret_cast<const char*>(data + offset), inode);
		}else{
			throw std::runtime_error("not supported");
		}
	}
	return std::move(dir);
}

static inline void write_dir_child(Dirchild dc, unsigned char* out)
{
	std::memcpy(out, dc.name, 256);
	write_long(out, 256, dc.inode);
	out[256 + 64] = static_cast<unsigned char>(dc.type);
}

void directory::write_directory(Directory* dir, unsigned char* out)
{
	out[0] = 0;
	out[1] = static_cast<unsigned char>(dir->count_children());
	for(unsigned int i=0;i< dir->count_children();i++)
	{
		write_dir_child(dir->get_child(i), out + 2 + i*(256+64+1));
	}
}
