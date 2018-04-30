scriptname NVA_NpcDistance_Thread extends Quest hidden

ObjectReference future
bool thread_queued = false

GlobalVariable Property NVA_fMinSummonDistance auto
GlobalVariable Property NVA_bCanInterruptScene auto
GlobalVariable Property NVA_bCannotSummonGuards auto
GlobalVariable Property NVA_bCannotSummonJarls auto

Actor theNPC
Actor thePlayer
string theName
Static theMarker

ObjectReference function get_async(Activator akFuture, ObjectReference akFutureAnchor, Actor akNPC, Static akXMarker, Actor akPlayer, string sName)
	thread_queued = true
	
	theNPC = akNPC
	theMarker = akXMarker
	thePlayer = akPlayer
	theName = sName
	
	future = akFutureAnchor.PlaceAtMe(akFuture)
endfunction

bool function queued()
	return thread_queued
endfunction

bool function has_future(ObjectReference akFuture)
	if akFuture == future
		return true
	else
		return false
	endif
endfunction

bool function force_unlock()
	clear_thread_vars()
	thread_queued = false
	return true
endfunction

event OnCalculateDistance()
	if thread_queued
		ObjectReference tempMarker = thePlayer.PlaceAtMe(theMarker)
		distance = GetTargetDistance()
		;(future as NVA_NpcDistance_Future).NPC = theNPC
		;(future as NVA_NpcDistance_Future).distance = distance
		
		clear_thread_vars()
		thread_queued = false
	endif
endevent

float function GetTargetDistance()
	if (!self.IsValidTarget())
		return -1
	endIf

	float distance = NVA.PlayerRef.GetDistance(akTarget)
	if (akTarget.IsGuard() && distance > NVA_fMinSummonDistance && NVA_bCannotSummonGuards)
		return -1
	endIf
	
	if (self.IsTargetJarl(akTarget) && distance > NVA_fMinSummonDistance && NVA_bCannotSummonJarls)
		return -1
	endIf
	
	return distance
EndFunction

bool function IsValidTarget()
	if (theName != "" && theName != theNPC.GetName())
		return false
	endIf
	
	if (theNPC.GetSleepState() > 2)
		return false
	endIf
	
	if (theNPC.IsBleedingOut())
		return false
	endIf
	
	if (theNPC.IsDead())
		return false
	endIf
	
	if (theNPC.IsUnconscious())
		return false
	endIf
	
	if (theNPC.IsInDialogueWithPlayer())
		return false
	endIf
	
	if (theNPC.IsTalking() && !CanInterrupt())
		return false
	endIf
	
	if (theNPC.IsInCombat() && !CanInterruptCombat())
		return false
	endIf

	if (theNPC.IsInScene() && !NVA_bCanInterruptScene)
		return false
	endIf

	return true
EndFunction

bool function CanInterrupt()
	; TODO: Needs implementation
	return true
EndFunction

bool function CanInterruptCombat()
	; TODO: Needs implementation
	return true
EndFunction

bool function TargetNeedsSummoning()
	; TODO: Needs implementation
	return false
EndFunction

bool function CanSummonTarget()
	; TODO: Needs implementation
	return false
EndFunction

bool function SummonTarget()
	; TODO: Needs implementation
	return false
EndFunction


