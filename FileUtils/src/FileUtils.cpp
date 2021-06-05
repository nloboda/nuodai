#include "FileUtils/FileUtils.h"
#include "string.h"

static const unsigned char* LETTERS = (const unsigned char *)"0123456789abcdef";
static unsigned char left_char_to_hash(const unsigned char b){
	return LETTERS[(unsigned char)(b >> 4)];
}

static unsigned char right_char_to_hash(const unsigned char b)
{
	return LETTERS[(unsigned char)(b & 0xf)];
}

static unsigned char map_byte_to_char(unsigned char b)
{
	if(b >= (unsigned char)'0' && b <= (unsigned char)'9')
	{
		return (unsigned char) (b - 48);
	}else if(b >= (unsigned char)'a' && b <= (unsigned char)'f'){
		return (unsigned char) (b - 87);
	} else {
		throw; //should never happen unles there's error in implementation
	}
}

void FileUtils::hash_to_name(const unsigned char* hash, unsigned char* filename)
{
	for(int i=0; i < 32; i++)
	{
		filename[i*2] = left_char_to_hash(hash[i]);
		filename[i*2 + 1] = right_char_to_hash(hash[i]);
	}
}

void FileUtils::name_to_hash(const unsigned char* filename, unsigned char* hash)
{
	
	for(int i=0;i<32;i++)
	{
		const unsigned char left = map_byte_to_char((const unsigned char)(filename[i*2]));
		const unsigned char right = map_byte_to_char((const unsigned char)(filename[i*2 + 1]));
		hash[i] = (left << 4) | (right & 0xf);
	}
}

bool FileUtils::compare_hashes(const unsigned char* h1, const unsigned char* h2)
{
	return memcmp(h1,h2, 32)==0;
}


