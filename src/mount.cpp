#include <iostream>
#include "string.h"
#include <memory>
#include <openssl/rand.h>
#include "FileUtils/FileUtils.h"
#include "FileUtils/BlockMapper.h"
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

#include "Fuse/FuseCLIHandler.h"


int main(int argc, char** argv)
{
	//mkfs("config.bin");
	//mount("config.bin");
	FuseCLIHandler* fcli = new FuseCLIHandler();
	return fcli->main(argc, argv);
}
