/*
 * FuseHandle.h
 *
 *  Created on: 20 Oct,2019
 *      Author: user
 */

#ifndef FUSE_H_FUSEADAPTER_
#define FUSE_H_FUSEADAPTER_
#define FUSE_USE_VERSION 31
#include <fuse_lowlevel.h>

enum Stage {
	REMOVE_SIGNAL_HANDLERS = 0,
	DESTROY_SESSION = 1,
	FREE_MOUNTPOINT = 2
};

enum Action {
	SHOW_HELP = 0,
	SHOW_VERSION = 1,
	DIE_DUE_TO_ERROR = 2,
	PROCEED = 3
};

class FuseAdapter {
public:
	FuseAdapter();
	~FuseAdapter();
	fuse_lowlevel_ops*  get_fuse_ll_ops();
	void finalise(fuse_session* session, fuse_cmdline_opts opts, fuse_args* args, Stage stage);


	struct fuse_session* create_session(struct fuse_args* args,
			  struct fuse_cmdline_opts *opts,
		      size_t op_size);

	Action parse_cmd_opts(struct fuse_args* args, struct fuse_cmdline_opts* opts);
};


#endif /* FUSE_H_FUSEADAPTER_ */
