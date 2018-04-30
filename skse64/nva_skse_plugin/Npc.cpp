#include "Npc.h"
#include "TextUtils.h"

Npc::Npc() {

}

const std::vector<std::wstring> Npc::ignoreWords = {
	L"a", L"an", L"the", L"sir", L"sire", L"dame", L"my", L"your", L"our", L"mr", L"mister", L"miss", L"ms", L"missus", L"mrs", L"mistress", // English articles
	L"one", L"two", L"three", L"four", L"five", L"six", L"seven", L"eight", L"nine", L"ten", // numbered articles
	L"as", L"at", L"by", L"from", L"for", L"in", L"into", L"of", L"off", L"on", L"onto", L"out", L"over", L"past", L"than", L"to", L"under", L"via", L"with", L"within", L"without" // prepositions
};

unsigned int Npc::getPrimaryId() const {
	return this->id;
}

std::wstring Npc::getPrimaryRuleName() const {
	return std::to_wstring(this->getPrimaryId());
}

unsigned int Npc::getSecondaryId() const {
	return this->id + MAX_NPCS;
}

std::wstring Npc::getSecondaryRuleName() const {
	return std::to_wstring(this->getSecondaryId());
}

std::wstring Npc::getWName() const {
	return this->wName;
}

void Npc::setWName(std::wstring wName) {
	this->wName = wName;
	this->name = std::string(wName.begin(), wName.end());
	this->setKeywords();
}

std::string Npc::getName() const {
	return this->name;
}

void Npc::setName(std::string name) {
	this->name = name;
	this->wName = std::wstring(name.begin(), name.end());
	this->setKeywords();
}

std::vector<std::wstring> Npc::getKeywords() const {
	return this->keywords;
}

void Npc::setKeywords() {
	std::vector<std::wstring> keywords;

	// TODO: Change this to allow for multiple delimiters
	TextUtils::WSplit(this->wName, L" ", keywords);

	for (std::vector<std::wstring>::iterator it = keywords.begin(); it != keywords.end(); ) {
		if (TextUtils::WInArray(*it, Npc::ignoreWords)) {
			it = keywords.erase(it);
		} else {
			++it;
		}		
	}
}
