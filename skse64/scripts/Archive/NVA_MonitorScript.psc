scriptname NpcNameFetcher extends ActiveMagicEffect

Utility Property NVA Auto
FormList Property NPCs Auto

Actor MySelf
string MyName

event OnEffectStart(Actor akTarget, Actor akCaster)
	MySelf = akTarget
	MyName = MySelf.GetName()
	
	If (NPCs.HasForm(MySelf as ObjectReference))
		; This shouldn't happen now -- fixed by adding actor as ObjectReference
		NVA.LogEntry("Grammar already has actor " + MyName + "!", "WARNING")
	endif
	
	NPCs.AddForm(MySelf as ObjectReference)
	
	If (!self.AddGrammar())
		self.Dispel()
	endif
	; No need to register for updates as long as we add to grammar here
endevent

event OnEffectFinish(Actor akTarget, Actor akCaster)
	NVA.LogEntry("Actor " + MyName + " removed from grammar.", "DEBUG")
	
	; IMPORTANT NOTE: Make sure we track HOW MANY of a particular name are in the grammar library
	; so that we handle multiple NPC's with the same name properly
	
	;int handle = Modevent.Create("NVA_OnRemoveGrammar")
	;Modevent.Push(handle, MyName)
	
	;return Modevent.Send(handle)
endevent

function AddGrammar()
	NVA.LogEntry("Actor " + MyName + " added to grammar.", "DEBUG")
	NPCs.RemoveAddedForm(MySelf as ObjectReference)
	
	;int handle = Modevent.Create("NVA_OnAddGrammar")
	;Modevent.Push(handle, MyName)
	
	;return Modevent.Send(handle)
	return true
endfunction