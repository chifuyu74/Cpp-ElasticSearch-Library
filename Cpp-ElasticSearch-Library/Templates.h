#pragma once
#include "Types.h"

/*---------------
	Concepts
---------------*/

template<typename T, typename... Types>
constexpr bool is_any_of_v = std::disjunction_v<std::is_same<T, Types>...>;

template<typename T, typename... Types>
concept IsNumber = is_any_of_v<T, double, int32, int64, uint32, uint64>;

template<typename T, typename... Types>
concept IsAvailableTypes = is_any_of_v<T, double, int32, int64, uint32, uint64, bool, std::wstring>;


/*---------------
	Functions
---------------*/

using Action = std::function<void(void)>;

template<typename... Args>
using Actions = std::function<void(Args...)>;

template<typename T>
using Func = std::function<T(void)>;

template<typename T, typename... Args>
using Funcs = std::function<T(Args...)>;