#pragma once
#include <vector>
#include <string>
#include <sstream>

#define MAX_NPCS 1000


// We are assuming that all BSFixedString's are single byte char * arrays (and it currently seems this is the case)
// If SKSE (or Skyrim for that matter) adds multi-byte character support, we can easily modify this class to 
// allow for that.

class Npc
{
public:
	UInt32 count;
	UInt32 id; // this is the primary rule id
	bool isActive;

	Npc();
	unsigned int getPrimaryId() const;
	unsigned int getSecondaryId() const;

	std::wstring getPrimaryRuleName() const;
	std::wstring getSecondaryRuleName() const;

	std::wstring getWName() const;
	void setWName(std::wstring);

	std::string getName() const;
	void setName(std::string);

	std::vector<std::wstring> getKeywords() const;

private:
	static const std::vector<std::wstring> ignoreWords;
	std::wstring wName;
	std::string name;
	std::vector<std::wstring> keywords;

	void setKeywords();
};