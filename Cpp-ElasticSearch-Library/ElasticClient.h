#pragma once
#include <cpr/session.h>
#include <cpprest/json.h>
#include "MacroCore.h"
#include "ElasticLogger.h"
#include "ElasticHost.h"

using namespace web;

namespace Elastic
{
	/*--------------------
		ElasticClient
	--------------------*/
	class ElasticClient
	{
	public:
		ElasticClient() = delete;
		ElasticClient(ElasticClient&& client) = delete;
		ElasticClient& operator=(ElasticClient&& client) = delete;
		ElasticClient& operator=(const ElasticClient& client) = delete;
		ElasticClient(const ElasticClient& client) = delete;

		ElasticClient(const std::string& host, const std::string& username, const std::string& password);
		virtual ~ElasticClient();

	private:
		/*-------------------------------
					Using Macro
			(CAUTION! Name Conflict!)
		-------------------------------*/
		USING_ERROR_TYPE;
		USING_HTTP_METHOD;
		USING_LOG_LEVEL;

	private:
		/*----------
			Log
		----------*/
		wLogCallback LogFunction;

	private:
		/*---------------------------
				Smart Pointer
		---------------------------*/
		ElasticHostRefU _hostInfo;
		LoggerRefU _logger;

		/*---------------------------
				HTTP Request
		---------------------------*/
		cpr::Url _url{};
		cpr::Session _session;
		std::string _bearer;
		std::string _currentMethod;
		std::string _currentUrl;

	public:
		/*------------------
			HelperMethod
		------------------*/
		void SetDefaultIndex(std::string defaultIndex) { _defaultIndex = defaultIndex; }
		void SetIsPretty(bool pretty) { _isPretty = pretty; }
		void SetPrintLog(bool printLog) { _printLog = printLog; }

	private:
		/*----------------------
			Utility Property
		----------------------*/
		std::string _defaultIndex;
		bool _isPretty = true;
		bool _printLog = true;

	private:
		/*--------------------------
				Curl Request
		--------------------------*/
		bool Request(HTTPMethod method, const std::string& path, const std::string& body, cpr::Response& resp);
		bool Get(const std::string& path, const std::string& body, cpr::Response& resp);
		bool Post(const std::string& path, const std::string& body, cpr::Response& resp);
		bool Put(const std::string& path, const std::string& body, cpr::Response& resp);
		bool Delete(const std::string& path, const std::string& body, cpr::Response& resp);
		bool Head(const std::string& path, const std::string& body, cpr::Response& resp);


		/*--------------
			Logging
		--------------*/
		void LogMessage(cpr::Response& resp);


	public:
		/*--------------
			Encoding
		--------------*/
		void EncodeBase64();
		char* EncodeUTF8(std::string str);

	private:

		/*------------------
			JSON Utility
		------------------*/

		enum class JSONNumberType
		{
			DOUBLE,
			INT32,
			INT64,
			UINT32,
			UINT64,
		};

		std::map<std::string, JSONNumberType> typedis
		{
			{"double", JSONNumberType::DOUBLE},
			{"int", JSONNumberType::INT32},
			{"__int64", JSONNumberType::INT64},
			{"unsigned int", JSONNumberType::UINT32},
			{"unsigned __int64", JSONNumberType::UINT32},
		};

		template<typename T>
		T FindNumber(json::number& num) requires IsNumber<T>;

		template<typename T>
		void PushMember(std::vector<T>& vec, json::array& value) requires IsAvailableTypes<T>;

	public:
		/*---------------------------
			JSON Parse/Serialize
		---------------------------*/
		json::value ParseToJSON(std::string rawString, std::error_code errCode);
		json::value ParseToJSON(std::wstring rawString);
		std::wstring SerializeToJSON(const json::value& val);

		/*-----------------------------
			Extract Value From JSON
		-----------------------------*/

		std::wstring GetStringFromJSON(const json::value& jsonObj, const std::wstring& key);
		json::object GetObjectFromJSON(const json::value& jsonObj, const std::wstring& key);
		bool GetBoolFromJSON(const json::value& jsonObj, std::wstring& key);

		template<typename T>
		std::vector<T> GetArrayFromJSON(json::value& jsonObj, std::wstring& key) requires IsAvailableTypes<T>;

		template<typename T>
		T GetNumberFromJSON(json::value& jsonObj, std::wstring& key) requires IsNumber<T>;

		/*------------------
			Split String
		------------------*/
		std::vector<std::string> Split(std::string input, char delimiter);
		std::vector<std::wstring> SplitW(std::wstring input, wchar_t delimiter);
		
		/*--------------------------
			Read/Write JSON File
		--------------------------*/

		void WriteJsonToFile(std::string dest, std::string content, std::ofstream::openmode openmode = std::ofstream::out, uint64 loopCnt = 1);
		void WriteJsonToFile(std::wstring dest, std::wstring content, std::ofstream::openmode openmode = std::ofstream::out, uint64 loopCnt = 1);
		std::stringstream ReadJsonToFile(std::string dest, std::ifstream::openmode openmode = std::ifstream::in);
		std::wstringstream ReadJsonToFile(std::wstring dest, std::wifstream::openmode openmode = std::wifstream::in);

	public:
		/*-----------------------
			ElasticSearch API
		-----------------------*/

		/*----------------
			Cluster API
		----------------*/
		void ClusterHealth(cpr::Response& resp, std::string target, std::string query);

		/*----------------
			Index API
		----------------*/
		void CreateIndex(cpr::Response& resp, std::string index);
		void Stat(cpr::Response& resp, std::string index);
		// Cannot recovery, are you sure?
		void DeleteIndex(cpr::Response& resp, std::string index);
		void ExistsIndex(cpr::Response& resp, std::string target);
		void CloneIndex(cpr::Response& resp, std::string index, std::string targetIndex, bool isIndexBlocksWrite = false);

		/*------------------
			Document API
		------------------*/
		void InsertDocument(cpr::Response& resp, std::string body, std::string index);
		void UpdateDocument(cpr::Response& resp, std::string body, std::string index, std::string id);
		void DeleteDocument(cpr::Response& resp, std::string index, std::string id);
		void Bulk(cpr::Response& resp, std::string body);
		void Bulk(cpr::Response& resp, std::string body, std::string index);

		/*----------------
			Search API
		----------------*/
		void DefaultSearch(cpr::Response& resp, std::string index, std::string body = "");
		void Search(cpr::Response& resp, std::string body = "");
		void Search(cpr::Response& resp, std::string index, std::string body = "");
		void Count(cpr::Response& resp);
		void Count(cpr::Response& resp, std::string index);

		// returns : id (wstring)
		std::wstring SearchAfter(cpr::Response& resp, std::string index, std::string body = "", int64 keepAliveTime = 1);
		void ClearScroll(cpr::Response& resp, std::string scrollId);
	};


	template<typename T>
	inline void ElasticClient::PushMember(std::vector<T>& vec, json::array& value) requires IsAvailableTypes<T>
	{
		

		for (json::value ar : value)
		{
			if (ar.is_boolean())
			{
				bool val = ar.as_bool();
				vec.push_back(val);
				continue;
			}

			if (ar.is_string())
			{
				std::wstring str = ar.as_string();
				vec.push_back(str);
				continue;
			}

			// TODO
			if (ar.is_object())
			{

			}

			if (ar.is_array())
			{

			}

			if (ar.is_number())
			{
				const char* typeName = typeid(T).name();
				std::string typeOfT(typeName);
				JSONNumberType typemap = typedis[typeOfT];
				json::number num = ar.as_number();
#pragma warning(disable : 4244)
				switch (typemap)
				{
				case JSONNumberType::DOUBLE:
					vec.push_back(num.to_double());
					break;
				case JSONNumberType::INT32:
					vec.push_back(num.to_int32());
					break;
				case JSONNumberType::INT64:
					vec.push_back(num.to_int64());
					break;
				case JSONNumberType::UINT32:
					vec.push_back(num.to_uint32());
					break;
				case JSONNumberType::UINT64:
					vec.push_back(num.to_uint64());
					break;
				default:
					break;
				}
			}
		}
	}

	template<typename T>
	std::vector<T> ElasticClient::GetArrayFromJSON(json::value& jsonObj, std::wstring& key) requires IsAvailableTypes<T>
	{
		std::vector<T> vec;

		if (jsonObj.is_object() && jsonObj.has_field(key) && jsonObj.has_array_field(key))
		{
			json::object obj = jsonObj.as_object();
			json::array value = obj.at(key).as_array();
			PushMember<T>(vec, value);
			return vec;
		}

		if (jsonObj.is_array())
		{
			json::array value = jsonObj.as_array();
			PushMember<T>(vec, value);
			return vec;
		}

		if (_printLog)
		{
			std::cout << "jsonObj is not object or array" << std::endl;
		}

		return std::vector<T>();
	}

	template<typename T>
	inline T ElasticClient::FindNumber(json::number& num) requires IsNumber<T>
	{
		const char* typeName = typeid(T).name();
		std::string typeOfT(typeName);
		JSONNumberType typemap = typedis[typeOfT];
		switch (typemap)
		{
		case JSONNumberType::DOUBLE:
			return num.to_double();
		case JSONNumberType::INT32:
			if (num.is_int32())
				return num.to_int32();
			break;
		case JSONNumberType::INT64:
			if (num.is_int64())
				return num.to_int64();
			break;
		case JSONNumberType::UINT32:
			if (num.is_uint32())
				return num.to_uint32();
			break;
		case JSONNumberType::UINT64:
			if (num.is_uint64())
				return num.to_uint64();
		default:
			break;
		}
		return T();
	}

	template<typename T>
	inline T ElasticClient::GetNumberFromJSON(json::value& jsonObj, std::wstring& key) requires IsNumber<T>
	{
		if (jsonObj.is_object())
		{
			if (jsonObj.has_number_field(key) && jsonObj.at(key).is_number())
			{
				json::number num = jsonObj.at(key).as_number();
				return FindNumber(num);
			}
		}

		if (jsonObj.is_number())
		{
			json::number num = jsonObj.as_number();
			return FindNumber(num);
		}

		throw std::exception("Not Found number from JSON");
	}
}