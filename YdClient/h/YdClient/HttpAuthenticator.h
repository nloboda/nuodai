#ifndef HTTP_CLIENT_AUTHENTICATOR_HEADER_INCLUDED
#define HTTP_CLIENT_AUTHENTICATOR_HEADER_INCLUDED true
#include <curl/curl.h>

class HttpAuthenticator
{
	public:
        void authenticate(struct curl_slist *list);
};

#endif
