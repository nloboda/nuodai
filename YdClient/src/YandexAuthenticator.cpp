#include <iostream>
#include "YdClient/Auth.h"
#include <YdClient/YandexAuthenticator.h>
#include <curl/curl.h>
#include <string.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <ctime>
#include <stdexcept>

#define YANDEX_TOKEN_REQUEST_URL "https://oauth.yandex.com/authorize?response_type=code&client_id="
#define YANDEX_APP_CLIENT_ID "5aa227f80b2349218d167ede8087cb56"
#define YANDEX_AUTH_POST_DATA "grant_type=authorization_code&client_id=5aa227f80b2349218d167ede8087cb56&client_secret=336be21030ce44abbb725b81170f1041&code="
#define YANDEX_TOKEN_POST_URL "https://oauth.yandex.com/token"


//use this https://github.com/open-source-parsers/jsoncpp
YandexAuthenticator::YandexAuthenticator()
{
	
};

YandexAuthenticator::~YandexAuthenticator()
{
	
};



static size_t write_callback(void *contents, size_t size, size_t nmemb, void *ptr)
{
	Json::Reader reader;
	Json::Value root;
	if(!reader.parse((char*)contents, root)) throw std::runtime_error("Unable to parse authentication response");
	Auth* auth = (Auth*)ptr;
	std::string access_token = root["access_token"].asString();
	int expires_in = root["expires_in"].asInt();
	std::string refresh_token = root["refresh_token"].asString();
	strcpy(auth->token, access_token.c_str());
	auth->expires_at = expires_in + std::time(nullptr);
	strcpy(auth->refresh_token, refresh_token.c_str());
	std::time_t time = std::time(0);
	return nmemb;
}

static const char* create_post_data_from_code(long code)
{
	const char* data = YANDEX_AUTH_POST_DATA;
	const char* s_code = std::to_string(code).c_str();
	int dest_size = strlen(data) + strlen(s_code) + 1;
	char* dest = new char[dest_size];
	strcpy(dest, data);
	strcat(dest, s_code);
	return dest;
}

void YandexAuthenticator::read_configuration(char* data)
{
	unsigned long int* expires_in = reinterpret_cast<unsigned long int*>(data);
	char* token = data + sizeof(unsigned long int);
	char* refresh_token = token + 42;

	this->auth.expires_at = *expires_in;
	strcpy(this->auth.refresh_token, refresh_token);
	strcpy(this->auth.token, token);
	throw std::runtime_error("not implemented");
}

void YandexAuthenticator::write_configuration(char* c)
{

}

bool YandexAuthenticator::authenticate()
{
	struct curl_slist *list = nullptr;
	std::cout << "Please visit the following url " << std::endl;
	std::cout << YANDEX_TOKEN_REQUEST_URL << YANDEX_APP_CLIENT_ID << std::endl;
	//TODO: implement token refresh
	long code;
	CURL *curl = curl_easy_init();
	if(curl) {
		CURLcode res;
		std::cin >> code;
		const char* data = create_post_data_from_code(code);
		curl_easy_setopt(curl, CURLOPT_URL, YANDEX_TOKEN_POST_URL);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(data));
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &(this->auth));
		res = curl_easy_perform(curl);
		if(res != CURLE_OK) throw std::runtime_error("Authentication has failed");
		curl_easy_cleanup(curl);
		delete data;
		
	}
	return true;
};


struct curl_slist* YandexAuthenticator::header()
{
	if(!this->auth.is_valid()) authenticate();
	char auth_header[100];
	strcpy(auth_header, "Authorization: OAuth ");
	strcpy(auth_header + 21, auth.token);
	std::cout << auth_header << std::endl;
	return curl_slist_append(nullptr, auth_header);
}
