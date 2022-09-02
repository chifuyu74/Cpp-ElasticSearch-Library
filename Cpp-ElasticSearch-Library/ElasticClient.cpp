#pragma once
#include "pch.h"
#include "ElasticClient.h"
#include "ElasticLogger.h"
#include "ElasticHost.h"
#include <boost/beast/core/detail/base64.hpp>
#include "MacroCore.h"

using namespace web;

namespace Elastic
{
	/*--------------------
		ElasticClient
	--------------------*/
	ElasticClient::ElasticClient(const std::string& host, const std::string& username, const std::string& password)
	{
		_hostInfo = std::make_unique<ElasticHost>(host, username, password);
		_logger = std::make_unique<Logger>();
		EncodeBase64();
		_url = cpr::Url{ _hostInfo->_host };
		LogFunction = _logger->GetWLogFunction();
	}

	ElasticClient::~ElasticClient()
	{
		// Reconsider if memory leaks are dangerous
		_hostInfo = nullptr;
		_logger = nullptr;
#if _DEBUG
		std::cout << "~ElasticClient" << std::endl;
#endif
	}

	/*--------------------------
			Curl Request
	--------------------------*/

	bool ElasticClient::Request(HTTPMethod method, const std::string& path, const std::string& body, cpr::Response& resp)
	{
		_currentUrl = _url.str() + path;

		if (_isPretty)
		{
			_currentUrl += _currentUrl.find('?') != std::string::npos ? "&pretty" : "?pretty";
		}

		_session.SetUrl(_currentUrl);

		cpr::Header header;
		if (header.contains("Authorization") == false)
		{
			header.insert(header.end(), std::make_pair<std::string, std::string>("Authorization", std::format("Basic {}", _bearer)));
		}


		header["Content-Type"] = "application/json; charset=utf-8";

		if (!body.empty())
		{
			header["Content-Type"] = "application/json; charset=utf-8";
		}
		_session.SetHeader(header);

		_session.SetBody(cpr::Body(body));

		switch (method)
		{
		case GET:
			_currentMethod = "GET";
			resp = _session.Get();
			break;
		case POST:
			_currentMethod = "POST";
			resp = _session.Post();
			break;
		case PUT:
			_currentMethod = "PUT";
			resp = _session.Put();
			break;
		case DELETE:
			_currentMethod = "DELETE";
			resp = _session.Delete();
			break;
		case HEAD:
			_currentMethod = "HEAD";
			resp = _session.Head();
			break;
		default:
			break;
		}

		if (_printLog)
		{
			LogMessage(std::ref(resp));
		}

		// Return false if current node failed for request from following reasons.
		// Status code = 0 means, that it is not possible to connect to Elastic node.
		// Status code = 503 means that Elastic node is temporarily unavailable, maybe because of queue
		// capacity of node is full filled.

		if (resp.status_code == 0 || resp.status_code == 503) {
			LogFunction(LogLevel::WARNING, _logger->ConvertWstring(std::format("Host on URL \'{}\' is unavailable.", _currentUrl.c_str())));
			return false;
		}
		return true;
	}

	bool ElasticClient::Get(const std::string& path, const std::string& body, cpr::Response& resp)
	{
		return Request(HTTPMethod::GET, path, body, resp);
	}

	bool ElasticClient::Post(const std::string& path, const std::string& body, cpr::Response& resp)
	{
		return Request(HTTPMethod::POST, path, body, resp);
	}

	bool ElasticClient::Put(const std::string& path, const std::string& body, cpr::Response& resp)
	{
		return Request(HTTPMethod::PUT, path, body, resp);
	}

	bool ElasticClient::Delete(const std::string& path, const std::string& body, cpr::Response& resp)
	{
		return Request(HTTPMethod::DELETE, path, body, resp);
	}

	bool ElasticClient::Head(const std::string& path, const std::string& body, cpr::Response& resp)
	{
		return Request(HTTPMethod::HEAD, path, body, resp);
	}

	/*--------------
		Logging
	--------------*/

	void ElasticClient::LogMessage(cpr::Response& resp)
	{
#if _DEBUG
		std::cout << std::endl;
#endif
		LogFunction(INFO, _logger->ConvertWstring(std::format("Request Method / URL : [{}] {}", _currentMethod, _currentUrl)));

		LogFunction(INFO, _logger->ConvertWstring(std::format("Status Code : [{}], Elapsed time {}s", resp.status_code, resp.elapsed)));

		LogFunction(DEBUG, _logger->ConvertWstring(std::format("Response Size : [{}], Response text: {}", resp.text.size(), resp.text)));

		if (resp.error) {
			LogFunction(WARNING, _logger->ConvertWstring(std::format("Request error: {}", resp.error.message)));
		}
	}

	/*--------------
		Encoding
	--------------*/

	void ElasticClient::EncodeBase64()
	{
		namespace base64 = boost::beast::detail::base64;
		std::string prev = format("{}:{}", _hostInfo->_username, _hostInfo->_password);

		std::vector<char> input(prev.begin(), prev.end());

		std::string encoded(base64::encoded_size(input.size()), '\0');
		boost::beast::detail::base64::encode(encoded.data(), input.data(), input.size());
		_bearer = std::string(encoded.begin(), encoded.end());
	}

	// TODO : memory return
	char* ElasticClient::EncodeUTF8(std::string str)
	{
#pragma warning(push)
#pragma warning(disable : 4267)
#pragma warning(disable : 4172)
		std::wstring wstr(str.begin(), str.end());
		const int32 max_size = (1LL << 16) - 1;
		wchar_t strUnicode[max_size] = {};
		char strUTF8[max_size] = {};
		strcpy_s(strUTF8, max_size, str.c_str());
		int nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8, strlen(strUTF8), NULL, NULL);
		MultiByteToWideChar(CP_UTF8, 0, strUTF8, strlen(strUTF8), strUnicode, nLen);
		return strUTF8;
#pragma warning(pop)
	}

	/*---------------------------
		JSON Parse/Serialize
	---------------------------*/

	json::value ElasticClient::ParseToJSON(std::string rawString, std::error_code errCode)
	{
		try
		{
			json::value val = json::value::parse(rawString, errCode);
			if (errCode.value() != 0)
				throw std::exception("JSON Parsing Error, Check your raw string.");

			return val;
		}
		catch (const std::exception& e)
		{
			std::string err = std::format("{}", e.what());
			std::wstring werr(err.begin(), err.end());
			LogFunction(LogLevel::ERROR, werr);
			return NULL;
		}
	}

	json::value ElasticClient::ParseToJSON(std::wstring rawString)
	{
		try
		{
			std::wstringstream wss;
			wss << rawString;
			json::value val = json::value::parse(wss);

			return val;
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			std::string err = std::format("{}", "JSON Parsing Error, Check your raw string.");
			std::wstring werr(err.begin(), err.end());
			LogFunction(LogLevel::ERROR, werr);
		}
	}

	std::wstring ElasticClient::SerializeToJSON(const json::value& val)
	{
		std::wstringstream serializeStream;
		val.serialize(serializeStream);
		return serializeStream.str();
	}

	std::wstring ElasticClient::GetStringFromJSON(const json::value& jsonObj, const std::wstring& key)
	{
		if (jsonObj.is_string())
		{
			return jsonObj.as_string();
		}

		if (jsonObj.is_object() && jsonObj.has_string_field(key))
		{
			std::wstring str = jsonObj.as_object().at(key).as_string();
			return str;
		}

		return L"";
	}

	json::object ElasticClient::GetObjectFromJSON(const json::value& jsonObj, const std::wstring& key)
	{
		if (jsonObj.is_object())
		{
			if (jsonObj.has_object_field(key) && jsonObj.at(key).is_object())
				return jsonObj.at(key).as_object();

			return jsonObj.as_object();
		}
		return jsonObj.as_object();
	}

	bool ElasticClient::GetBoolFromJSON(const json::value& jsonObj, std::wstring& key)
	{
		if (jsonObj.is_boolean())
			return jsonObj.as_bool();

		if (jsonObj.is_object())
		{
			if (jsonObj.has_boolean_field(key) && jsonObj.at(key).is_boolean())
				return jsonObj.at(key).as_bool();
		}

		throw std::exception("Not Found bool from JSON");
	}

	/*------------------
		Split String
	------------------*/

	std::vector<std::string> ElasticClient::Split(std::string input, char delimiter)
	{
		std::vector<std::string> splitted;
		std::stringstream ss;
		std::string temp;

		while (std::getline(ss, temp, delimiter))
		{
			splitted.push_back(temp);
		}

		return splitted;
	}

	std::vector<std::wstring> ElasticClient::SplitW(std::wstring input, wchar_t delimiter)
	{
		std::vector<std::wstring> splitted;
		std::wistringstream wss(input);
		std::wstring temp;

		while (std::getline(wss, temp, delimiter))
		{
			splitted.push_back(temp);
		}

		return splitted;
	}

	/*--------------------------
		Read/Write JSON File
	--------------------------*/

	void ElasticClient::WriteJsonToFile(std::string dest, std::string content, std::ofstream::openmode openmode, uint64 loopCnt)
	{
		std::ofstream file(dest, openmode);

		if (file.is_open())
		{
			for (int i = 0; i < loopCnt; i++)
			{
				file << content;
			}
		}
		file.close();
	}

	void ElasticClient::WriteJsonToFile(std::wstring dest, std::wstring content, std::ofstream::openmode openmode, uint64 loopCnt)
	{
		std::wofstream file(dest, openmode);

		if (file.is_open())
		{
			for (int i = 0; i < loopCnt; i++)
			{
				file << content;
			}
		}
		file.close();
	}

	std::stringstream ElasticClient::ReadJsonToFile(std::string dest, std::ifstream::openmode openmode)
	{
		std::ifstream file(dest, openmode);
		std::stringstream ss;

		if (file.is_open())
		{
			ss << file.rdbuf();
		}
		file.close();
		return ss;
	}

	std::wstringstream ElasticClient::ReadJsonToFile(std::wstring dest, std::wifstream::openmode openmode)
	{
		std::wifstream file(dest, openmode);
		std::wstringstream ss;

		if (file.is_open())
		{
			ss << file.rdbuf();
		}
		file.close();
		return ss;
	}

	/*----------------
		Cluster API
	----------------*/
	
	void ElasticClient::ClusterHealth(cpr::Response& resp, std::string target, std::string query)
	{
		std::string reqPath = std::format("_cluster/health/{}?{}", target, query);
		Get(reqPath, "", std::ref(resp));
	}

	/*----------------
		Index API
	----------------*/

	void ElasticClient::CreateIndex(cpr::Response& resp, std::string index)
	{
		if (index.empty())
		{
			PRINT_ERROR_MAP(Index_Empty)
			return;
		}

		if (index.starts_with('-') || index.starts_with('_') || index.starts_with('+'))
		{
			PRINT_ERROR_MAP(Index_StartsWithUnderscore)
			return;
		}

		if (index == "." || index == "..")
		{
			PRINT_ERROR_MAP(Index_CannotDot)
			return;
		}

		if (index.size() > 255)
		{
			PRINT_ERROR_MAP(Index_Oversize)
			return;
		}

		std::regex re1(
			"[\\\\/\\*\?\"\\<\\>\\s\\,\\#]+",
			std::regex_constants::ECMAScript
		);
		std::smatch match1;

		if (std::regex_search(index, match1, re1, std::regex_constants::match_any))
		{
			PRINT_ERROR_MAP(Index_NoInclude)
			return;
		}

		std::regex re2("[\\:]+");
		std::smatch match2;
		if (std::regex_search(index, match2, re2, std::regex_constants::match_any))
		{
			PRINT_ERROR_MAP(Index_NoColon)
			return;
		}

		for (auto& str : index)
		{
			int ascii = static_cast<int>(str);
			if (ascii >= 65 && ascii <= 90)
			{
				PRINT_ERROR_MAP(Index_OnlyLowercase)
				return;
			}
		}

		std::string reqPath = std::format("{}", index);
		Put(reqPath, "", std::ref(resp));
	}

	void ElasticClient::Stat(cpr::Response& resp, std::string index)
	{
		if (index.empty())
		{
			PRINT_ERROR_MAP(Index_Empty)
			return;
		}

		std::string reqPath = std::format("{}/_stats", index);
		Get(reqPath, "", std::ref(resp));

	}

	void ElasticClient::DeleteIndex(cpr::Response& resp, std::string index)
	{
		std::string reqPath = std::format("{}", index);
		Delete(reqPath, "", std::ref(resp));
	}

	void ElasticClient::ExistsIndex(cpr::Response& resp, std::string target)
	{
		std::string reqPath = std::format("{}", target);
		Head(reqPath, "", std::ref(resp));
	}

	void ElasticClient::CloneIndex(cpr::Response& resp, std::string index, std::string targetIndex, bool isIndexBlocksWrite)
	{
		if (index.empty() || targetIndex.empty())
		{
			PRINT_ERROR_MAP(Index_Empty_From_Stat)
			return;
		}

		if (isIndexBlocksWrite)
		{
			std::function<void()> ss = [index, &resp, this]() {
				std::string preReqPath = std::format("{}/_settings", index);
				std::string body = std::format("{{\"settings\": {{\"index.blocks.write\": {}}}}}", true);
				Put(preReqPath, body, resp);
			};

			std::future<void> k = std::async(std::launch::async, ss);
			k.get();
		}

		std::string reqPath = std::format("{}/_clone/{}", index, targetIndex);
		Post(reqPath, "", std::ref(resp));
	}

	/*------------------
		Document API
	------------------*/

	void ElasticClient::InsertDocument(cpr::Response& resp, std::string body, std::string index)
	{
		if (index.empty())
		{
			PRINT_ERROR_MAP(Index_Empty)
			return;
		}

		std::string reqPath = std::format("{}/_doc", index);
		Post(reqPath, std::ref(body), std::ref(resp));
	}

	void ElasticClient::UpdateDocument(cpr::Response& resp, std::string body, std::string index, std::string id)
	{
		if (index.empty() || id.empty())
		{
			PRINT_ERROR_MAP(Index_Empty_Or_Id_Empty)
			return;
		}

		std::string reqPath = std::format("{}/_update/{}", index, id);
		Post(reqPath, std::ref(body), std::ref(resp));
	}

	void ElasticClient::DeleteDocument(cpr::Response& resp, std::string index, std::string id)
	{
		if (index.empty() || id.empty())
		{
			PRINT_ERROR_MAP(Index_Empty_Or_Id_Empty)
			return;
		}

		std::string reqPath = std::format("{}/_doc/{}", index, id);
		Delete(reqPath, "", std::ref(resp));
	}

	void ElasticClient::Bulk(cpr::Response& resp, std::string body)
	{
		Bulk(std::ref(resp), body, "");
	}

	void ElasticClient::Bulk(cpr::Response& resp, std::string body, std::string index)
	{
		std::string reqPath = index.size() == 0 ? "_bulk" : std::format("{}/_bulk", index);
		Post(reqPath, body, std::ref(resp));
	}

	/*----------------
		Search API
	----------------*/
	void ElasticClient::DefaultSearch(cpr::Response& resp, std::string index, std::string body)
	{
		
		if (index.empty())
		{
			PRINT_ERROR_MAP(Index_Empty)
			return;
		}

		std::string requestPath = std::format("{}/_search", index);
		Get(requestPath, "{ \"size\": 1000 }", std::ref(resp));
	}

	void ElasticClient::Search(cpr::Response& resp, std::string body)
	{
		Search(std::ref(resp), _defaultIndex, body);
	}

	void ElasticClient::Search(cpr::Response& resp, std::string index, std::string body)
	{
		std::string requestPath = std::format("{}/_search", index);
		Get(requestPath, body, std::ref(resp));
	}

	void ElasticClient::Count(cpr::Response& resp)
	{
		Count(std::ref(resp), _defaultIndex);
	}

	void ElasticClient::Count(cpr::Response& resp, std::string index)
	{
		std::string requestPath = std::format("{}/_count", index);
		Get(requestPath, "", std::ref(resp));
	}

	std::wstring ElasticClient::SearchAfter(cpr::Response& resp, std::string index, std::string body, int64 keepAliveSeconds)
	{
		std::chrono::seconds s(keepAliveSeconds);
		std::chrono::minutes m = duration_cast<std::chrono::minutes>(s);
		std::cout << m.count() << std::endl;
		std::string requestPath = std::format("{}/_pit?keep_alive={}m", index, m.count());
		Post(requestPath, "", std::ref(resp));
		std::error_code e{};
		json::value ret = json::value::parse(resp.text, e);
		if (ret == json::value::Null)
		{
			LogFunction(LogLevel::ERROR, L"JSON Convert Failed");
			return L"";
		}
		web::json::object str = ret.as_object();
		json::value x = str[L"id"];
		return x.as_string();
	}

	void ElasticClient::ClearScroll(cpr::Response& resp, std::string scrollId)
	{
		std::string reqPath = "_search/scroll";
		std::string bodyString = std::format("{{\"scroll_id\": \"{}\"}}", scrollId);
		Delete(reqPath, bodyString, std::ref(resp));
	}
}