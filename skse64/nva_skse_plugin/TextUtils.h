#pragma once

#include <vector>
#include <string>

namespace TextUtils {
	std::wstring WJoin(const std::vector<std::wstring>&, const wchar_t *);

	void WSplit(const std::wstring &, const std::wstring &, std::vector<std::wstring>&);
	bool WInArray(const std::wstring &, const std::vector<std::wstring> &);
}
