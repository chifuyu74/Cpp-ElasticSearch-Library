#pragma once
#undef ERROR

namespace Elastic
{
	enum class LogLevel {
		FATAL = 0,
		ERROR = 1,
		WARNING = 2,
		INFO = 3,
		DEBUG = 4
	};

	/*-----------
		Logger
	-----------*/

	using LogCallback = std::function<void(Elastic::LogLevel, const std::string&)>;
	using wLogCallback = std::function<void(Elastic::LogLevel, const std::wstring&)>;

	class Logger
	{
	public:
		Logger();
		virtual ~Logger();

		void SetLogFunction(LogCallback extLogFunction);

		LogCallback GetLogFunction() { return _logFunction; }
		wLogCallback GetWLogFunction() { return _wLogFunction; }

		std::wstring ConvertWstring(std::string msg)
		{
			std::wstring wmsg;
			wmsg.assign(msg.begin(), msg.end());
			return wmsg;
		}

	public:
		using enum LogLevel;

	private:
		wLogCallback _wLogFunction = [=](Elastic::LogLevel level, const std::wstring& msg)
		{
			std::wstring log;
			switch (level)
			{
				using enum LogLevel;
			case FATAL:
				log = L"[FATAL]";
				break;
			case ERROR:
				log = L"[ERROR]";
				break;
			case WARNING:
				log = L"[WARNING]";
				break;
			case INFO:
				log = L"[INFO]";
				break;
			case DEBUG:
				log = L"[DEBUG]";
				break;
			}
			std::wcout << log << L" : " << msg << std::flush << std::endl;
			return;
		};


		LogCallback _logFunction = [=](LogLevel level, const std::string& msg)
		{
			std::string log;
			switch (level)
			{
			case FATAL:
				log = "[FATAL]";
				break;
			case ERROR:
				log = "[ERROR]";
				break;
			case WARNING:
				log = "[WARNING]";
				break;
			case INFO:
				log = "[INFO]";
				break;
			case DEBUG:
				log = "[DEBUG]";
				break;
			}
			std::cout << format("{} : {}", log, msg) << std::endl;
		};
	};
}

