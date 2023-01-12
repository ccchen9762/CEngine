#pragma once

#include <exception>
#include <string>

void CharToWChar(const char* pChar, std::wstring& wstr);
void WCharToChar(const wchar_t* pWChar, std::string& str);

class BaseException : std::exception {
public:
	BaseException(const char* file, int line);
	
	const char* what() const noexcept override;
	virtual const char* getType() const { return "BaseException"; }
	virtual const wchar_t* getWType() const { return L"BaseException"; }
	const std::string& getFile() const { return m_file; }
	int getLine() const { return m_line; }
	std::string getErrorLocation() const;

private:
	std::string m_file;
	int m_line;

protected:
	mutable std::string m_whatString;
	mutable std::wstring m_whatWString;

};