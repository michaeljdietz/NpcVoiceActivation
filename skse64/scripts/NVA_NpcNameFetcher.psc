scriptname NVA_NpcNameFetcher extends ActiveMagicEffect

Utility Property NVA Auto

Actor MySelf
string MyName

function RegisterNpc(string sName) Global Native
function UnregisterNpc(string sName) Global Native

event OnEffectStart(Actor akTarget, Actor akCaster)
	MySelf = akTarget
	MyName = MySelf.GetName()	; for debug purposes only -- remove after testing
	
	NVA.LogEntry("Actor " + MyName + " registered with speech recognition.", "DEBUG")
	RegisterNpc(MyName)
endevent

event OnEffectFinish(Actor akTarget, Actor akCaster)
	NVA.LogEntry("Actor " + MyName + " unregistered with speech recognition.", "DEBUG")
	UnregisterNpc(MyName)
endevent