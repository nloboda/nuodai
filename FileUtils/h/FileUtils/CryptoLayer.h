#ifndef CRYPTOLAYER_H_INCLUDED
#define CRYPTOLAYER_H_INCLUDED
#include "FileUtils/PlainFs.h"
#include "openssl/evp.h"
class CryptoLayer
{
	public:
		CryptoLayer(PlainFs* plain_fs, const unsigned char* key);
		~CryptoLayer();
		void read_decrypted(const char* block, char* buffer, const char* iv);
		void write_encrypted(const char* buffer, char* block, const char* iv);
	private:
		const unsigned char* key;
		PlainFs* plain_fs;
};

#endif // CRYPTOLAYER_H_INCLUDED
