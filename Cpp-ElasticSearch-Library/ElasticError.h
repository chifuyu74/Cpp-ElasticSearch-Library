#pragma once
#include <map>
#include <string>

namespace Elastic
{
	enum class ElasticErrorIndex
	{
		/*-----------
			Index
		-----------*/
		Index_Empty,
		Index_Empty_From_Stat,
		Index_Empty_Or_Id_Empty,
		Index_StartsWithUnderscore,
		Index_CannotDot,
		Index_Oversize,
		Index_NoInclude,
		Index_NoColon,
		Index_OnlyLowercase,
	};

	using ErrorType = std::map<ElasticErrorIndex, const char*>;
	class ElasticError
	{
	public:
		
		static ErrorType Errors;
	};
}