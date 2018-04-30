#include "TextUtils.h"
#include <iterator>
#include <sstream>

std::wstring TextUtils::WJoin(const std::vector<std::wstring> &elements, const wchar_t* separator) {
	switch (elements.size()) {
	case 0:
		return L"";
	case 1:
		return elements[0];
	default:
		std::wstringstream os;
		std::copy(elements.begin(), elements.end() - 1, std::ostream_iterator<std::wstring, wchar_t>(os, separator));
		os << *elements.rbegin();
		return os.str();
	}
}


// TODO: Add overload to split by MULTIPLE delimiters
void TextUtils::WSplit(const std::wstring &path, const std::wstring &separator, std::vector<std::wstring> &result) {
	std::wstring::const_iterator cur = path.begin();
	std::wstring::const_iterator beg = path.begin();
	bool added = false;

	while (cur < path.end()) {
		if (*cur == *separator.c_str()) {
			result.insert(result.end(), std::wstring(beg, cur));
			beg = ++cur;
			added = true;
		} else {
			cur++;
		}
	}

	result.insert(result.end(), std::wstring(beg, cur));
}

bool TextUtils::WInArray(const std::wstring &needle, const std::vector<std::wstring> &haystack) {
	int max = haystack.size();

	if (max == 0) return false;

	for (int i = 0; i < max; i++) {
		if (haystack[i] == needle) {
			return true;
		}
	}

	return false;
}