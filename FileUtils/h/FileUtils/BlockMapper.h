#ifndef FILEUTILS_H_FILEUTILS_BLOCKMAPPER_H_
#define FILEUTILS_H_FILEUTILS_BLOCKMAPPER_H_
#include <vector>
#include "FileUtils/PlainFs.h"
#include "FileUtils/FsConstants.h"

#define BLOCK_MAPPER_HASH_SIZE 32
#define BLOCK_MAPPER_IV_SIZE 16
#define BLOCK_MAPPER_STATE_SIZE 1

#define BLOCK_STATE_SIZE 1;

namespace Blocks
{
	const unsigned int REFERENCE_SIZE = BLOCK_MAPPER_HASH_SIZE + BLOCK_MAPPER_IV_SIZE + BLOCK_MAPPER_STATE_SIZE;
	const unsigned long REFERENCES_IN_BLOCK = FsConstants::BLOCK_SIZE / REFERENCE_SIZE;
}

enum class BlockState: char
{
	NOT_EXISTS = 1,
	ALLOCATED = 2,
	FREE = 4,
	LOCKED = 8
};

class BlockMapper
{
public:
	BlockMapper();
	~BlockMapper();
	BlockState find_mapping(const unsigned long id, unsigned char** hash, unsigned char** iv);
	bool update_mapping(const unsigned long id, const unsigned char* hash, const unsigned char* iv);
	void append_block(char* block);
	char* make_new_fat();
	char* get_fat_data();
private:
	std::vector<char*> map;
};



#endif /* FILEUTILS_H_FILEUTILS_BLOCKMAPPER_H_ */
