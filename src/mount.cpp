#include <iostream>
#define FUSE_USE_VERSION 31

#include <stdexcept>
#include <cstring>

#include "Fuse/FuseCLIHandler.h"


int main(int argc, char** argv)
{
	//mkfs("config.bin");
	//mount("config.bin");
	FuseCLIHandler *fcli = new FuseCLIHandler();
	return fcli->main(argc, argv);
}
