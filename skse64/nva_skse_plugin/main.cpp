#include "skse64/PluginAPI.h"		// super
#include "skse64_common/skse_version.h"	// What version of SKSE is running?
#include <shlobj.h>
#include <time.h>

#include "NpcVoiceActivation.h"

static PluginHandle					g_pluginHandle = kPluginHandle_Invalid;
static SKSEPapyrusInterface         * g_papyrus = NULL;

void SKSEMessageHandler(SKSEMessagingInterface::Message *);

extern "C"	{
	bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info) {	// Called by SKSE to learn about this plugin and check that it's safe to load it
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
		NpcVoiceActivation::RegisterHandle(&g_pluginHandle);

		NpcVoiceActivation::RegisterMessagingInterface((SKSEMessagingInterface *)skse->QueryInterface(kInterface_Messaging));

		if(skse->isEditor)
		{
			_MESSAGE("loaded in editor, marking as incompatible");

			return false;
		}
		else if(skse->runtimeVersion != RUNTIME_VERSION_1_5_39 /*RUNTIME_VR_VERSION_1_3_64*/ )
		{
			_MESSAGE("unsupported runtime version %08X", skse->runtimeVersion);

			return false;
		}

		// ### do not do anything else in this callback
		// ### only fill out PluginInfo and return true/false

		// supported runtime version
		return true;
	}

	bool SKSEPlugin_Load(const SKSEInterface * skse) {
		_MESSAGE("NpcVoiceActivation loaded");

		g_papyrus = (SKSEPapyrusInterface *)skse->QueryInterface(kInterface_Papyrus);

		bool btest = g_papyrus->Register(NpcVoiceActivation::RegisterFuncs);

		if (btest) {
			_MESSAGE("Register Succeeded");
		}

		return true;
	}
};