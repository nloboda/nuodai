#include "Configuration/Configuration.h"
#include "YdClient/Auth.h"
#include "FileUtils/CryptoBlockData.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "openssl/rand.h"
#include "string.h"
#include <vector>
#include "memory.h"


Configuration::Configuration(std::shared_ptr<Auth> _auth,
		std::shared_ptr<const char> _key,
		std::shared_ptr<const char> _iv,
		std::shared_ptr<std::vector<CryptoBlockData>> map,
		std::shared_ptr<std::vector<CryptoBlockData>> fat):
	auth(_auth),
	key(_key),
	iv(_iv),
	map(map),
	fat(fat)
{
}

Configuration::~Configuration()
{

}
std::shared_ptr<Auth> Configuration::get_auth()
{
	return this->auth;
}

std::shared_ptr<const char> Configuration::get_key()
{
	return this->key;
}
std::shared_ptr<const char> Configuration::get_iv()
{
	return this->iv;
}

std::shared_ptr<std::vector<CryptoBlockData>> Configuration::get_map()
{
	return this->map;
}

std::shared_ptr<std::vector<CryptoBlockData>> Configuration::get_fat()
{
	return this->fat;
}

static inline void serialize(FILE* file, Auth* a)
{
	std::fwrite(&(a->expires_at), sizeof(unsigned long int),1,file);
	std::fwrite(a->token, sizeof(char), AUTH_TOKEN_FIELD_SIZE, file);
	std::fwrite(a->refresh_token, sizeof(char), AUTH_TOKEN_REFRESH_FIELD_SIZE, file);
}
static inline void serialize(FILE* file, const char* key, unsigned int lenth)
{
	std::fwrite(key, sizeof(char), lenth, file);
}

static inline void deserialize(FILE* file, char* key, unsigned int length)
{
	std::fread(key, sizeof(char), length, file);
}

static inline void deserialize(FILE* file, Auth* a)
{
	std::fread(&(a->expires_at),sizeof(unsigned long int), 1, file);
	std::fread(a->token, sizeof(char), AUTH_TOKEN_FIELD_SIZE, file);
	std::fread(a->refresh_token, sizeof(char), AUTH_TOKEN_REFRESH_FIELD_SIZE, file);
}

static inline void serialize(FILE* file, CryptoBlockData* cbd)
{
	std::fwrite(cbd->block, sizeof(char), 32, file);
	std::fwrite(cbd->iv, sizeof(char), 16, file);
}

static inline void deserialize(FILE* file, CryptoBlockData* cbd)
{
	char buffer[32];
	std::fread(&buffer, sizeof(char), 32, file);
	memcpy(&(cbd->block), &buffer, 32);
	std::fread(&buffer, sizeof(char), 16, file);
	memcpy(&(cbd->iv), &buffer, 16);
}

static inline void serialize(FILE* file, std::shared_ptr<std::vector<CryptoBlockData>> v)
{
	int m_size = v->size();
	std::fwrite(&m_size, sizeof(int), 1, file);
	for(int i=0;i<m_size;i++)
	{
		serialize(file, &(v->at(i)));
	}
}


static inline void deserialize_vector(FILE* file, std::shared_ptr<std::vector<CryptoBlockData>> d)
{
	int size;
	std::fread(&size, sizeof(int),1,file);
	for(int i=0;i<size;i++) {
		char* data = new char[48];
		std::fread(data, sizeof(char), 32, file);
		std::fread(data + 32, sizeof(char), 16, file);
		d->push_back(CryptoBlockData(data, data+32));

	}
	std::cout << size << std::endl;
}

/**
 * Saves configuration to file.
 * NOTE: configuration should
 */
void config::save(Configuration* c, std::string filename)
{
	FILE* config_file = std::fopen(filename.c_str(), "wb");

	if(!config_file) std::runtime_error("Can't open file");

	serialize(config_file, c->get_auth().get());
	serialize(config_file, c->get_key().get(), 32);
	serialize(config_file, c->get_iv().get(), 16);
	serialize(config_file, c->get_map());
	serialize(config_file, c->get_fat());
	std::fclose(config_file);
}

/**
 * loads configuration from file
 */
std::shared_ptr<Configuration> config::load(std::string filename)
{
	FILE* config_file = std::fopen(filename.c_str(), "rb");
	std::shared_ptr<Auth> a = std::make_shared<Auth>();
	char* key = new char[32];
	char* iv = new char[16];
	deserialize(config_file, a.get());
	deserialize(config_file, key, 32);
	deserialize(config_file, iv, 16);
	std::shared_ptr<std::vector<CryptoBlockData>> map = std::make_shared<std::vector<CryptoBlockData>>(std::vector<CryptoBlockData>());
	std::shared_ptr<std::vector<CryptoBlockData>> fat = std::make_shared<std::vector<CryptoBlockData>>(std::vector<CryptoBlockData>());
	deserialize_vector(config_file, map);
	deserialize_vector(config_file, fat);

	return std::make_shared<Configuration>(a,
				std::shared_ptr<const char>(key),
				std::shared_ptr<const char>(iv),
				map,
				fat);
}

static inline char* allocate_random_bytes(unsigned int size)
{
	char* result = new char[size];
	if(RAND_bytes((unsigned char*)result, size)==1) return result;
	delete result;
	throw new std::runtime_error("We've just failed to generate random numbers. What a shame!");
}

std::shared_ptr<Configuration> config::generate()
{
	std::shared_ptr<Auth> a = std::make_shared<Auth>();
	strcpy(a->token,"abcdefghijklmnopqrstuvwxyzabcdefghijklnmopqrstuvwxz0123456789");
	strcpy(a->refresh_token,"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

	char* key = allocate_random_bytes(32);
	char* iv = allocate_random_bytes(16);

	a->expires_at = 0x0;
	std::shared_ptr<std::vector<CryptoBlockData>> map = std::make_shared<std::vector<CryptoBlockData>>(std::vector<CryptoBlockData>());
	std::shared_ptr<std::vector<CryptoBlockData>> fat = std::make_shared<std::vector<CryptoBlockData>>(std::vector<CryptoBlockData>());

	return std::make_shared<Configuration>(
			std::shared_ptr<Auth>(a),
			std::shared_ptr<const char>(key),
			std::shared_ptr<const char>(iv),
			std::shared_ptr<std::vector<CryptoBlockData>>(map),
			std::shared_ptr<std::vector<CryptoBlockData>>(map));
}



