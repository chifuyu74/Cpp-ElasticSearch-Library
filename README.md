# Cpp-ElasticSearch-Library


- ElasticSearch Library in written C++20 on Windows.




### Comments

- I tried to write it according to the standard as much as possible.






### Requirements

- x64 (Not x86)


- C++20


- vcpkg


- cpr (1.8.3)


- cpprestsdk (2.10.18#2)


- boost v1.79.0 (#include \<boost/beast/core/detail/base64.hpp\>)


- Need your ElasticSearch account { endpoint, username, password }... ([ElasticSearch Link](https://elastic.co))




### Install

- vcpkg install cpr:x64-windows cpprestsdk:x64-windows boost:x64-windows


- Build (x64)


- include Elastic.lib


### Operating System Environment


- I haven't checked if it works on Linux / MacOS.


- Windows 11 (22000.795) x64


- Visual Studio 2022


- vcpkg