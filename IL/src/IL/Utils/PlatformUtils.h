#pragma once

#include <string>

namespace IL
{

	class FileDialogs
	{
	public:
		// Implemented by different platforms
		// These return empty strings if cancelled
		static std::string FileDialogs::OpenFile(const char* filter);
		static std::string FileDialogs::SaveFile(const char* filter);
	};

}