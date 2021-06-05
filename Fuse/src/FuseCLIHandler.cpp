/*
 * FuseCLIHandler.cpp
 *
 *  Created on: 01 Jan,2020
 *      Author: user
 */

#include "Fuse/FuseCLIHandler.h"
#include "Fuse/FuseOperationsSupport.h"
#include "Configuration/ConfigManager.h"

#include "string.h"
#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <fuse_common.h>
#include <fuse_lowlevel.h>
#include <stdio.h>
#include <stdlib.h>

int FuseCLIHandler::main(int argc, char** argv)
{
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
	struct fuse_cmdline_opts opts;
	int ret = -1;

	if (fuse_parse_cmdline(&args, &opts) != 0) return 1;

	if(!validate_input(&args, &opts, argv[0]))
	{
		free(opts.mountpoint);
		fuse_opt_free_args(&args);
		return 1;
	}

	struct fuse_session *se = make_session(&args, &opts);

	if (se == nullptr){
		free(opts.mountpoint);
		fuse_opt_free_args(&args);
		return -1;
	}


	fuse_daemonize(opts.foreground);

	/* Block until ctrl+c or fusermount -u */
	if (opts.singlethread)
		ret = fuse_session_loop(se);
	else
		ret = fuse_session_loop_mt(se, opts.clone_fd);

	fuse_session_unmount(se);
	fuse_remove_signal_handlers(se);
	fuse_session_destroy(se);
	free(opts.mountpoint);
	fuse_opt_free_args(&args);
	return 0;
}

void FuseCLIHandler::show_help(char* program_name)
{
	printf("usage: %s [options] <mountpoint>\n\n", program_name);
	fuse_cmdline_help();
	fuse_lowlevel_help();
}

void FuseCLIHandler::show_version()
{
	printf("FUSE library version %s\n", fuse_pkgversion());
	fuse_lowlevel_version();
}

bool FuseCLIHandler::validate_input(fuse_args* args, fuse_cmdline_opts* opts, char* program_name)
{

	if (opts->show_help) {
		this->show_help(program_name);
		return false;
	} else if (opts->show_version) {
		this->show_version();
		return false;
	}
	if(opts->mountpoint == nullptr) {
		this->show_help(program_name);
		return false;
	}
	return true;
}

fuse_session* FuseCLIHandler::make_session(fuse_args* args, fuse_cmdline_opts* opts)
{
	fuse_lowlevel_ops* z = get_fuse_ll_ops();
	struct fuse_session *se;
	se = fuse_session_new(args, z, sizeof(*z), nullptr);
	if(se == nullptr)
	{
		return nullptr;
	}
	if (fuse_set_signal_handlers(se) != 0)
	{
		fuse_session_destroy(se);
		return nullptr;
	}
	if (fuse_session_mount(se, opts->mountpoint) != 0)
	{
		fuse_remove_signal_handlers(se);
		fuse_session_destroy(se);
		return nullptr;
	}
	return se;
}

fuse_lowlevel_ops* FuseCLIHandler::get_fuse_ll_ops()
{
	fuse_lowlevel_ops *a = new fuse_lowlevel_ops;
	FuseOperationsSupport::init("config.bin");
	a->lookup	= FuseOperationsSupport::lookup;
	a->getattr	= FuseOperationsSupport::getattr;
	a->readdir	= FuseOperationsSupport::readdir;
	a->open		= FuseOperationsSupport::open;
	a->read		= FuseOperationsSupport::read;
	return a;
}
