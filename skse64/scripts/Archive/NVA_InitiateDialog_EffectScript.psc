ScriptName NVA_Adjacent_ApplyScript extends ActiveMagicEffect

Utility Property NVA Auto
Spell Property MonitorAbility Auto
Actor Property PlayerRef Auto

event OnEffectStart(Actor akTarget, Actor akCaster)
	if (!PlayerRef.IsSneaking())
		NVA.LogEntry("Attempting traditional activation with " + npcName, "DEBUG")
		if (akActor.Activate(PlayerRef, false))
			NVA.LogEntry("Activation with " + akActor.GetName() + " successful!", "DEBUG")
			return true
		endif
		
		NVA.LogEntry("Traditional activation with " + akActor.GetName() + " failed!", "WARNING")
	endif
	
	NVA.LogEntry("Attempting AI package activation with " + akActor.GetName(), "DEBUG")
	; Player was either sneaking or activate failed for some reason
	; Use Scene.Start() method, quest alias, AI Package override instead
	; Add ActiveMagicEffect to NPC that adds to QuestAlias
	
	return false
endevent
