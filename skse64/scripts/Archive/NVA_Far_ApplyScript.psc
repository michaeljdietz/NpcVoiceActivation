ScriptName NVA_Far_ApplyScript extends ActiveMagicEffect

Spell Property FarAbility Auto

Event OnEffectStart(Actor akTarget, Actor akCaster)
		akTarget.AddSpell(FarAbility)
EndEvent