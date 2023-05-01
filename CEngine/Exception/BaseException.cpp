#include "BaseException.h"

#include <sstream>

BaseException::BaseException(const char* file, int line) : 
	m_file(file), m_line(line){
}

const char* BaseException::what() const {
	m_whatString.clear();
	m_whatString = getType();
	m_whatString += "\n";

	return m_whatString.c_str();
}

std::string BaseException::getErrorLocation() const {
	std::ostringstream oss;
	oss << "File: " << m_file << "\n" <<
		"Line: " + std::to_string(m_line);

	return oss.str();
}

void CharToWChar(const char* pChar, std::wstring& wstr) {
	const size_t len = strlen(pChar);
	wstr.reserve(len);
	mbstowcs_s(nullptr, &wstr[0], len + 1, pChar, len);
}

void WCharToChar(const wchar_t* pWChar, std::string& str) {
	const size_t len = wcslen(pWChar);
	str.reserve(len+1);
	wcstombs_s(nullptr, &str[0], len+1, pWChar, len);
}