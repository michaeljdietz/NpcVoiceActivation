#pragma once
#include "Npc.h"
#include "TextUtils.h"

#include "skse64/PapyrusNativeFunctions.h"
#include "skse64/PluginAPI.h"
#include "skse64/PapyrusEvents.h"
#include "skse64/GameReferences.h"
#include "skse64/GameExtraData.h"
#include "skse64/PapyrusVM.h"

#include <algorithm>
#include <sstream>
#include <atlstr.h>
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
	#pragma region Papyrus Native Functions

		// TODO: Can we pass an Actor object reference and simply activate them here in the DLL instead of using a cloak ability in Papyrus?
		// TODO: Can we acquire a list of nearby Actor's in C++ instead of using a cloak ability in Papyrus to improve performance and maintainability?

		// Registers a particular Actor for voice recognition by the Actor's name
		void RegisterNpc(StaticFunctionTag*, BSFixedString);

		// Unregisters a particular Actor for voice recognition by the Actor's name
		void UnregisterNpc(StaticFunctionTag*, BSFixedString);
	
		#pragma	region Event Registration

			// Used by papyrus script to register for the voice command event
			void RegisterForOnVoiceCommand(StaticFunctionTag *base, TESForm * thisForm);

		#pragma endregion

	#pragma endregion

	#pragma region API
		unsigned __stdcall Initialize(void *params);
		bool AddRule(const int, const std::wstring, std::wstring, const std::wstring, const float);
		bool DisableRule(const std::wstring);
		void __stdcall OnVoiceCommand(WPARAM, LPARAM);
		bool isValidResult(const HRESULT&);

		#pragma region Abstractions
			bool AddNpcRules(const Npc* npc);
			bool DisableNpcRules(const Npc* npc);
			int GetGreetingRuleId(int index);
			std::wstring GetGreetingRuleName(int index);
			void RegisterGreetings(StaticFunctionTag*, VMArray<BSFixedString>);
			void UnregisterGreetings();
			int GetNpcIndexByKeyword(BSFixedString);
			int GetNpcIndex(BSFixedString);
		#pragma endregion

	#pragma endregion

	#pragma region Messaging Interface
	void OnSKSEMessageReceived(SKSEMessagingInterface::Message* message);
	void RegisterMessagingInterface(SKSEMessagingInterface*);
	void RegisterHandle(PluginHandle*);
	#pragma endregion

	bool RegisterFuncs(VMClassRegistry*);
};
