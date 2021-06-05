#include <iostream>
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
#include "FileUtils/CryptoLayer.h"
#include "FileUtils/InodeManager.h"
#include <stdexcept>
#include "Configuration/ConfigManager.h"
#include <cstring>


static int mkfs(std::string config_filename)
{
	std::unique_ptr<Config> config = config_manager::make();

	BlockMapper*  bm  = new BlockMapper();
	PlainFs* pfs = new PlainFs(config->get_working_dir());
	CryptoLayer* cl = new CryptoLayer(pfs, reinterpret_cast<const unsigned char*>(config->get_key()));
	InodeManager* im = new InodeManager(bm, cl, config->get_iv());
	im->new_fat();
	im->save();
	im->suballocate(0, 1); //TODO: fixme this is dirty hat to make root dir have inode=1
	Directory root_dir;
	root_dir.mkdir("whatever", 2);
	unsigned long root_inode =  im->suballocate(0, root_dir.size());
	unsigned char* chunk = reinterpret_cast<unsigned char*>(im->read(root_inode));
	directory::write_directory(&root_dir, chunk);
	im->flush(root_inode);
	std::unique_ptr<char> new_special_block = im->save();
	std::memcpy(config->get_special_block(), new_special_block.get(), 32);
	config_manager::save(config.get(), config_filename);
	return 0;
}

static int mount(std::string config_filename)
{
	std::unique_ptr<Config> config = config_manager::load(config_filename);
	BlockMapper*  bm  = new BlockMapper();
	PlainFs* pfs = new PlainFs(config->get_working_dir());
	CryptoLayer* cl = new CryptoLayer(pfs, reinterpret_cast<const unsigned char*>(config->get_key()));
	InodeManager* im = new InodeManager(bm, cl, config->get_iv());
	im->load(config->get_special_block());
	char* d = im->read(1);
	const unsigned char* data = reinterpret_cast<const unsigned char*>(d);
	std::unique_ptr<Directory> root_dir = directory::read_directory(data);
	std::cout << root_dir->get_child(0).name << std::endl;
}

int main(int argc, char** argv)
{
	mkfs("config.bin");
	mount("config.bin");

}
