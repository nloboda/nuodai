#ifndef YD_CLIENT_AUTH_HEADER_INCLUDED
#define YD_CLIENT_AUTH_HEADER_INCLUDED
#include <ctime>
#define AUTH_TOKEN_FIELD_SIZE 100
#define AUTH_TOKEN_REFRESH_FIELD_SIZE 100
/**
 *	\brief Authentication to remote storage service
 *	This contains data needed to authenticate to remote (cloud for hipsters) storage service
 */
class Auth
{
	public:
		Auth();
		~Auth();
		bool is_valid();
		char* token;
		unsigned long int expires_at;
		char* refresh_token;
};
#endif
