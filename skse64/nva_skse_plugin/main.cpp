#include "skse64/PluginAPI.h"		// super
#include "skse64_common/skse_version.h"	// What version of SKSE is running?
#include <shlobj.h>
#include <time.h>

#include "NpcVoiceActivation.h"
#include "CustomIniFile.h"

static PluginHandle					g_pluginHandle = kPluginHandle_Invalid;
static SKSEPapyrusInterface         * g_papyrus = NULL;
static SKSEMessagingInterface		* g_messagingInterface = NULL;

void SKSEMessageHandler(SKSEMessagingInterface::Message *);

extern "C"	{
	bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)	{	// Called by SKSE to learn about this plugin and check that it's safe to load it
		gLog.OpenRelative(CSIDL_PERSONAL, "\\My Games\\Skyrim Special Edition\\SKSE\\NpcVoiceActivation.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_Message);
		gLog.SetLogLevel(IDebugLog::kLevel_Message);

		_MESSAGE("NpcVoiceActivation");

		// populate info structure
		info->infoVersion =	PluginInfo::kInfoVersion;
		info->name =		"NpcVoiceActivation";
		info->version =		1;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = skse->GetPluginHandle();

		if(skse->isEditor)
		{
			_MESSAGE("loaded in editor, marking as incompatible");

			return false;
		}
		else if(skse->runtimeVersion != RUNTIME_VERSION_1_5_39 /*RUNTIME_VR_VERSION_1_3_64*/)
		{
			_MESSAGE("unsupported runtime version %08X", skse->runtimeVersion);

			return false;
		}

		// ### do not do anything else in this callback
		// ### only fill out PluginInfo and return true/false

		// supported runtime version
		return true;
	}

	bool SKSEPlugin_Load(const SKSEInterface * skse)	{	// Called by SKSE to load this plugin
		return true;
	}
};

void NpcVoiceActivation_Initialize(void) {
	/*
	if (!NpcVoiceActivation::Initialize()) {
	_ERROR("NpcVoiceActivation load failed!");
	}
	*/

	_MESSAGE("NpcVoiceActivation loaded");
	g_papyrus = (SKSEPapyrusInterface *)skse->QueryInterface(kInterface_Papyrus);
	g_messagingInterface = (SKSEMessagingInterface *)skse->QueryInterface(kInterface_Messaging);


	g_messagingInterface->RegisterListener(g_pluginHandle, "SKSE", SKSEMessageHandler);
	NpcVoiceActivation::RegisterMessagingInterface(g_messagingInterface);


	// Check if the function registration was a success...
	bool btest = g_papyrus->Register(NpcVoiceActivation::RegisterFuncs);

	// CustomIniFile::SetModName("NVA");
	// btest = btest && g_papyrus->Register(CustomIniFile::RegisterFuncs);

	if (btest) {
		_MESSAGE("Register Succeeded");
	}


	_MESSAGE("Test successful");
}

void SKSEMessageHandler(SKSEMessagingInterface::Message * msg) {
	switch (msg->type) {
	case SKSEMessagingInterface::kMessage_PostLoad:
		_MESSAGE("Initializing NpcVoiceActivation");
		
		HANDLE threads[1];
		unsigned aID;
		threads[0] = (HANDLE)_beginthreadex(NULL, 0, NpcVoiceActivation::Initialize, NULL, 0, &aID);
		// WaitForMultipleObjects(1, threads, true, INFINITE);
		// NpcVoiceActivation::Initialize();
	}
}

#if BLD_DEBUG
BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		FreeConsole();
	}
	return TRUE;
}

#endif
