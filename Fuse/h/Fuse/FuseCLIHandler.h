/*
 * FuseCLIHandler.h
 *
 *  Created on: 01 Jan,2020
 *      Author: user
 */

#ifndef FUSE_H_FUSE_FUSECLIHANDLER_H_
#define FUSE_H_FUSE_FUSECLIHANDLER_H_
#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <fuse_lowlevel.h>

class FuseCLIHandler
{
public:
	int main(int argc, char** argv);
private:
	void show_help(char* program_name);

	void show_version();

	bool validate_input(fuse_args* args, fuse_cmdline_opts* opts, char* program_name);

	fuse_session* make_session(fuse_args* args, fuse_cmdline_opts* opts);

	fuse_lowlevel_ops* get_fuse_ll_ops();

};


#endif /* FUSE_H_FUSE_FUSECLIHANDLER_H_ */
