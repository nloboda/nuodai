	/*
 * EntityType.h
 *
 *  Created on: 11 Nov,2019
 *      Author: user
 */

#ifndef FILESYSTEM_H_FILESYSTEM_ENTITYTYPE_H_
#define FILESYSTEM_H_FILESYSTEM_ENTITYTYPE_H_


enum EntityType
{
	File,
	Dir,
	FileBlob,
	DirBlob
};

namespace entities
{
	/**
	 * size of table entry. All your shit should fit here. If it does not you should use additional entity and point to it.
	 */
	const unsigned int TE_SIZE = 16384;
}
#endif /* FILESYSTEM_H_FILESYSTEM_ENTITYTYPE_H_ */
