 /*
 * InodeTypes.h
 *
 *  Created on: 24 Dec,2019
 *      Author: user
 */

#ifndef FILESYSTEM_H_FILESYSTEM_INODETYPES_H_
#define FILESYSTEM_H_FILESYSTEM_INODETYPES_H_

namespace filesystem
{
	enum class InodeType: unsigned char {
		Dir = 1,
		File = 2
	};
}



#endif /* FILESYSTEM_H_FILESYSTEM_INODETYPES_H_ */
