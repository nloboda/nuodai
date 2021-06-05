/*
 * FuseHandle.cpp
 *
 *  Created on: 20 Oct,2019
 *      Author: user
 */

#define FUSE_USE_VERSION 31
#include <fuse_lowlevel.h>
#include <fuse_common.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include "Fuse/FuseAdapter.h"

static const char *hello_str = "Hello World!\n";
static const char *hello_name = "hello";

struct dirbuf {
	char *p;
	size_t size;
};

static void dirbuf_add(fuse_req_t req, struct dirbuf *b, const char *name,
		       fuse_ino_t ino)
{
	struct stat stbuf;
	size_t oldsize = b->size;
	b->size += fuse_add_direntry(req, nullptr, (size_t)0, name, nullptr, 0);
	b->p = (char *) realloc(b->p, b->size);
	memset(&stbuf, 0, sizeof(stbuf));
	stbuf.st_ino = ino;
	fuse_add_direntry(req, b->p + oldsize, b->size - oldsize, name, &stbuf,
			  b->size);
}

#define min(x, y) ((x) < (y) ? (x) : (y))

static int hello_stat(fuse_ino_t ino, struct stat *stbuf)
{
	stbuf->st_ino = ino;
	switch (ino) {
	case 1:
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		break;

	case 2:
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(hello_str);
		break;

	default:
		return -1;
	}
	return 0;
}


static void hello_ll_getattr(fuse_req_t req, fuse_ino_t ino,
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

static void hello_ll_lookup(fuse_req_t req, fuse_ino_t parent, const char *name)
{
	struct fuse_entry_param e;

	if (parent != 1 || strcmp(name, hello_name) != 0)
	{
		fuse_reply_err(req, ENOENT);
	} else {
		memset(&e, 0, sizeof(e));
		e.ino = 2;
		e.attr_timeout = 1.0;
		e.entry_timeout = 1.0;
		hello_stat(e.ino, &e.attr);

		fuse_reply_entry(req, &e);
	}
}


static int reply_buf_limited(fuse_req_t req, const char *buf, size_t bufsize,
			     off_t off, size_t maxsize)
{
	if (off < bufsize)
		return fuse_reply_buf(req, (const char*)(buf + off),
				      min(bufsize - off, maxsize));
	else
		return fuse_reply_buf(req, nullptr, 0);
}

static void hello_ll_readdir(fuse_req_t req, fuse_ino_t ino, size_t size,
			     off_t off, struct fuse_file_info *fi)
{
	(void) fi;

	if (ino != 1)
	{
		fuse_reply_err(req, ENOTDIR);
	}
	else
	{
		struct dirbuf b;

		memset(&b, 0, sizeof(b));
		dirbuf_add(req, &b, ".", 1);
		dirbuf_add(req, &b, "..", 1);
		dirbuf_add(req, &b, hello_name, 2);
		reply_buf_limited(req, b.p, b.size, off, size);
		free(b.p);
	}
}

static void hello_ll_open(fuse_req_t req, fuse_ino_t ino,
			  struct fuse_file_info *fi)
{
	if (ino != 2)
		fuse_reply_err(req, EISDIR);
	else if ((fi->flags & O_ACCMODE) != O_RDONLY)
		fuse_reply_err(req, EACCES);
	else
		fuse_reply_open(req, fi);
}

static void hello_ll_read(fuse_req_t req, fuse_ino_t ino, size_t size,
			  off_t off, struct fuse_file_info *fi)
{
	(void) fi;

	assert(ino == 2);
	reply_buf_limited(req, hello_str, strlen(hello_str), off, size);
}

FuseAdapter::FuseAdapter()
{

}

fuse_lowlevel_ops* FuseAdapter::get_fuse_ll_ops() {
	fuse_lowlevel_ops *a = new fuse_lowlevel_ops;
	a->lookup	= hello_ll_lookup;
	a->getattr	= hello_ll_getattr;
	a->readdir	= hello_ll_readdir;
	a->open		= hello_ll_open;
	a->read		= hello_ll_read;
	return a;
}

void FuseAdapter::finalise(fuse_session* session, fuse_cmdline_opts opts, fuse_args* args, Stage stage)
{
	if(stage >= REMOVE_SIGNAL_HANDLERS)
	{
		fuse_remove_signal_handlers(session);
	}

	if(stage >=DESTROY_SESSION)
	{
		fuse_session_destroy(session);
	}

	if(stage >= FREE_MOUNTPOINT)
	{
		free(opts.mountpoint);
	}

	fuse_opt_free_args(args);
}

struct fuse_session* FuseAdapter::create_session(struct fuse_args* args,
		  struct fuse_cmdline_opts *opts,
	      size_t op_size)
{
		fuse_lowlevel_ops *a = this->get_fuse_ll_ops();
		struct fuse_session *se = fuse_session_new(args, a, sizeof(*a), nullptr);

		if (se == nullptr)
		{
			this->finalise(se, *opts, args, FREE_MOUNTPOINT);
		}

		if (fuse_set_signal_handlers(se) != 0)
		{
			this->finalise(se, *opts, args, DESTROY_SESSION);
			return nullptr;
		}

		if (fuse_session_mount(se, opts->mountpoint) != 0)
		{
			this->finalise(se, *opts, args, REMOVE_SIGNAL_HANDLERS);
			return nullptr;
		}

		return se;
}

Action FuseAdapter::parse_cmd_opts(struct fuse_args* args, struct fuse_cmdline_opts* opts)
{
	if (fuse_parse_cmdline(args, opts) != 0)
	{
		return DIE_DUE_TO_ERROR;
	}

	if (opts->show_help)
	{
		return SHOW_HELP;
	}
	if (opts->show_version)
	{
		return SHOW_VERSION;
	}
	if (opts->mountpoint == nullptr){
		return DIE_DUE_TO_ERROR;
	}
	return PROCEED;
}

FuseAdapter::~FuseAdapter()
{

}
