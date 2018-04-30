#pragma once
#include "../skse64/PapyrusNativeFunctions.h"

/*
 * Usage: CustomIniFile myIniFile = new CustomIniFile("MyMod");
 * In Papyrus: Create a script named MyMod and register the following 5 functions:
 * MyMod_ReloadSettings(), MyMod_GetFloatValue(string), MyMod_GetIntValue(string),
 * MyMod_GetBoolValue(string), MyMod_GetStringValue(string)
 *
 * Use these native methods on your MyMod script to get values from the Data/MyMod.ini file.
 * The ini file uses a similar format as Skryim.ini/Fallout4.ini
 */

namespace CustomIniFile {
	void ReadFile();

	void SetModName(std::string modName);
	void ReloadSettings(StaticFunctionTag*);
	float GetFloatValue(StaticFunctionTag*, BSFixedString);
	UInt32 GetIntValue(StaticFunctionTag*, BSFixedString);
	BSFixedString GetStringValue(StaticFunctionTag*, BSFixedString);
	bool GetBoolValue(StaticFunctionTag*, BSFixedString);
	bool RegisterFuncs(VMClassRegistry*);

	std::string GetFilename();
}