#include "pch.h"
#include "ElasticHost.h"

namespace Elastic
{
	/*-----------------
		ElasticHost
	-----------------*/

	ElasticHost::ElasticHost(const std::string& host, const std::string& username, const std::string& password)
	{
		_host = host.ends_with("/") == false ? host + "/" : host;
		_username = username;
		_password = password;
	}

	ElasticHost::~ElasticHost()
	{
#if _DEBUG
		std::cout << "~ElasticHost" << std::endl;
#endif
	}
}