#ifndef YD_CLIENT_AUTHENTICATOR_HEADER_INCLUDED
#define YD_CLIENT_AUTHENTICATOR_HEADER_INCLUDED true
#include "YdClient/Auth.h"
#include "YdClient/HttpAuthenticator.h"
#include <curl/curl.h>
#include <Configuration/Configuration.h>


class YandexAuthenticator: public HttpAuthenticator
{
	public:
	YandexAuthenticator();
	~YandexAuthenticator();
	void read_configuration(Configuration* config);
	void write_configuration(Configuration* config);
	bool authenticate();
	struct curl_slist* header();
	private:
		Auth auth;
};

#endif
