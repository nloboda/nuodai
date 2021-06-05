/*
 * ConfigManager.h
 *
 *  Created on: 22 Dec,2019
 *      Author: user
 */

#ifndef CONFIGURATION_H_CONFIGURATION_CONFIGMANAGER_H_
#define CONFIGURATION_H_CONFIGURATION_CONFIGMANAGER_H_

#include "Configuration/Config.h"
#include <string>
#include <memory>
namespace config_manager
{
	std::unique_ptr<Config> load(std::string filename);
	std::unique_ptr<Config> make(std::string filename, char* entry);
	void save(Config* c, std::string filename);

}

#endif /* CONFIGURATION_H_CONFIGURATION_CONFIGMANAGER_H_ */
