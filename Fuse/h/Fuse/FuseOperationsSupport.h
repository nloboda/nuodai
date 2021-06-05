/*
 * FuseOperationsSupport.h
 *
 *  Created on: 01 Jan,2020
 *      Author: user
 */

#ifndef FUSE_H_FUSE_FUSEOPERATIONSSUPPORT_H_
#define FUSE_H_FUSE_FUSEOPERATIONSSUPPORT_H_

#define FUSE_USE_VERSION 31

#include <fuse_lowlevel.h>

#include <stdio.h>
#include <errno.h>
#include <string>

#include "FileUtils/InodeManager.h"

class FuseOperationsSupport
{
public:
	static void init(std::string config_filename);
	static void lookup(fuse_req_t req, fuse_ino_t parent, const char *name);
	static void readdir(fuse_req_t req, fuse_ino_t ino, size_t size,
				     off_t off, struct fuse_file_info *fi);

	static void open(fuse_req_t req, fuse_ino_t ino,
				  struct fuse_file_info *fi);

	static void read(fuse_req_t req, fuse_ino_t ino, size_t size,
				  off_t off, struct fuse_file_info *fi);

	static void getattr(fuse_req_t req, fuse_ino_t ino,
				     struct fuse_file_info *fi);
};


#endif /* FUSE_H_FUSE_FUSEOPERATIONSSUPPORT_H_ */
