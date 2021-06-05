#include <iostream>
#include "Configuration/Configuration.h"
#include "YdClient/YandexAuthenticator.h"
#include "YdClient/YandexDiskClient.h"
#include "string.h"
#include <memory>
#include <openssl/rand.h>
#include "FileUtils/FileUtils.h"
#include "FileUtils/BlockMapper.h"
#include "Fuse/FuseAdapter.h"
#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <fuse_common.h>
#include <fuse_lowlevel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <assert.h>
#include "FileSystem/Directory.h"
#include <stdexcept>


static void finalise(fuse_session* session, fuse_cmdline_opts opts, fuse_args* args, Stage stage)
{
	if(stage >= REMOVE_SIGNAL_HANDLERS)
	{
		fuse_remove_signal_handlers(session);
	}
	if(stage >= DESTROY_SESSION)
	{
		fuse_session_destroy(session);
	}
	if(stage >= FREE_MOUNTPOINT)
	{
		free(opts.mountpoint);
	}
	fuse_opt_free_args(args);
}

static void print(EntityType et)
{
	switch(et)
	{
	case EntityType::File:
		std::cout << "File" << std::endl;
		break;
	case EntityType::Dir:
		std::cout << "Directory" << std::endl;
		break;
	default:
		throw std::invalid_argument("lkjhgcfxdgfchj");

	}
}

int main(int argc, char** argv)
{
	/**
	 * We should not use key per block, just iv will be enough
	 */

	const char data[] = {
			2,1,
			't','e','s','t','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
			'\0','\0','\0','\0','\0','\0','\0', 'a',
			1, 'p','i','s','u','n'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
			'\0','\0','\0','\0','\0','\0','\0', 'a',

			0, 'k','u','k','u','n'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
			,'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
			'\0','\0','\0','\0','\0','\0','\0', 'a'


	};

	Directory d((const char*)data);
	std::cout << (int)d.count_children() << std::endl;
	std::cout << d.get_name(0) << std::endl;
	std::cout << d.get_name(1) << std::endl;
	print(d.get_type(0));
	print(d.get_type(2));
	return 0;

	/*
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
	struct fuse_session *se = nullptr;
	struct fuse_cmdline_opts opts;
	int ret = -1;
	FuseAdapter* fh = new FuseAdapter();
	switch(fh->parse_cmd_opts(&args, &opts))
	{
		case SHOW_HELP:
		case SHOW_VERSION:
		case DIE_DUE_TO_ERROR:
			fh->finalise(se, opts,&args, FREE_MOUNTPOINT);
			return 1;
		case PROCEED:
			se = fh->create_session(&args, &opts, sizeof(opts));;
			if(se != nullptr)
			{
				fuse_daemonize(opts.foreground);

				/ * Block until ctrl+c or fusermount -u * /
				if (opts.singlethread)
				{
					ret = fuse_session_loop(se);
				}
				else
				{
					ret = fuse_session_loop_mt(se, opts.clone_fd);
				}
				fuse_session_unmount(se);
			}
			fuse_opt_free_args(&args);
	}



	return ret ? 1 : 0;*/
/**
	char* block = new char[plainfs::BLOCK_SIZE];
	BlockMapper bm = BlockMapper();
	bm.append_block(block);
	bool a = bm.update_mapping((const unsigned long)0, (const unsigned char*)"0123456789abcdef0123456789abcde",
			(const unsigned char*)"0123456789abcde");
	std::cout << "update successful " << a << std::endl;
	unsigned char* hash = new unsigned char[32];
	unsigned char* key = new unsigned char[32];
	unsigned char* iv = new unsigned char[16];
	bm.find_mapping(0, &hash, &iv);
	std::cout << hash << std::endl;
*/
/*
	std::shared_ptr<Configuration> c =  config::generate();
	char* block = new char[32];
	char* key = new char[32];
	char* iv = new char[16];
	memcpy(block,"0123456789012345678901234567\0912", 32);
	memcpy(key,"98765432109876543210987654\021012", 32);
	memcpy(iv,"10293847561029\085", 16);

	std::cout << block << std::endl;
	{
		c->get_fat()->push_back(CryptoBlockData(block, key, iv));
		c->get_fat()->push_back(CryptoBlockData(block, key, iv));
		c->get_fat()->push_back(CryptoBlockData(block, key, iv));
	}
	std::cout << c->get_fat()->size() << std::endl;
	config::save(c.get(),"../tdisk/ccc.conf");
	std::shared_ptr<Configuration> conf = config::load("../tdisk/ccc.conf");
	std::cout << conf->get_fat()->at(0).iv << std::endl;


	std::cout << "done" << std::endl;
*/
}
