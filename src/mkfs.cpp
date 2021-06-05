#include "FileUtils/FileUtils.h"
#include "FileUtils/BlockMapper.h"
#include "FileSystem/Directory.h"
#include "FileUtils/CryptoLayer.h"
#include "FileUtils/InodeManager.h"
#include "Configuration/ConfigManager.h"
#include <cstring>
#include <iostream>


static void show_help() {
	std::cout
	<< "RTFM will follow " << std::endl
	<< std::endl
	<< "Usage: " << std::endl
	<< "mkfs <configfilename> " << std::endl
	<< "for example: mkfs config.bin " << std::endl;
}

static int check_args(int argc, char** argv) {

}
static int make(int argc, char** argv)
{
	if(argc!=1)
	{
		show_help();
		return 1;
	}

	std::unique_ptr<Config> config = config_manager::make();
	BlockMapper* bm  = new BlockMapper();
	PlainFs* pfs = new PlainFs(config->get_working_dir());
	CryptoLayer* cl = new CryptoLayer(pfs, reinterpret_cast<const unsigned char*>(config->get_key()));
	InodeManager* im = new InodeManager(bm, cl, config->get_iv());
	im->new_fat();
	im->save();
	im->suballocate(0, 1); //TODO: fixme this is dirty hack to make root dir have inode=1
	Directory root_dir;
	root_dir.mkdir("whatever", 2);
	root_dir.mkdir("whatever_else", 3);
	unsigned long root_inode =  im->suballocate(0, root_dir.size());
	unsigned char* chunk = reinterpret_cast<unsigned char*>(im->read(root_inode));
	directory::write_directory(&root_dir, chunk);
	im->flush(root_inode);
	im->save();
	std::unique_ptr<char> new_special_block = im->save();
	std::memcpy(config->get_special_block(), new_special_block.get(), 32);
	config_manager::save(config.get(), argv[1]);
	im->reset();
	return 0;
}



static int sanity_check(int argc, char** argv)
{
	std::unique_ptr<Config> config = config_manager::load(argv[1]);
	BlockMapper*  bm  = new BlockMapper();
	PlainFs* pfs = new PlainFs(config->get_working_dir());
	CryptoLayer* cl = new CryptoLayer(pfs, reinterpret_cast<const unsigned char*>(config->get_key()));
	InodeManager* im = new InodeManager(bm, cl, config->get_iv());
	im->load(config->get_special_block());
	std::unique_ptr<Directory> d1 =  directory::read_directory(reinterpret_cast<unsigned char*>(im->read(0)));
	std::unique_ptr<Directory> d2 =  directory::read_directory(reinterpret_cast<unsigned char*>(im->read(1)));
	std::cout << d2->get_child(0).name << std::endl;
}

int main(int argc, char** argv)
{
	make(argc, argv);
	sanity_check(argc, argv);
}
