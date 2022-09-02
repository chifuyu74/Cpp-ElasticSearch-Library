#pragma once
#if _WIN64
#undef DELETE
#endif

namespace Elastic
{
	/*----------------
		HTTPMethod
	----------------*/
	enum class HTTPMethod
	{
		GET,
		POST,
		PUT,
		DELETE,
		HEAD,
	};
}