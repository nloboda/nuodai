/*
 * BlockSemaphore.h
 *
 *  Created on: 01 Dec,2019
 *      Author: user
 */

#ifndef FILEUTILS_H_FILEUTILS_BLOCKSEMAPHORE_H_
#define FILEUTILS_H_FILEUTILS_BLOCKSEMAPHORE_H_

#include <mutex>
enum Access
{
	READ, WRITE
};
class BlockSemaphore {
public:
	BlockSemaphore();
	~BlockSemaphore();
	bool acquire(long chunk_id, Access access);
	bool release(long chunk_id, Access access);

private:
	bool get_bit(long chunk_id);
	void set_bit(long chunk_id, bool bit);
	constexpr static const unsigned int LOCK_GRANULARITY = 8;
	unsigned long bits[LOCK_GRANULARITY];
	std::mutex mutex;
};


#endif /* FILEUTILS_H_FILEUTILS_BLOCKSEMAPHORE_H_ */
