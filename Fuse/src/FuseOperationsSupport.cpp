/*
 * FuseOperationsSupport.cpp
 *
 *  Created on: 01 Jan,2020
 *      Author: user
 */

#include "Fuse/FuseOperationsSupport.h"
#define FUSE_USE_VERSION 31

#include <fuse_lowlevel.h>

#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <assert.h>
#include <stdexcept>
#include <cstring>
#include "FileSystem/Directory.h"
#include "Configuration/Config.h"
#include "Configuration/ConfigManager.h"

static const char *hello_str = "Hello World!\n";
static const char *hello_name = "hello";

namespace guts_handle
{
	InodeManager* inode_manager = nullptr;
}


static int hello_stat(fuse_ino_t ino, struct stat *stbuf)
{
	stbuf->st_ino = ino;
	switch (ino) {
	case 1:
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		break;

	case 666:
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink	 = 1;
		stbuf->st_size = strlen(hello_str);
		break;

	default:
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink	 = 1;
	}
	return 0;
}
struct dirbuf {
	char *p;
	size_t size;
};

static void dirbuf_add(fuse_req_t req, struct dirbuf *b, const char *name,
		       fuse_ino_t ino)
{
	struct stat stbuf;
	size_t oldsize = b->size;
	b->size += fuse_add_direntry(req, NULL, 0, name, NULL, 0);
	b->p = (char *) realloc(b->p, b->size);
	memset(&stbuf, 0, sizeof(stbuf));
	stbuf.st_ino = ino;
	fuse_add_direntry(req, b->p + oldsize, b->size - oldsize, name, &stbuf,
			  b->size);
}

#define min(x, y) ((x) < (y) ? (x) : (y))

static int reply_buf_limited(fuse_req_t req, const char *buf, size_t bufsize,
			     off_t off, size_t maxsize)
{
	if (off < bufsize)
		return fuse_reply_buf(req, buf + off,
				      min(bufsize - off, maxsize));
	else
		return fuse_reply_buf(req, NULL, 0);
}


void FuseOperationsSupport::lookup(fuse_req_t req, fuse_ino_t parent, const char *name)
{
	struct fuse_entry_param e;

	if (parent != 1 || strcmp(name, hello_name) != 0)
		fuse_reply_err(req, ENOENT);
	else {
		memset(&e, 0, sizeof(e));
		e.ino = 2;
		e.attr_timeout = 1.0;
		e.entry_timeout = 1.0;
		hello_stat(e.ino, &e.attr);

		fuse_reply_entry(req, &e);
	}
}

void FuseOperationsSupport::init(std::string config_filename)
{
	std::unique_ptr<Config> config = config_manager::load(config_filename);
	PlainFs* pfs = new PlainFs(config->get_working_dir());
	CryptoLayer* cl = new CryptoLayer(pfs, reinterpret_cast<const unsigned char*>(config->get_key()));
	BlockMapper*  bm  = new BlockMapper();
	guts_handle::inode_manager = new InodeManager(bm, cl, config->get_iv());
	guts_handle::inode_manager->load(config->get_special_block());
	guts_handle::inode_manager->read(1);
}

void FuseOperationsSupport::readdir(fuse_req_t req, fuse_ino_t ino, size_t size,
				 off_t off, struct fuse_file_info *fi)
{
	(void) fi;

	if (ino != 1)
		fuse_reply_err(req, ENOTDIR);
	else {
		struct dirbuf b;
		char* data = guts_handle::inode_manager->read(ino);
		std::unique_ptr<Directory> dir = directory::read_directory(reinterpret_cast<unsigned char*>(data));

		memset(&b, 0, sizeof(b));
		dirbuf_add(req, &b, ".", 1);
		dirbuf_add(req, &b, "..", 1);
		/*for(unsigned int i = 0; i < dir->count_children(); i++)
		{
			Dirchild dc = dir->get_child(i);
			dirbuf_add(req, &b, dc.name, dc.inode);
		}*/
		reply_buf_limited(req, b.p, b.size, off, size);
		free(b.p);
	}
}

void FuseOperationsSupport::open(fuse_req_t req, fuse_ino_t ino,
			  struct fuse_file_info *fi)
{
	if (ino != 2)
		fuse_reply_err(req, EISDIR);
	else if ((fi->flags & O_ACCMODE) != O_RDONLY)
		fuse_reply_err(req, EACCES);
	else
		fuse_reply_open(req, fi);
}

void FuseOperationsSupport::read(fuse_req_t req, fuse_ino_t ino, size_t size,
			  off_t off, struct fuse_file_info *fi)
{
	(void) fi;

	assert(ino == 2);
	reply_buf_limited(req, hello_str, strlen(hello_str), off, size);
}

void FuseOperationsSupport::getattr(fuse_req_t req, fuse_ino_t ino,
				 struct fuse_file_info *fi)
{
	struct stat stbuf;

	(void) fi;

	memset(&stbuf, 0, sizeof(stbuf));
	if (hello_stat(ino, &stbuf) == -1)
		fuse_reply_err(req, ENOENT);
	else
		fuse_reply_attr(req, &stbuf, 1.0);
}

