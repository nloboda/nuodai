
#ifndef CONFIGURATION_H_CONFIGURATION_CONFIG_H_
#define CONFIGURATION_H_CONFIGURATION_CONFIG_H_
#include <memory>
/**
 *
 */

namespace config
{
	constexpr unsigned int iv_offset = 0;
	constexpr unsigned int key_offset = 16;
	constexpr unsigned int special_block_offset = key_offset + 32;
	constexpr unsigned int working_dir_offset = special_block_offset + 32;
	constexpr unsigned int remote_auth_data_offset = working_dir_offset + 1024;
	constexpr unsigned int magic_number_offset = remote_auth_data_offset + 1024;
	constexpr unsigned int size = magic_number_offset + 16;
	constexpr char magic_number[]{'I','l','i','k',
		'e','b','l','a',
		'c','k','c','o',
		'c','k','s','!'};
}
static_assert(config::size % 32 == 0, "Config size should be even to 32");

class Config
{
public:
	Config();
	~Config();
	bool operator==(Config &c);
	bool operator!=(Config &c);
	Config(Config& c);
	Config& operator=(Config& c);
	char* get_key();
	char* get_iv();
	char* get_special_block();
	char* get_working_dir();
	char* get_remote_auth_data();
	char* get_data();
	bool validate();
private:
	char* data;
};



#endif /* CONFIGURATION_H_CONFIGURATION_CONFIG_H_ */
