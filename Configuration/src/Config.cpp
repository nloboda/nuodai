/*
 * Config.cpp
 *
 *  Created on: 22 Dec,2019
 *      Author: user
 */

#include <openssl/evp.h>
#include <openssl/rand.h>
#include "Configuration/Config.h"
#include "Configuration/ConfigManager.h"

inline static void write_header(char* data) noexcept
{
	for(int i = 0; i < 16; i++) data[config::size - 16 + i] = config::magic_number[i];
}

inline static char* initialize_data()
{
	char* config_data = new char[config::size];
	RAND_bytes(reinterpret_cast<unsigned char*>(config_data), config::size);
	write_header(config_data);
	return config_data;
}


Config::Config():
		data(initialize_data())
{
}

Config::Config(Config& c)
{
	this->data = new char[config::size];
	std::copy(c.data, c.data + config::size, this->data);
}
Config& Config::operator=(Config& c)
{
	if(&c == this) return *this;
	std::copy(c.data, c.data + config::size, this->data);
	return *this;
}

bool Config::operator==(Config& c)
{
	return false;
}


char* Config::get_iv()
{
	return data + config::iv_offset;
}

char* Config::get_key()
{
	return data + config::key_offset;
}

char* Config::get_special_block()
{
	return data + config::special_block_offset;
}

char* Config::get_remote_auth_data()
{
	return data + config::remote_auth_data_offset;
}

char* Config::get_working_dir()
{
	return data + config::working_dir_offset;
}

char* Config::get_data()
{
	return data;
}

bool Config::validate()
{
	char* magic_number = data + config::magic_number_offset;
	for(int i=0; i < 16; i++)
		if(magic_number[i] != config::magic_number[i]) return false;
	return true;
}

Config::~Config()
{
	delete this->data;
}


