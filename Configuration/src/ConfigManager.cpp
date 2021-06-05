/*
 * ConfigManager.cpp
 *
 *  Created on: 22 Dec,2019
 *      Author: user
 */

#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstring>
#include <string>
#include "Configuration/Config.h"
#include "Configuration/ConfigManager.h"

#define ENCRYPT_FAIL throw std::runtime_error("Encryption failed")


inline static void derive_key(std::string password, unsigned char* key, unsigned char* iv)
{
	const char* data = password.c_str();
	int data_len = std::strlen(data);
	const unsigned char* salt = reinterpret_cast<const unsigned char*>("01234567");

	EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha256(),
			salt, reinterpret_cast<const unsigned char*>(data),
			data_len,
			10000,
			key,
			iv);
}

inline static std::string read_password(bool make_repeat)
{
	std::string password;
	std::string password_repeat;
	std::cout << "Please enter password: ";
	std::getline(std::cin, password);
	if(make_repeat)
	{
		std::cout << "Once again: ";
		std::getline(std::cin, password_repeat);
	}
	if(make_repeat && password.compare(password_repeat) != 0)
	{
		std::cout << "Just type the damn thing twice" << std::endl;
		exit(1);
	}else{
		return password;
	}
}

static std::string read_string(std::string promt)
{
	std::string s;
	do {
		std::cout << promt << ": " << std::flush;
	}while (std::getline(std::cin, s) && s.empty());
	return s;
}


inline static EVP_CIPHER_CTX* create_encryption_context()
{
	unsigned char* key = new unsigned char[32];
	unsigned char* iv = new unsigned char[16];
	derive_key(read_password(true), key, iv);
	EVP_CIPHER_CTX* context = EVP_CIPHER_CTX_new();
	if(1 != EVP_EncryptInit_ex(context, EVP_aes_256_cbc(), nullptr, key, iv)) ENCRYPT_FAIL;
	EVP_CIPHER_CTX_set_padding(context, 0);
	delete key;
	delete iv;
	return context;
}

inline static EVP_CIPHER_CTX* create_decryption_context()
{
	unsigned char* key = new unsigned char[32];
	unsigned char* iv = new unsigned char[16];
	derive_key(read_password(false), key, iv);
	EVP_CIPHER_CTX* context = EVP_CIPHER_CTX_new();
	if(1 != EVP_DecryptInit_ex(context, EVP_aes_256_cbc(), nullptr, key, iv)) ENCRYPT_FAIL;
	EVP_CIPHER_CTX_set_padding(context, 0);
	delete key;
	delete iv;
	return context;
}

std::unique_ptr<Config> config_manager::load(std::string filename)
{

	std::unique_ptr<Config> c = std::make_unique<Config>();
	std::ifstream ifs(filename, std::ifstream::binary );
	if(!ifs.is_open()) throw new std::invalid_argument("Can't open " + filename);
	ifs.read(c->get_data(), config::size);
	ifs.close();


	int len;

	EVP_CIPHER_CTX* context = create_decryption_context();
	if(1 != EVP_DecryptUpdate(context,
			reinterpret_cast<unsigned char*>(c->get_data()),
			&len,
			reinterpret_cast<unsigned char*>(c->get_data()),
			config::size))
		ENCRYPT_FAIL;
	if(1 != EVP_DecryptFinal_ex(context, reinterpret_cast<unsigned char*>(c->get_data() + len), &len))
	        ENCRYPT_FAIL;
	EVP_CIPHER_CTX_free(context);

	return c;
}


std::unique_ptr<Config> config_manager::make(char* special_block)
{
 	std::unique_ptr<Config> config = std::make_unique<Config>();

	for(int i = 0; i < 32; i++)
		config->get_special_block()[i] = special_block[i];

	std::string working_dir = read_string("Enter directory where encrypted data will be stored");
	unsigned int working_dir_lenth = std::strlen(working_dir.c_str());
	for(unsigned int i = 0; i < working_dir_lenth; i++)
		config->get_working_dir()[i] = working_dir[i];
	config->get_working_dir()[working_dir_lenth] = '\0';

	return config;
}

void config_manager::save(Config* config, std::string filename)
{

	EVP_CIPHER_CTX* context = create_encryption_context();
	char* encrypted_data = new char[config::size];
	char* in_data = config->get_data();

	int len;
	if(1 != EVP_EncryptUpdate(context,
			reinterpret_cast<unsigned char*>(encrypted_data),
			&len, reinterpret_cast<unsigned char*>(in_data),
			config::size)) ENCRYPT_FAIL;

	if( 1 != EVP_EncryptFinal_ex(context,
			reinterpret_cast<unsigned char*>(encrypted_data + len),
			&len)) ENCRYPT_FAIL;

	EVP_CIPHER_CTX_free(context);

	std::ofstream of(filename, std::ofstream::out | std::ofstream::binary| std::ofstream::trunc);
	if(!of.is_open()) throw std::runtime_error("Can't open file " + filename + " for writing");
	of.write(encrypted_data, config::size);
	of.close();
}


