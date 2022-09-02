#pragma once

namespace Elastic
{
	/*-----------------
		ElasticHost
	-----------------*/
	class ElasticHost
	{
	public:
		ElasticHost() = delete;
		ElasticHost(const std::string& host, const std::string& username, const std::string& password);
		virtual ~ElasticHost();

	public:
		std::string _host;
		std::string _username;
		std::string _password;
	};
}