#include "FileUtils/CryptoLayer.h"
#include "FileUtils/PlainFs.h"
#include "FileUtils/FsConstants.h"
#include "openssl/evp.h"
#include <openssl/err.h>
#include <iostream>
#include <stdexcept>

#define DECRYPT_FAIL throw std::runtime_error("Decryption failed on the OpenSSL side");
#define ENCTRYPT_FAIL throw std::runtime_error("Encryption failed on the OpenSSL side");

namespace crypto_layer
{
	thread_local EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
	thread_local EVP_MD_CTX *digest_context = EVP_MD_CTX_create();
};
/**
 * Encryption layer. This is encrypting wrapper around PlainFs
 * \param plain_fs configured plain fs
 * \param key, should be 256 bit
 */
CryptoLayer::CryptoLayer(PlainFs* plain_fs, const unsigned char* key)
{
	this->plain_fs = plain_fs;
	this->key = key;
}

CryptoLayer::~CryptoLayer()
{

}
/**
 * Reads and decrypts block
 *
 * \param block block hash
 * \param out buffer to output
 * \param iv initialisation vector
 */
void CryptoLayer::read_decrypted(const char* block, char* out, const char* iv)
{
	EVP_CIPHER_CTX_reset(crypto_layer::ctx);
	if(1 != EVP_DecryptInit_ex(crypto_layer::ctx,
			EVP_aes_256_cbc(),
			nullptr,
			key,
			reinterpret_cast<const unsigned char*>(iv))) DECRYPT_FAIL;

	//blocks are multiplies of 32 bytes. Padding is not needed
	EVP_CIPHER_CTX_set_padding(crypto_layer::ctx, 0);

	char in[FsConstants::BLOCK_SIZE];
	plain_fs->read_plain(block, in);
	int len;

	if(1 != EVP_DecryptUpdate(crypto_layer::ctx,
			reinterpret_cast<unsigned char*>(out),
			&len,
			reinterpret_cast<const unsigned char*>(in),
			FsConstants::BLOCK_SIZE)) DECRYPT_FAIL;

	if(1 != EVP_DecryptFinal_ex(crypto_layer::ctx,
			reinterpret_cast<unsigned char*>(out + len), &len)) DECRYPT_FAIL;

}
/**
 * Encrypts and writes block
 *
 * \param in block data to be encrypted
 * \param block block hash
 * \param iv initialization vector
 */
void CryptoLayer::write_encrypted(const char* in, char* block, const char* iv)
{
	EVP_CIPHER_CTX_reset(crypto_layer::ctx);
	if(1 != EVP_EncryptInit_ex(crypto_layer::ctx,
			EVP_aes_256_cbc(),
			nullptr,
			this->key,
			reinterpret_cast<const unsigned char *>(iv))) ENCTRYPT_FAIL;

	//blocks are multiplies of 32 bytes. Padding is not needed
	EVP_CIPHER_CTX_set_padding(crypto_layer::ctx, 0);
	char out[plainfs::BLOCK_SIZE];
	int len;

	if(1 != EVP_EncryptUpdate(crypto_layer::ctx,
			reinterpret_cast<unsigned char*>(out),
			&len,
			reinterpret_cast<const unsigned char*>(in),
			plainfs::BLOCK_SIZE)) ENCTRYPT_FAIL;

	if(1 != EVP_EncryptFinal_ex(crypto_layer::ctx,
			reinterpret_cast<unsigned char*>(out + len),
			&len)) ENCTRYPT_FAIL;

	plain_fs->write_plain(out, block);
}

#undef DECRYPT_FAIL
#undef ENCTRYPT_FAIL
