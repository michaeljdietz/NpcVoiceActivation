scriptname NVA_PlayerScript extends ReferenceAlias

Utility Property NVA Auto
Actor Property PlayerRef Auto

Spell Property NameFetcherSpell Auto
Spell Property NearActivationSpell Auto
Spell Property FarActivationSpell Auto

GlobalVariable Property NVA_NpcNameFetcherOn Auto
float fPollingRate = 8 ; a rate of 8 would be once per cell at a full sprint
float fFarNpcDelay = 0.5 ; we may be able to lower this, requires testing
float fNameFetcherLifetime = 1

event OnInit()
	RegisterForSingleUpdate(1)
	; RegisterForModEvent("NVA_OnVoiceCommand", "OnVoiceCommand")
	
	; Testing PlayerRef vs NVA.PlayerRef
	NVA.LogEntry("PlayerRef start", "DEBUG")
	NVA.LogEntry(PlayerRef.GetName(), "DEBUG")
	NVA.LogEntry("PlayerRef end", "DEBUG")
	
	NVA.LogEntry("NVA.PlayerRef start", "DEBUG")
	NVA.LogEntry(NVA.PlayerRef.GetName(), "DEBUG")
	NVA.LogEntry("NVA.PlayerRef end", "DEBUG")
endevent

event OnUpdate()
	if NVA_NpcNameFetcherOn.GetValue()
		PlayerRef.AddSpell(NameFetcherSpell, false)
		Utility.Wait(fNameFetcherLifetime)
		PlayerRef.RemoveSpell(NameFetcherSpell)
	endif
	
	RegisterForSingleUpdate(fPollingRate)
endevent

event OnVoiceCommand(string sNpcName)
	NVA.LogEntry("Voice command received for NPC Activation " + sNpcName, "DEBUG")
	NVA.ActivatedName = sNpcName
	NVA.IsActivating = true
	
	NearActivationSpell.Cast(PlayerRef) ; I'm assuming we don't need to set a target here and it will use the crosshair instead
	Utility.Wait(fFarNpcDelay)
	
	if (!NVA.IsActivating)
		return
	endif
	
	FarActivationSpell.Cast(PlayerRef) ; Since this is a cloak, we DEFINITELY don't need to specify a target
endevent

; this probably can be deleted -- need to test first (if target is required for Spell.Cast, we will have to use this to find nearest NPC in cloak
Actor function GetNearestLOSActor(string sNpcName)
	NVA.LogEntry("Looking for nearest actor with LOS " + sNpcName, "DEBUG")

	Actor akNearestLOSActor = None
	float shortestDistance = fMaxUnnamedDistance + 1

	Int i = NPCs.GetSize()
	
	if (i <= 0)
		return
	endif
	
	while i > 0
		; TODO: Should this be multi-threaded?  Depends on radius of cloak
		; Use futures to populate array of distances
		; Create an array of FormList indexes in order by distance
		; Test LOS for each actor in a single thread, starting with the shortest distance first
		; Return when one with LOS is found
		; Threads need to not share player resource somehow for distance calculation...
		; Threads can be started with a RaiseEvent
		
		i -= 1
		akNPC = NPCs.GetAt(i) as Actor
		distance = self.GetTargetDistance(akNPC, sNpcName)
		NVA.LogEntry("Distance of " + akNPC.GetName() + " is " + distance)
		if (distance != -1 && distance < shortestDistance)
			NVA.LogEntry(akNPC.GetName() + " now has the shortest distance.", "DEBUG")
			if (NVA.PlayerRef.HasDetectionLOS(akNPC))
				NVA.LogEntry(akNPC.GetName() + " is in the player's line of sight.", "DEBUG")
				akNearestLOSActor = akNPC
				shortestDistance = distance
			Else
				NVA.LogEntry(akNPC.GetName() + " is NOT in the player's line of sight.", "DEBUG")
			endif
		endif
	endwhile
	
	NVA.LogEntry("Nearest LOS Actor is: " + akNearestLOSActor.GetName(), "DEBUG")
	return akNearestLOSActor
endfunction
