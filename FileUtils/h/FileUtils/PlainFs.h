#ifndef PLAIN_FILESYSTEM_HEADER_INCLUDED
#define PLAIN_FILESYSTEM_HEADER_INCLUDED true
#include <string>

namespace plainfs
{
	extern const int BLOCK_SIZE;
}


class PlainFs
{
	public:
		PlainFs(const std::string working_directory);
		~PlainFs();
		void read_plain(const char* block, char* buffer);
		void write_plain(const char* buffer, char* block);
	private:
		bool verify_hash(const unsigned char* block, const char* buffer);
};

#endif
