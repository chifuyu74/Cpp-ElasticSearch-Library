#include "pch.h"
#include "ElasticLogger.h"

namespace Elastic
{
	/*-----------
		Logger
	-----------*/
	Logger::Logger()
	{

	}

	Logger::~Logger()
	{
#if _DEBUG
		std::cout << "~Logger" << std::endl;
#endif
	}

	void Logger::SetLogFunction(LogCallback extLogFunction) {
		_logFunction = extLogFunction;
	}
}