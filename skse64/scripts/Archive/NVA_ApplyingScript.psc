ScriptName NVA_ApplyingScript extends ActiveMagicEffect

Utility Property NVA Auto
Spell Property MonitorAbility Auto

Event OnEffectStart(Actor akTarget, Actor akCaster)
		akTarget.AddSpell(MonitorAbility)
EndEvent
