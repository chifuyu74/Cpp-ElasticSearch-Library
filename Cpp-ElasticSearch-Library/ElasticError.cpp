#pragma once
#include "pch.h"
#include "ElasticError.h"

namespace Elastic
{
	using enum ElasticErrorIndex;

	ErrorType ElasticError::Errors{
		{ Index_Empty, "index cannot be empty" },
		{ Index_Empty_From_Stat, "index or targetIndex cannot be empty" },
		{ Index_Empty_Or_Id_Empty, "index or id cannot be empty." },
		{ Index_StartsWithUnderscore, "Cannot start with -, _, +" },
		{ Index_CannotDot, "Cannot be . or .." },
		{ Index_Oversize, "Cannot be longer than 255 bytes (note it is bytes, so multi-byte characters will count towards the 255 limit faster)" },
		{ Index_NoInclude, "Cannot include \\, /, *, ?, \", <, >, | , ` `(space character), (,) , #" },
		{ Index_NoColon, "Indices prior to 7.0 could contain a colon (:), but that¡¯s been deprecated and won¡¯t be supported in 7.0+" },
		{ Index_OnlyLowercase, "Lowercase only"},
	};
}