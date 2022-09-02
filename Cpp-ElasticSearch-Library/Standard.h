#pragma once

#define Cpp11 201103L
#define Cpp14 201402L
#define Cpp17 201704L
#define Cpp20 202002L
#define Cpp23 202301L

#if __cplusplus < Cpp20
#error C++ required C++20 or latest
#endif

// C++17
#if __cplusplus >= Cpp17

// Utility
#include <any>
#include <bitset>
#include <chrono>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <initializer_list>
#include <optional>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <utility>
#include <variant>

// Dynamic Memory Management
#include <memory>
#include <memory_resource>
#include <new>
#include <scoped_allocator>

// Numeric Limits
#include <cfloat>
#include <cinttypes>
#include <climits>
#include <cstdint>
#include <limits>

// Error Handling
#include <cassert>
#include <cerrno>
#include <exception>
#include <stdexcept>
#include <system_error>

// String
#include <cctype>
#include <charconv>
#include <cstring>
#include <cuchar>
#include <cwchar>
#include <cwctype>
#include <string>
#include <string_view>

// Container
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Algorithm
#include <algorithm>
#include <execution>

// Iterator
#include <iterator>

// Numerics
#include <cfenv>
#include <cmath>
#include <complex>
#include <numeric>
#include <random>
#include <ratio>
#include <valarray>

// Localization
#include <clocale>
#include <locale>

// Input/Output
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <sstream>
#include <streambuf>
#include <iostream>

// Filesystem
#include <filesystem>

// Regex
#include <regex>

// Atomic Operator
#include <atomic>

// Thread
#include <condition_variable>
#include <future>
#include <mutex>
#include <shared_mutex>
#include <thread>
#endif

// C++20
#if __cplusplus >= Cpp20

// Concept
#include <concepts>

// Coroutine
#include <coroutine>

// Utility
#include <compare>
#include <source_location>
#include <version>

// String
#include <format>
#include <span>

// Numeric
#include <bit>
#include <numbers>

// Input/Output
#include <syncstream>

// Thread
#include <barrier>
#include <latch>
#include <semaphore>
#include <stop_token>
#endif

// C++23
#if __cplusplus > Cpp23

// #include <>
#endif