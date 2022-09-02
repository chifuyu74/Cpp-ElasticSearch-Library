#pragma once

namespace Elastic
{
	#define PRINT_ERROR_MAP_DEBUG(key) throw std::runtime_error(ElasticError::Errors[key]);
}