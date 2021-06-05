#ifndef CONFIGURATION_READER_H_INCLUDED
#define CONFIGURATION_READER_H_INCLUDED
#include "YdClient/Auth.h"
#include <string>
#include <memory>
#include <vector>
#include "FileUtils/CryptoBlockData.h"
/**
 *  \brief Storage initialisation configuration
 *
 *  This class contains all values needed to initialise storage: API keys, encryption keys IVs etc
 */
class Configuration
{

public:
	Configuration(std::shared_ptr<Auth> auth,
			std::shared_ptr<const char> key,
			std::shared_ptr<const char> iv,
			std::shared_ptr<std::vector<CryptoBlockData>> map,
			std::shared_ptr<std::vector<CryptoBlockData>> fat);

	~Configuration();

	std::shared_ptr<Auth> get_auth();

	std::shared_ptr<const char> get_key();

	std::shared_ptr<const char> get_iv();

	std::shared_ptr<std::vector<CryptoBlockData>> get_map();

	std::shared_ptr<std::vector<CryptoBlockData>> get_fat();

private:
	std::shared_ptr<Auth> auth;

	std::shared_ptr<const char> key;

	std::shared_ptr<const char> iv;

	std::shared_ptr<std::vector<CryptoBlockData>> map;

	std::shared_ptr<std::vector<CryptoBlockData>> fat;

};

namespace config
{
	void save(Configuration* c, std::string filename);
	std::shared_ptr<Configuration> load(std::string filename);
	/**
	 * This is expected to be used during first run to generate random key
	 * @return random configuration
	 */
	std::shared_ptr<Configuration> generate();
};

#endif
