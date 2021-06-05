#ifndef FILE_UTILS_HEADER_INCLUDED
#define FILE_UTILS_HEADER_INCLUDED true

namespace FileUtils
{
		void hash_to_name(const unsigned char* hash, unsigned char* filename);
		void name_to_hash(const unsigned char* name, unsigned char* hash);
		bool compare_hashes(const unsigned char* h1, const unsigned char* h2);
};
#endif
