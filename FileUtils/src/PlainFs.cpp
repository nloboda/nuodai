#include "FileUtils/PlainFs.h"
#include "FileUtils/FileUtils.h"
#include "FileUtils/FsConstants.h"
#include <string>
#include <string.h>
#include <iostream>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdexcept>

#define ERR_WORKING_DIRECTORY_TOO_LONG std::runtime_error("Snap! Working directory name is too long")
#define ERR_REMOTE_HASH_INVALID std::runtime_error("Hash is invalid! someone has been tampering with your files!")
#define ERR_OPENSSL_ALLOC_FAILED std::runtime_error("Openssl failed to allocate memory")
#define ERR_OPENSSL_DIGEST_INIT_FAILED std::runtime_error("Openssl's digest init failed");
#define ERR_OPENSSL_DIGEST_UPDATE_FILED std::runtime_error("Openssl's digest update failed");
#define ERR_OPENSSL_DIGEST_FINALIZE_FILED std::runtime_error("Openssl's digest filanize failed");
#define ERR_WORKING_DIRECTORY_NOT_ENDING_WITH_SLASH std::runtime_error("Working directory name should end with /");
#define ERR_WORKING_DIRECTORY_PATH_IS_EMPTY std::runtime_error("Working may not be empty");
#define ERR_DIRECTORY_SHOULD_ENDS_WITH_SLASH(dirname) std::runtime_error("Working directory name should end with / " + dirname + " given")
#define ERR_CANT_OPEN_FILE(filename) std::runtime_error("Can't open the file: " + std::string(filename));

namespace plainfs_guts
{
	const int FILENAME_BUFFER_SIZE = 1024;
	/**
	 * length of filename for chunk. Currently we're using sha256 which
	 * has 256 bits hash. in hex this will be 64 symbols
	 */
	const int FILENAME_LENGTH = 64;
	thread_local char filename_buffer[FILENAME_BUFFER_SIZE];
	thread_local int working_directory_lenth;
}

inline void validate_working_direcotory(const std::string working_directory)
{
	const int directory_size = working_directory.size();
	if(directory_size < 1)
		throw ERR_WORKING_DIRECTORY_PATH_IS_EMPTY;

	char last_char = working_directory.c_str()[directory_size - 1];
	if(last_char != '/') throw ERR_DIRECTORY_SHOULD_ENDS_WITH_SLASH(working_directory);

	const int max_directory_lenth = plainfs_guts::FILENAME_BUFFER_SIZE - plainfs_guts::FILENAME_LENGTH - 1;
	if(directory_size > max_directory_lenth)
		throw ERR_WORKING_DIRECTORY_TOO_LONG;
}


static void hash_block(const unsigned char* block, unsigned char* buffer)
{
	EVP_MD_CTX *mdctx;
	if((mdctx = EVP_MD_CTX_create()) == nullptr)
		throw ERR_OPENSSL_ALLOC_FAILED;
	if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr))
		throw ERR_OPENSSL_DIGEST_INIT_FAILED;
	if(1 != EVP_DigestUpdate(mdctx, block, FsConstants::BLOCK_SIZE))
		throw ERR_OPENSSL_DIGEST_UPDATE_FILED;
	unsigned int digest_len = 32;

	if(1 != EVP_DigestFinal_ex(mdctx, buffer, &digest_len))
		throw ERR_OPENSSL_DIGEST_FINALIZE_FILED;
	EVP_MD_CTX_destroy(mdctx);
}


/**
 * PlainFs is an abstraction layer on top of underlying filesystem
 * hides allows  the rest of application to use just hashes
 * \param working_directory working directory, can't be longer then 960 bytes
 */
PlainFs::PlainFs(const std::string working_directory)
{
	validate_working_direcotory(working_directory);
	plainfs_guts::working_directory_lenth = working_directory.size();;

	strcpy(plainfs_guts::filename_buffer, working_directory.c_str());
	//This damn thing adds termination symbol after filename
	plainfs_guts::filename_buffer[plainfs_guts::working_directory_lenth + 64] = '\0';
}

PlainFs::~PlainFs()
{

}

bool PlainFs::verify_hash(const unsigned char* block, const char* buffer)
{
	unsigned char* digest  = reinterpret_cast<unsigned char *>(OPENSSL_malloc(EVP_MD_size(EVP_sha256())));
	if(digest == nullptr)
		throw ERR_OPENSSL_ALLOC_FAILED;

	hash_block(block, digest);
	bool valid = memcmp(buffer, digest, 32) == 0;
	OPENSSL_free(digest);

	return valid;
}

/**
 * reads chunk of data into buffer
 * \param block block containing sha256 hash of the block to be retrieved
 * \param buffer buffer contents will be puke into
 */
void PlainFs::read_plain(const char* block, char* buffer)
{
	char* filename_buffer = plainfs_guts::filename_buffer + plainfs_guts::working_directory_lenth;
	FileUtils::hash_to_name(reinterpret_cast<const unsigned char*>(block), reinterpret_cast<unsigned char *>(filename_buffer));

	FILE* block_file = fopen(plainfs_guts::filename_buffer, "rb");
	if(block_file == nullptr) throw ERR_CANT_OPEN_FILE(plainfs_guts::filename_buffer);

	fread(buffer, FsConstants::BLOCK_SIZE, 1, block_file);
	fclose(block_file);

	if(!verify_hash(reinterpret_cast<const unsigned char *>(buffer), block))
		throw ERR_REMOTE_HASH_INVALID;
}
/**
 * writes buffer into block
 * \param buffer buffer to be written
 * \param block (out) new block sha256 hash
 */
void PlainFs::write_plain(const char* buffer, char* block)
{
	unsigned char* digest;
	if((digest = reinterpret_cast<unsigned char *>(OPENSSL_malloc(EVP_MD_size(EVP_sha256())))) == nullptr)
		throw ERR_OPENSSL_ALLOC_FAILED;

	hash_block(reinterpret_cast<const unsigned char *>(buffer), digest);
	memcpy(block, digest, 32);
	OPENSSL_free(digest);

	char* filename_buffer = plainfs_guts::filename_buffer + plainfs_guts::working_directory_lenth;
	FileUtils::hash_to_name(reinterpret_cast<const unsigned char *>(block), reinterpret_cast<unsigned char *>(filename_buffer));

	FILE* block_file = fopen(plainfs_guts::filename_buffer, "wb");
	fwrite(buffer, FsConstants::BLOCK_SIZE, 1, block_file);
	fclose(block_file);
}
