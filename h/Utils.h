/*
 * Utils.h
 *
 *  Created on: 31 Dec,2019
 *      Author: user
 */

#ifndef H_UTILS_H_
#define H_UTILS_H_


/**
 * remove last 8 bits
 */
static unsigned long get_chunk_id(const unsigned long inode_id) noexcept
{
	return inode_id / 256;
}

static inline unsigned int read_int(unsigned char *data, unsigned int offset) {
	return static_cast<unsigned int>(data[offset]) << 24
			| static_cast<unsigned int>(data[offset + 1]) << 16
			| static_cast<unsigned int>(data[offset + 2]) << 8
			| static_cast<unsigned int>(data[offset + 3]);
}

static inline unsigned int readint(unsigned char* data, unsigned int offset) noexcept
{
	unsigned int a = data[offset];
	unsigned int b = data[offset + 1];
	unsigned int c = data[offset + 2];
	unsigned int d = data[offset + 3];
	return a << 24|b << 16|c << 8| d;
}

static inline void write_int(unsigned char* data, unsigned int offset, unsigned int value) noexcept
{
	data[offset] = static_cast<unsigned char>((value & 0xff000000) >> 24);
	data[offset + 1] = static_cast<unsigned char>((value & 0x00ff0000) >> 16);
	data[offset + 2] = static_cast<unsigned char>((value & 0x0000ff00) >> 8);
	data[offset + 3] = static_cast<unsigned char>(value & 0x000000ff);
}


#endif /* H_UTILS_H_ */
