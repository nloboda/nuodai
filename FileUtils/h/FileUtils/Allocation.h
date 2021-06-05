/*
 * Allocation.h
 *
 *  Created on: 01 Dec,2019
 *      Author: user
 */

#ifndef FILEUTILS_H_FILEUTILS_ALLOCATION_H_
#define FILEUTILS_H_FILEUTILS_ALLOCATION_H_

#include <memory>
enum class AllocationType: unsigned char
{
	/**
	 * Small directory. All children information in current allocation
	 */
	SmallDir,
	/**
	 * Big directory. contains list of allocations pointing to
	 * subdir info
	 */
	BigDir,
	/**
	 * Small file that fits into allocation
	 */
	SmallFile,
	/**
	 * Big file. Contains list of allocations pointing to parts of file
	 */
	BigFile,
	/**
	 * One of chunks containing BigDir info
	 */
	DirData,
	/**
	 * One of chunks having BigFile Data
	 */
	FileData

};

class Allocation
{
public:
	Allocation(unsigned char* data, unsigned char allocation);
	~Allocation();
	AllocationType get_type();
	unsigned int get_length();
	unsigned char* get_data();
private:
	unsigned char* data;
	unsigned char allocation;
};


#endif /* FILEUTILS_H_FILEUTILS_ALLOCATION_H_ */
