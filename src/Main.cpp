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


void mkfs(std::string config_filename)
{



}

int main(int argc, char** argv)
{
	BlockMapper*  bm  = new BlockMapper();
	std::unique_ptr<Config> before = config_manager::make("~/config.bin", static_cast<char*>("abcdefghijklmnopqrstuvwxyz123456"));
	config_manager::save(before.get(), "~/nuodai.bin");
	std::unique_ptr<Config> after = config_manager::load("~/nuodai.bin");

	PlainFs* pfs = new PlainFs(before->get_working_dir());
	CryptoLayer* cl = new CryptoLayer(pfs, reinterpret_cast<const unsigned char*>(before->get_key()));
	InodeManager* im = new InodeManager(bm, cl);



	std::cout << before->validate()  << std::endl;
	std::cout << after->validate()  << std::endl;
	std::cout << std::memcmp(before->get_data(), after->get_data(), config::size) << std::endl;
}
