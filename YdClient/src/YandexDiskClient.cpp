#include "YdClient/YandexDiskClient.h"
#include "YdClient/YandexAuthenticator.h"
#include "FileUtils/FileUtils.h"
#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include <string.h>
#include <json/value.h>
#include <json/json.h>
#include <stdexcept>
#include "FileUtils/PlainFs.h"

#define YANDEX_UPLOAD_PREFIX "https://cloud-api.yandex.net/v1/disk/resources/upload?path="
#define YANDEX_DOWNLOAD_PREFIX "https://cloud-api.yandex.net/v1/disk/resources/download?path="

YandexDiskClient::YandexDiskClient(YandexAuthenticator* auth, PlainFs* fs) :
	authenticator(auth),
	filesystem(fs)
{

};

YandexDiskClient::~YandexDiskClient()
{
	delete this->authenticator;
}

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *ptr)
{
	Json::Reader reader;
	Json::Value root;
	if(!reader.parse((char*)contents, root)) throw std::runtime_error("We can't parse server response");
	strcpy((char *)ptr, root["href"].asString().c_str());

	return nmemb;
}

static std::string get_push_url(YandexAuthenticator* auth, unsigned const char* hash)
{
	const int prefix_length = strlen(YANDEX_UPLOAD_PREFIX);
	unsigned char filename[prefix_length + 65];
	strcpy((char*)filename, YANDEX_UPLOAD_PREFIX);
	
	filename[prefix_length + 64] = '\0';
	FileUtils::hash_to_name(hash, filename + prefix_length);

	CURL *curl = curl_easy_init();
	if(!curl) throw new std::runtime_error("Push url fetch failed");

	char buffer[1024];
	curl_easy_setopt(curl, CURLOPT_URL, filename);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, auth->header());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	CURLcode res = curl_easy_perform(curl);
	std::string result;
	result.assign(buffer);
	return result;
}

static std::string get_pull_url(YandexAuthenticator* auth, unsigned const char* hash)
{
	const int prefix_length = strlen(YANDEX_DOWNLOAD_PREFIX);
	unsigned char filename[prefix_length + 65];
	strcpy((char*)filename, YANDEX_DOWNLOAD_PREFIX);

	filename[prefix_length + 64] = '\0';
	FileUtils::hash_to_name(hash, filename + prefix_length);

	CURL *curl = curl_easy_init();
	if(!curl) throw std::runtime_error("Pull url fetch failed");

	char buffer[1024];
	curl_easy_setopt(curl, CURLOPT_URL, filename);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, auth->header());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	CURLcode res = curl_easy_perform(curl);
	std::string result;
	result.assign(buffer);
	return result;
}



static size_t read_callback(char *buffer, size_t size, size_t nitems, void *userdata)
{
	memcpy(buffer, userdata, plainfs::BLOCK_SIZE);
	return plainfs::BLOCK_SIZE;
}

static size_t file_download_callback(void *contents, size_t size, size_t nmemb, void *ptr)
{
	if(size!=plainfs::BLOCK_SIZE) throw std::runtime_error("Downloaded chunk has different size!");
	memcpy(ptr, contents, plainfs::BLOCK_SIZE);
	return plainfs::BLOCK_SIZE;
}

void YandexDiskClient::pull(unsigned const char* hash)
{
	std::string pull_url = get_pull_url(authenticator, hash);
	std::cout << pull_url << std::endl;
	CURL *curl = curl_easy_init();
	if(!curl) throw std::runtime_error("curl_easy_init failed");
	char buffer[plainfs::BLOCK_SIZE];
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, file_download_callback);
	curl_easy_setopt(curl, CURLOPT_URL, pull_url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
	int res = curl_easy_perform(curl);
	if(res != CURLE_OK) throw std::runtime_error("pull attempt failed");
	char received_hash[32];
	filesystem->write_plain(buffer, received_hash);
	FileUtils::compare_hashes(hash, (unsigned char*)received_hash);
}

void YandexDiskClient::push(unsigned const char* hash)
{
	std::string push_url = get_push_url(authenticator, hash);
	std::cout << push_url << std::endl;
	CURL *curl = curl_easy_init();
	if(!curl) throw std::runtime_error("curl_easy_init failed");
	char buffer[plainfs::BLOCK_SIZE];
	filesystem->read_plain((const char*)hash,buffer);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
	curl_easy_setopt(curl, CURLOPT_URL, push_url.c_str());
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(curl, CURLOPT_PUT, 1);
	curl_easy_setopt(curl, CURLOPT_READDATA, buffer);
	curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, plainfs::BLOCK_SIZE);
	int res = curl_easy_perform(curl);
	if(res!=CURLE_OK) throw std::runtime_error("push attempt failed");
}
