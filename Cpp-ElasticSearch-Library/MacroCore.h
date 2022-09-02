#pragma once
#include <iostream>
#include <memory>
#include "ElasticError.h"
#include "HttpMethod.h"
#include "ElasticLogger.h"

#if _DEBUG
#include "MacroDebug.h"
#else
#include "MacroRelease.h"
#endif

namespace Elastic
{
	/*---------------------
		std::unique_ptr
	---------------------*/
#define USING_UNIQUE_PTR(name) using name##RefU = std::unique_ptr<class name>;

	USING_UNIQUE_PTR(Logger);
	USING_UNIQUE_PTR(ElasticHost);

	/*----------------
		HTTPMethod
	----------------*/
#define USING_HTTP_METHOD using enum HTTPMethod;


	/*--------------
		LogLevel
	--------------*/
#define USING_LOG_LEVEL using enum LogLevel;

	/*------------------
		ElasticError
	------------------*/
#define USING_ERROR_TYPE using enum Elastic::ElasticErrorIndex;
#define ELASTIC_ERROR(ErrorIndex) ElasticError::Errors[ErrorIndex];
#define USE_ERROR_MAP(key) ElasticError::Errors[key]

#if _DEBUG
#define PRINT_ERROR_MAP(key) PRINT_ERROR_MAP_DEBUG(key)
#else
#define PRINT_ERROR_MAP(key) PRINT_ERROR_MAP_RELEASE(key)
#endif
}