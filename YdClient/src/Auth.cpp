#include "YdClient/Auth.h"
#include <ctime>

#define TIME_SECONDS_IN_HOUR 3600
Auth::Auth()
{
	token = new char[100];
	expires_at = 0;
	refresh_token = new char[100];
}

bool Auth::is_valid()
{
	unsigned long int now = static_cast<unsigned long int>(std::time(nullptr));
	return expires_at > (now + TIME_SECONDS_IN_HOUR);
}

Auth::~Auth()
{
	delete token;
	delete refresh_token;
}
