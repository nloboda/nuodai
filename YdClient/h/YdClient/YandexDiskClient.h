#ifndef YD_CLIENT_HEADER_INCLUDED
#define YD_CLIENT_HEADER_INCLUDED true
#include "YdClient/YandexAuthenticator.h"
#include "FileUtils/PlainFs.h"

class YandexDiskClient
{
	public:
		YandexDiskClient(YandexAuthenticator* authenticator, PlainFs* filesystem);
		YandexDiskClient(const YandexDiskClient &client) = delete;
		YandexDiskClient& operator=(const YandexDiskClient &client) = delete;
		YandexDiskClient(YandexDiskClient &&client);
		YandexDiskClient& operator=(const YandexDiskClient &&client);
		~YandexDiskClient();
		void pull(unsigned const char* hash);
		void push(unsigned const char* hash);
	private:
		YandexAuthenticator* authenticator;
		PlainFs* filesystem;
};

#endif
