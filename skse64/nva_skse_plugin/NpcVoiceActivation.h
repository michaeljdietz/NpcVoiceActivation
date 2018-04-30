#pragma once
#include "Npc.h"
#include "skse64/PapyrusNativeFunctions.h"
#include "skse64/PluginAPI.h"

#include <sphelper.h>
#include <sapi.h>
#include <vector>
#include <string>
#include <Windows.h>
#include <windef.h>

#define PRIMARY_WORD_WEIGHT		2.0f
#define SECONDARY_WORD_WEIGHT	1.0f
#define GREETING_WEIGHT			0.5f



namespace NpcVoiceActivation {
	unsigned __stdcall Initialize(void *params);

	bool AddNpcRules(const Npc* npc);
	bool AddRule(const int, const std::wstring, std::wstring, const std::wstring, const float);

	bool DisableNpcRules(const Npc* npc);
	bool DisableRule(const std::wstring);

	bool isValidResult(const HRESULT& result);
	void __stdcall OnVoiceCommand(WPARAM, LPARAM);

	int GetGreetingRuleId(int index);
	std::wstring GetGreetingRuleName(int index);
	void RegisterGreetings(StaticFunctionTag*, VMArray<BSFixedString>);
	void UnregisterGreetings();

	int GetNpcIndexByKeyword(BSFixedString);
	int GetNpcIndex(BSFixedString);
	void RegisterNpc(StaticFunctionTag*, BSFixedString);
	void UnregisterNpc(StaticFunctionTag*, BSFixedString);

	bool RegisterFuncs(VMClassRegistry*);
	void RegisterMessagingInterface(SKSEMessagingInterface*);
};
