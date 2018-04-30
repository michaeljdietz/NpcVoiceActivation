ScriptName NVA extends Utility

Actor Property PlayerRef Auto
Keyword Property JarlKeywordProperty Auto

Spell Property NameFetcherSpell Auto
MagicEffect Property NameFetcherEffect Auto

Spell Property NearActivationSpell Auto
MagicEffect Property NearActivationEffect Auto

Spell Property FarActivationSpell Auto
MagicEffect Property FarActivationEffect Auto

bool IsActivating = false
string ActivatedName = ""

; MOVE THESE ALL TO SKSE library
; ini settings (default values)
bool bShowLogDebug = true
bool bShowLogWarning = true
bool bShowLogNotice = true
bool bShowLogError = true
bool bShowLogCritical = true

float fSummonSpeed = 0 ; How quickly an NPC travels to the player when summoned (between 0 [walk] and 1 [run])
float fSummonSpeed0 = -1
float fSummonSpeed1 = -1
float fSummonSpeed2 = -1
float fSummonSpeed3 = -1
float fSummonSpeed4 = -1
float fSummonSpeed5 = -1
float fSummonSpeed6 = -1
float fSummonSpeed7 = -1
float fSummonSpeed8 = -1

float fMaxSummonDistance = 25 ; NPC's further than this cannot be "summoned"
float fMaxSummonDistance0 = -1
float fMaxSummonDistance1 = -1
float fMaxSummonDistance2 = -1
float fMaxSummonDistance3 = -1
float fMaxSummonDistance4 = -1
float fMaxSummonDistance5 = -1
float fMaxSummonDistance6 = -1
float fMaxSummonDistance7 = -1
float fMaxSummonDistance8 = -1

float fMinSummonDistance = 10 ; NPC's further than this MUST be "summoned"
float fMinSummonDistance0 = -1
float fMinSummonDistance1 = -1
float fMinSummonDistance2 = -1
float fMinSummonDistance3 = -1
float fMinSummonDistance4 = -1
float fMinSummonDistance5 = -1
float fMinSummonDistance6 = -1
float fMinSummonDistance7 = -1
float fMinSummonDistance8 = -1

int iCanSummonGuards = 0
int iCanSummonGuards0 = -1
int iCanSummonGuards2 = -1
int iCanSummonGuards2 = -1
int iCanSummonGuards3 = -1
int iCanSummonGuards4 = -1
int iCanSummonGuards5 = -1
int iCanSummonGuards6 = -1
int iCanSummonGuards7 = -1
int iCanSummonGuards8 = -1

int iCanSummonJarls = 0
int iCanSummonJarls0 = -1
int iCanSummonJarls2 = -1
int iCanSummonJarls2 = -1
int iCanSummonJarls3 = -1
int iCanSummonJarls4 = -1
int iCanSummonJarls5 = -1
int iCanSummonJarls6 = -1
int iCanSummonJarls7 = -1
int iCanSummonJarls8 = -1

int iCanInterrupt = 1 ; Can player interrupt an NPC's conversation
int iCanInterrupt0 = -1
int iCanInterrupt1 = -1
int iCanInterrupt2 = -1
int iCanInterrupt3 = -1
int iCanInterrupt4 = -1
int iCanInterrupt5 = -1
int iCanInterrupt6 = -1
int iCanInterrupt7 = -1
int iCanInterrupt8 = -1

int iCanInterruptCombat = 0 ; Can player interrupt an NPC's combat state
int iCanInterruptCombat0 = -1
int iCanInterruptCombat1 = -1
int iCanInterruptCombat2 = -1
int iCanInterruptCombat3 = -1
int iCanInterruptCombat4 = -1
int iCanInterruptCombat5 = -1
int iCanInterruptCombat6 = -1
int iCanInterruptCombat7 = -1
int iCanInterruptCombat8 = -1

; WARNING -- Setting these to true could break your game!!!
int iCanInterruptScene = 0 ; Can player interrupt an NPC's scene
int iCanInterruptScene0 = -1
int iCanInterruptScene1 = -1
int iCanInterruptScene2 = -1
int iCanInterruptScene3 = -1
int iCanInterruptScene4 = -1
int iCanInterruptScene5 = -1
int iCanInterruptScene6 = -1
int iCanInterruptScene7 = -1
int iCanInterruptScene8 = -1

function NVA_ReloadSettings() Global Native
float function NVA_GetFloatValue(sKey) Global Native
int function NVA_GetIntValue(sKey) Global Native
string function NVA_GetStringValue(sKey) Global Native
bool function NVA_GetBoolValue(sKey) Global Native

event OnInit()
	GetSettings()
	UpdateProperties()
endevent

function UpdateProperties()
	; Needs implementation
endfunction

int function GetScopedIntValue(string sKey, int iScope)
	generalValue = NVA_GetIntValue(sKey)
	specificValue = NVA_GetIntValue(sKey + (relationshipRank as string))
	
	if (specificValue >= 0)
		return specificValue
	endif
	
	return generalValue
endfunction

float function GetScopedFloatValue(string sKey, int iScope)
	generalValue = NVA_GetFloatValue(sKey)
	specificValue = NVA_GetFloatValue(sKey + (relationshipRank as string))
	
	if (specificValue >= 0)
		return specificValue
	endif
	
	return generalValue
endfunction

function LogEntry(sLogText, sLogLevel) Global
	If (sLogLevel != "")
		sLogLevel = sLogLevel + ": "
	endif
	
	if ((!NVA_GetBoolValue(bShowLogDebug) && sLogLevel == "DEBUG") \
	 || (!NVA_GetBoolValue(bShowLogWarning) && sLogLevel == "WARNING" \
	 || (!NVA_GetBoolValue(bShowLogNotice) && sLogLevel == "NOTICE") \
	 || (!NVA_GetBoolValue(bShowLogError) && sLogLevel == "ERROR") \
	 || (!NVA_GetBoolValue(bShowLogCritical) && sLogLevel == "CRITICAL") \
	)
		return
	endif
	
	Debug.Notification(Utility.GetCurrentRealTime() + " " + sLogLevel + sLogText)
endfunction

; should we separate this to prevent script locking with requests for IsActivating property?
function InitiateDialog(Actor akActor)
	npcName = akActor.GetName()
	
	if (!IsValidDialogTarget(akActor))
		return false
	endif

	if (!PlayerRef.IsSneaking())
		LogEntry("Attempting traditional activation with " + npcName, "DEBUG")
		if (akActor.Activate(PlayerRef, false))
			LogEntry("Activation with " + npcName + " successful!", "DEBUG")
			return true
		endif
		
		LogEntry("Traditional activation with " + npcName + " failed!", "WARNING")
	endif
	
	LogEntry("Attempting AI package activation with " + npcName, "DEBUG")
	; Player was either sneaking or activate failed for some reason
	; Use Scene.Start() method, quest alias, AI Package override instead
	; Add ActiveMagicEffect to NPC that adds to QuestAlias
	
	return false
endfunction

bool function IsValidDialogTarget(Actor akTarget)	
	if (akTarget.GetSleepState() > 2 \
	 || akTarget.IsBleedingOut() \
	 || akTarget.IsDead() \
	 || akTarget.IsUnconscious() \
	 || akTarget.IsInDialogueWithPlayer() \
	)
		return false
	endif

	relationshipRank = akTarget.GetRelationshipRank(PlayerRef) + 4	; 0 to 8 instead of -4 to 4
	
	if ((akTarget.IsTalking() && GetScopedIntValue("CanInterrupt", relationshipRank) <= 0) \
	 || (akTarget.IsInCombat() && GetScopedIntValue("CanInterruptCombat", relationshipRank) <= 0) \
	 || (akTarget.IsInScene() && GetScopedIntValue("CanInterruptScene", relationshipRank) <= 0) \
	 || (!IsValidDistance(akTarget, relationshipRank)) \
	)
		return false
	endif
	
	return true
endfunction

bool function IsValidDistance(Actor akTarget, int relationshipRank)
	float distance = PlayerRef.GetDistance(akTarget)

	if (distance > GetScopedFloatValue("fMaxSummonDistance", relationshipRank) <= 0)
		return false
	endif
	
	if (akTarget.IsGuard() \
	 && distance > GetScopedFloatValue("fMinSummonDistance", relationshipRank) <= 0 \
	 && GetScopedIntValue("CanSummonGuard", relationshipRank) <= 0) \
	)
		return false
	endif
	
	if (akTarget.HasKeyword(JarlKeywordProperty) \
	 && distance > GetScopedFloatValue("fMinSummonDistance", relationshipRank) <= 0 \
	 && GetScopedIntValue("CanSummonJarl", relationshipRank) <= 0) \
	)
		return false
	endif
	
	return true
endfunction