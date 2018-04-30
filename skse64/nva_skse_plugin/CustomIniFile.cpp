#include "CustomIniFile.h"

namespace CustomIniFile {
	std::string modName;

	std::map<std::string, int> intValues;
	std::map<std::string, std::string> stringValues;
	std::map<std::string, bool> boolValues;
	std::map<std::string, float> floatValues;
}

void CustomIniFile::SetModName(std::string modName) {
	CustomIniFile::modName = modName;
}

std::string CustomIniFile::GetFilename() {
	return modName + ".ini";
}

void CustomIniFile::ReadFile() {
	/* TODO: All the good stuff goes here!
	 * Format:
	 * 1) Key=Value
	 * 2) Trailing whitespace is ignored on either side of either variable (key/value)
	 * 3) All text on a line after a semi-colon is ignored (comments)
	 * 4) Parse error skips to next line
	 * 5) Section headings are ignored (only for user organization)
	 */
}

void CustomIniFile::ReloadSettings(StaticFunctionTag* base) {
	return;
}

float CustomIniFile::GetFloatValue(StaticFunctionTag* base, BSFixedString key) {
	return 0.0f;
}

UInt32 CustomIniFile::GetIntValue(StaticFunctionTag* base, BSFixedString key) {
	return 0;
}

bool CustomIniFile::GetBoolValue(StaticFunctionTag* base, BSFixedString key) {
	return true;
}

BSFixedString CustomIniFile::GetStringValue(StaticFunctionTag* base, BSFixedString key) {
	const char * result = "";
	return BSFixedString(result);
}

#include "skse64/PapyrusVM.h"
#include "skse64/PapyrusNativeFunctions.h"


/*
 * This registers 4 "Get" functions: GetFloatValue, GetIntValue, GetBoolValue, and GetStringValue.
 * This class upon mod load will look for a file in the Data/ directory named ModName.ini and load any values from that
 * file into memory.  Whenever Papyrus calls one of the four functions above, the key passed will be looked up in memory to find
 * its value. 
 *
 * This also registers a ReloadSettings() function that will refresh values from ModName.ini (so you can create a console
 * command in Papyrus to do so at runtime).
 */
bool CustomIniFile::RegisterFuncs(VMClassRegistry* registry) {
	registry->RegisterFunction(
		new NativeFunction0 <StaticFunctionTag, void>("ReloadSettings", modName.c_str(), CustomIniFile::ReloadSettings, registry)
	);

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, float, BSFixedString>("GetFloatValue", modName.c_str(), CustomIniFile::GetFloatValue, registry)
	);

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, UInt32, BSFixedString>("GetIntValue", modName.c_str(), CustomIniFile::GetIntValue, registry)
	);

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, bool, BSFixedString>("GetBoolValue", modName.c_str(), CustomIniFile::GetBoolValue, registry)
	);

	registry->RegisterFunction(
		new NativeFunction1 <StaticFunctionTag, BSFixedString, BSFixedString>("GetStringValue", modName.c_str(), CustomIniFile::GetStringValue, registry)
	);

	return true;
}