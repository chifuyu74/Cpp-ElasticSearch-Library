### Naming Convention

- C++ Version

```cpp
// Must be include the Standrd Library
// header according to C++ Version.

// The minimum version is C++20.

#define Cpp17 201704L
#define Cpp20 202002L

#if __cplusplus < Cpp20
#error C++ required C++20 or latest
#endif

// C++17
#if __cplusplus >= Cpp17

// Utility
#include <any>
#include <bitset>
#include <chrono>
#endif
```

- Macro, enum

```cpp
#define USE_XXX Macro; // Uppercase + _
```

<br />

- Function, Class, Namespace, FileName

```cpp
// Only Pascal case
// Except : main.cpp, pch.h, pch.cpp
namespace Elastic
{
    class ElasticClient
    {
    public:
        void Print();
    }
}
```

<br />

- Variable, Parameter

```cpp
// Camel case
int helloWorld = 1;
void Print(const std::string& msg)
{
    std::cout << msg << std::endl;
}
```

<br />

- Standard Library

```cpp
// No "using namespace std;"
// prevent name conflict
{
    // Bad
    using namespace std;
    cout << "Hello World!" << endl;
}

{
    std::cout << "Hello World!" << std::endl;
}

```

<br />

### Getting Started

```cpp
#pragma once
#include <string>
#include <cpr/cpr.h>
#include <ElasticClient.h>

// Be set your project's C++ version to C++20

using uint64 = unsigned __int64;

int main()
{
    // Define your account info (https://elastic.co). ElasticSearch's endpoint, username, password
    std::string host{ "your-endpoint" };
    std::string username{ "your-username" };
    std::string password{ "your-password" };

    // Create ElasticClient instance.
    Elastic::ElasticClient client{ host, username, password };

    // All future responses will be received with this variable.
    cpr::Response resp;


    // Set Default Index (optional)
    // Not yet applied for all methods.
    std::string defaultIndex{ "default-index" };
    client.SetDefaultIndex(defaultIndex);

    // Search API example
    // DefaultSearch is basically size 1000.
    client.DefaultSearch(resp, defaultIndex);

    // The body string must be in json format.
    // Examples are as follows.
    // => It means that it must be a string that can be parsed in json format.
    uint64 size = 2000;
    std::string searchBody = std::format("{{ \"size\" : {} }}", size);
    // searchBody => { size : 2000 }
    client.Search(resp, defaultIdx, body);

    // status code after request
    if (resp.status_code == 200)
    {
        std::cout << "succeed" << std::endl;
        // The results of the response are in resp.text.
        std::cout << resp.text << std::endl;
    }


    // Please, See the documentation for other APIs.
    // ...
}
```

<br />

- All APIs belong to the same namespace. (Elastic)

<br />

### [Class] Logger API

- Log Level Enum

```cpp
enum class LogLevel {
    FATAL = 0,
    ERROR = 1,
    WARNING = 2,
    INFO = 3,
    DEBUG = 4
};
```

<br />

- Logging Function Type

```cpp
using LogCallback = std::function<void(LogLevel, const std::string&)>;

using wLogCallback = std::function<void(LogLevel, const std::wstring&)>;

```

<br />

- Return log function (LogCallback type / wLogCallback type)

```cpp
LogCallback GetLogFunction();
wLogCallback GetWLogFunction();
```

<br />

- Set log function (LogCallback type / wLogCallback type)

```cpp
void SetLogFunction(LogCallback func);
```

<br />

### [Enum] HTTPMethod

```cpp
enum class HTTPMethod
{
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
};
```

<br />

### [Class] ElasticClient API

- If you try to force private members to set them up, you may encounter errors, so do not set them up.

<br />

- Index API

```cpp
// Set Default Index (optional)
// Not yet applied for all methods.
void SetDefaultIndex(std::string defaultIndex)

// Create new index
void CreateIndex(cpr::Response& resp, std::string index);

// check index's stat
void Stat(cpr::Response& resp, std::string index);

// clone from index to targetIndex.
// if isIndexBlockWrite is true, set current index of settings. (index.blocks.write : true)
void CloneIndex(cpr::Response& resp, std::string index, std::string targetIndex, bool isIndexBlocksWrite = false);



```

<br />

- Document API

```cpp
// Insert a new document into the index.
void InsertDocument(cpr::Response& resp, std::string body, std::string index);

// Update documnet that exists in index.
void UpdateDocument(cpr::Response& resp, std::string body, std::string index, std::string id);

// Delete document that exists in index.
void DeleteDocument(cpr::Response& resp, std::string index, std::string id);

// Bulk
void Bulk(cpr::Response& resp, std::string body);
void Bulk(cpr::Response& resp, std::string body, std::string index);
```
