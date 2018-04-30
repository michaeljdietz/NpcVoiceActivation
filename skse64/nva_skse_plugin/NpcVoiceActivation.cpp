#include "NpcVoiceActivation.h"

namespace NpcVoiceActivation {
	ISpAudio*					m_audio;
	CComPtr<ISpRecognizer>		m_recognizer;
	CComPtr<ISpRecoContext>		m_recoContext;
	CComPtr<ISpRecoGrammar>		m_recoGrammar;

	const ULONGLONG				grammarId = 0;
	bool						isNpcActivated = false;
	std::string					activatedNpcName = "";

	PluginHandle*				g_pluginHandle;
	SKSEMessagingInterface*		g_messagingInterface;

	std::vector<Npc>			npcs;
	std::vector<std::wstring>	greetings;


	#pragma region Papyrus Native Functions
		void RegisterNpc(StaticFunctionTag* base, BSFixedString npcName) {
			_MESSAGE("RegisterNpc");

			int i = GetNpcIndex(npcName);

			if (i == -1) {
				Npc npc;
				npc.setName(npcName.c_str());
				npc.count = 1;
				npc.id = npcs.size() + 1;
				npc.isActive = true;

				npcs.push_back(npc);
				AddNpcRules(&npc);

				return;
			}

			Npc *npc = &npcs[i];
			npc->count++;
		}

		void UnregisterNpc(StaticFunctionTag* base, BSFixedString npcName) {
			_MESSAGE("UnregisterNpc");

			int i = GetNpcIndex(npcName);

			if (i == -1) {
				_ERROR("Error unregistering the NPC because they have never been registered!");
				return;
			}
			else {
				Npc *npc = &npcs[i];
				npc->count--;

				if (npc->count == 0) {
					npc->isActive = false;
					DisableNpcRules(npc);
				}
			}
		}

		#pragma region Event Registration

			void RegisterForOnVoiceCommand(StaticFunctionTag *base, TESForm * thisForm)
			{

			}
		#pragma endregion
	
	#pragma endregion
	
	#pragma region API

		unsigned __stdcall Initialize(void *params)
		{
			HRESULT hr;

			// Initialize COM library
			hr = ::CoInitialize(NULL);

			if (FAILED(hr)) {
				_ERROR("Could not initialize COM libary");
				return 0;
			}

			hr = m_recognizer.CoCreateInstance(CLSID_SpInprocRecognizer);

			if (!isValidResult(hr)) {
				_ERROR("Error creating COM instance for speech recognizer!");
				return 0;
			}

			hr = m_recognizer->CreateRecoContext(&m_recoContext);
			if (!isValidResult(hr)) {
				_ERROR("Error creating speech recognizer context!");
				return 0;
			}

			hr = m_recoContext->SetNotifyCallbackFunction(OnVoiceCommand, 0, 0);
			if (!isValidResult(hr)) {
				_ERROR("Error assigning speech recognizer callback function!");
				return 0;
			}

			hr = CoCreateInstance(CLSID_SpMMAudioIn,
				nullptr, CLSCTX_ALL, IID_ISpAudio,
				reinterpret_cast<void**>(&m_audio));

			if (!isValidResult(hr)) {
				_ERROR("Error creating COM instance for speech recognition audio input device!");
				return 0;
			}

			/* TODO: Fix this... ERROR 0x00000001 when trying to SetInput to default audio device
			hr = m_recognizer->SetInput(m_audio, false);
			if (!isValidResult(hr)) {
			_ERROR("Error setting input for speech recognizer!");
			return 0;
			}
			*/

			hr = m_recoContext->CreateGrammar(grammarId, &m_recoGrammar);
			if (!isValidResult(hr)) {
				_ERROR("Error creating speeceh recognition grammar!");
				return 0;
			}

			_MESSAGE("Done with initialization");
			return 1;
		}

		bool AddRule(const int ruleId, const std::wstring ruleName, std::wstring words, const std::wstring separator, const float weight) {
			_MESSAGE("AddRule");
			HRESULT hr;
			SPSTATEHANDLE state;

			hr = m_recoGrammar->GetRule(ruleName.c_str(), ruleId, SPRAF_TopLevel | SPRAF_Active, true, &state);
			if (!isValidResult(hr)) {
				const std::string message = "Error creating grammar rule #" + std::to_string(ruleId) + "!";
				_ERROR(message.c_str());
				return false;
			}

			hr = m_recoGrammar->AddWordTransition(state, NULL, words.c_str(), separator.c_str(), SPWT_LEXICAL, weight, nullptr);
			if (!isValidResult(hr)) {
				const std::string message = "Error adding words to grammar rule #" + std::to_string(ruleId) + "!";
				_ERROR(message.c_str());
				return false;
			}

			// this needs to be here in case a rule is disabled and then re-enabled
			hr = m_recoGrammar->SetRuleState(ruleName.c_str(), NULL, SPRS_ACTIVE);
			if (!isValidResult(hr)) {
				const std::string message = "Error activating grammar rule #" + std::to_string(ruleId) + "!";
				_ERROR(message.c_str());
				return false;
			}

			return true;
		}

		bool DisableRule(const std::wstring ruleName) {
			_MESSAGE("DisableRule");
			HRESULT hr;

			hr = m_recoGrammar->SetRuleState(ruleName.c_str(), NULL, SPRS_INACTIVE);
			if (!isValidResult(hr)) {
				const std::string message = "Error disabling grammar rule for " + std::string(ruleName.begin(), ruleName.end()) + "!";
				_ERROR(message.c_str());
				return false;
			}

			return true;
		}

		void __stdcall OnVoiceCommand(WPARAM wParam, LPARAM sParam) {
			_MESSAGE("OnVoiceCommand");

			CSpEvent	event;
			int			i = 0;
			HRESULT		hr = S_OK;
			BOOL		fContinue = true;

			if (!SUCCEEDED(event.GetFrom(m_recoContext))) {
				_ERROR("Error reading event from speech recognizer!");
				return;
			}

			switch (event.eEventId) {
			case SPEI_RECOGNITION:
				ISpRecoResult * pResult = event.RecoResult();

				LPWSTR pszCoMemResultText = NULL;
				hr = pResult->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &pszCoMemResultText, NULL);

				if (SUCCEEDED(hr)) {
					int npcIndex = -1;
					std::string word = CW2A(pszCoMemResultText);
					BSFixedString name = "";

					npcIndex = GetNpcIndex(word.c_str());
					if (npcIndex == -1) {
						npcIndex = GetNpcIndexByKeyword(word.c_str());
					}

					if (npcIndex >= 0) {
						name = word.c_str();
					}

					SKSEModCallbackEvent evn("OnVoiceCommand", name, 0.0, NULL);
					EventDispatcher<SKSEModCallbackEvent> * dispatcher = (EventDispatcher<SKSEModCallbackEvent> *) g_messagingInterface->GetEventDispatcher(SKSEMessagingInterface::kDispatcher_ModEvent);
					if (dispatcher) {
						dispatcher->SendEvent(&evn);
					}
				}

				if (NULL != pszCoMemResultText) {
					CoTaskMemFree(pszCoMemResultText);
				}

				break;
			}
		}

		bool isValidResult(const HRESULT& result)
		{
			if (result == S_OK) {
				return true;
			}

			std::string message;

			switch (result)
			{
			case E_INVALIDARG:
				message = "One or more arguments are invalids.";

			case E_ACCESSDENIED:
				message = "Acces Denied.";

			case E_NOINTERFACE:
				message = "Interface does not exist.";

			case E_NOTIMPL:
				message = "Not implemented method.";

			case E_OUTOFMEMORY:
				message = "Out of memory.";

			case E_POINTER:
				message = "Invalid pointer.";

			case E_UNEXPECTED:
				message = "Unexpecter error.";

			case E_FAIL:
				message = "Failure";

			default:
				std::string strHR;
				strHR.resize(11, 0);
				sprintf(&(strHR[0]), "0x%08X", result);

				message = "Unknown : " + strHR;
			}

			_ERROR(message.c_str());

			return false;
		}

		#pragma region Abstractions

			bool AddNpcRules(const Npc* npc)
			{
				_MESSAGE("AddNpcRules");
				if (!AddRule(npc->getPrimaryId(), npc->getPrimaryRuleName(), npc->getWName(), NULL, PRIMARY_WORD_WEIGHT)) {
					std::wstring name = npc->getWName();
					const std::string message = "Error adding primary grammar rule for " + std::string(npc->getName().begin(), npc->getName().end()) + "!";
					_ERROR(message.c_str());
					return false;
				}

				std::wstring keywords = TextUtils::WJoin(npc->getKeywords(), L" ");
				if (!AddRule(npc->getSecondaryId(), npc->getSecondaryRuleName(), keywords, L" ", SECONDARY_WORD_WEIGHT)) {
					std::wstring name = npc->getWName();
					const std::string message = "Error adding secondary grammar rule for " + std::string(npc->getName().begin(), npc->getName().end()) + "!";
					_ERROR(message.c_str());
					return false;
				}

				return true;
			}

			bool DisableNpcRules(const Npc* npc) {
				_MESSAGE("DisableNpcRules");

				bool hasError = false;

				if (!DisableRule(npc->getPrimaryRuleName())) {
					_ERROR("Error disabling primary grammar rule #" + npc->getPrimaryId());
					hasError = true;
				}

				if (!DisableRule(npc->getSecondaryRuleName())) {
					_ERROR("Error disabling secondary grammar rule #" + npc->getSecondaryId());
					hasError = true;
				}

				return hasError;
			}

			int GetGreetingRuleId(int index) {
				return index + 2 * MAX_NPCS;
			}

			std::wstring GetGreetingRuleName(int index) {
				return std::to_wstring(GetGreetingRuleId(index));
			}

			void RegisterGreetings(VMArray<BSFixedString> greetings) {
				_MESSAGE("RegisterGreetings");

				UnregisterGreetings();

				for (UInt32 i = 0; i < greetings.Length(); i++) {
					BSFixedString *greeting = NULL;
					greetings.Get(greeting, i);

					std::string sGreeting = greeting->c_str();
					std::wstring wGreeting = std::wstring(sGreeting.begin(), sGreeting.end());
					NpcVoiceActivation::greetings.push_back(wGreeting);

					AddRule(GetGreetingRuleId(i), GetGreetingRuleName(i), wGreeting, NULL, GREETING_WEIGHT);
				}
			}

			void UnregisterGreetings() {
				_MESSAGE("UnregisterGreetings");

				for (int i = 0; i < greetings.size(); i++) {
					int ruleId = GetGreetingRuleId(i);
					DisableRule(GetGreetingRuleName(i));
				}

				greetings.clear();
			}

			int GetNpcIndexByKeyword(BSFixedString word) {
				int i = 0;
				std::string sWord = word.c_str();
				std::wstring wWord = std::wstring(sWord.begin(), sWord.end());

				for (std::vector<Npc>::iterator it = npcs.begin(); it != npcs.end(); ++it) {
					if (TextUtils::WInArray(wWord, it->getKeywords())) {
						return i;
					}
					i++;
				}

				return -1;
			}

			int GetNpcIndex(BSFixedString npcName) {
				int i = 0;

				for (std::vector<Npc>::iterator it = npcs.begin(); it != npcs.end(); ++it) {
					if (it->getName().c_str() == npcName.c_str()) {
						return i;
					}
					i++;
				}

				return -1;
			}

		#pragma endregion

	#pragma endregion

	#pragma region Messaging Interface

		// Listens for SKSE events
		void OnSKSEMessageReceived(SKSEMessagingInterface::Message* message)
		{
			if (message)
			{
				_MESSAGE("Received SKSE message %d", message->type);
				switch (message->type)
				{
				case SKSEMessagingInterface::kMessage_PostPostLoad:
					_MESSAGE("Game Loaded, Dispatching Init messages to all listeners");

					// TODO: Seems ridiculous that we have to create another thread just to initialize all the COM objects
					// It was hanging otherwise (no message pump?). Any ideas?
					HANDLE threads[1];
					unsigned aID;
					threads[0] = (HANDLE)_beginthreadex(NULL, 0, NpcVoiceActivation::Initialize, NULL, 0, &aID);
				}
			}
		}

		void RegisterMessagingInterface(SKSEMessagingInterface * messagingInterface) {
			if (messagingInterface && g_pluginHandle)
			{
				g_messagingInterface = messagingInterface;
				_MESSAGE("Registering for plugin loaded message!");
				g_messagingInterface->RegisterListener(*g_pluginHandle, "SKSE", OnSKSEMessageReceived);
			}
		}

		void RegisterHandle(PluginHandle* handle) {
			if (handle)
				g_pluginHandle = handle;
		}

	#pragma endregion

	// Entry Point
	bool RegisterFuncs(VMClassRegistry* registry)
	{
		_MESSAGE("Registering native functions...");
		registry->RegisterFunction(new NativeFunction1 <StaticFunctionTag, void, BSFixedString>("RegisterNpc", "NpcVoiceActivation", RegisterNpc, registry));
		registry->RegisterFunction(new NativeFunction1 <StaticFunctionTag, void, BSFixedString>("UnregisterNpc", "NpcVoiceActivation", UnregisterNpc, registry));

		return true;
	}
}