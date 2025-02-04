#pragma once

namespace Str // String Series
{
	[[nodiscard(L"Extension Not Used")]] static const auto FileExtensionSearcher(const std::string& filePath) noexcept
	{
		const auto dotPos{ filePath.rfind('.') };
		if (dotPos != std::string::npos) return filePath.substr(dotPos + Def::UIntOne);

		return std::string{ "" };
	}

	[[nodiscard(L"Result Not Used")]] static const auto ReplaceChar(const std::string& str, char oldChar, char newChar) noexcept
	{
		auto result{ str };

		for (auto& c : result) 
			if (c == oldChar) c = newChar;

		return result;
	}

	[[nodiscard(L"Result Not Used")]] static const auto ReplaceString(const std::string& str, const std::string& oldStr, const std::string& newStr)
	{
		auto result{ str };
		auto pos   { size_t{} };

		while ((pos = result.find(oldStr, pos)) != std::string::npos) 
		{
			result.replace(pos, oldStr.length(), newStr);
			pos += newStr.length();
		}

		return result;
	}

	[[nodiscard(L"File Paths Not Used")]] static const auto GetFilePaths(const std::filesystem::path& directory, const std::string_view& exte)
	{
		auto filePaths{ std::vector<std::string>{} };

		for (const auto& entry : std::filesystem::directory_iterator(directory))
			if (entry.path().extension() == exte) filePaths.emplace_back(entry.path().string());

		return filePaths;
	}

	[[nodiscard(L"Convert String Not Used")]] static const auto StrToWStr(const std::string& str) noexcept
	{
		auto len{ MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0) };
		if (len <= NULL) {
			assert(false && "Length Errer");
			return std::wstring{ L"" };
		}
		auto wstr{ std::wstring(len, L'\0') };
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], len);
		return wstr;
	}

	[[nodiscard(L"Convert String Not Used")]] static const auto WStrToStr(const std::wstring& wstr) noexcept
	{
		auto len{ WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr) };
		if (len <= NULL) {
			assert(false && "Length Errer");
			return std::string{ "" };
		}

		auto str{ std::string(len, '\0') };
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], len, nullptr, nullptr);
		return str;
	}
}