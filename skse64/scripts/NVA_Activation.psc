scriptname NVA_Activation extends ActiveMagicEffect

Utility Property NVA Auto
Actor Property PlayerRef Auto

string NpcName
string ActivatedName

Event OnEffectStart(Actor akTarget, Actor akCaster)
	if !NVA.IsActivating
		return
	endif

	ActivatedName = NVA.ActivatedName
	NpcName = akTarget.GetName()
	
	if activatedName == "" || StringUtil.Find(NpcName, ActivatedName) >= 0
		NVA.LogEntry("Activating " + NpcName + "...", "DEBUG")
		NVA.IsActivating = false
		NVA.InitiateDialog(akTarget)
		NVA.LogEntry("Finished activating " + NpcName, "DEBUG")
	endif
endevent

; Spell:
;   ID: NVA_NearActivationSpell
;   Name: NPC Voice Activation Spell (Near)
;   Type: Spell
;   Casting: Fire and Forget
;   Delivery: Aimed (needs projectile then)?  Target Location?
;   Menu Display Object: None
;   Equip Type: Either Hand
;   Casting Perk: None
;   Range: 10 (in feet)?
;   Disallow Spell Absorb/Reflect: Yes
;   Ignore Resistance: Yes
;   Area Effect Ignores LOS: No
;   PC Start Spell: No
;   No Dual Cast Modifications: Yes
;  	Description: When a recognized voice command is sent, spell is cast to activate a(n) [matching] NPC
;  	Effects:
;	[0] { 
;		Name: NPC Voice Activation Applying Effect (Near)
;		Magnitude: 0
;		Area: 0
;		Duration: 0
;		Cost: 0
;		Magic School: None?
;	}

; MagicEffect:
;	ID: NVA_NearActivationEffect
;	Name: NPC Voice Activation Applying Effect (Near)
;	Effect Archetype: Value Modifier?
;	Casting Type: Fire and Forget
;	Delivery: Target Location (or Projectile?)
;	Magic Skill: None?
;   Minimum Skill Level: 0?
;   Associated Item 1: None?
;   Associated Item 2: None
;   2nd AV Weight: 0   
;	Resist Value: None
;	Perk to Apply: None
;	Base Cost: 0
;	Skill Usage Mult: 0?
;	Taper Duration: 0
;	Taper Weight: 0
;	Taper Curve: 0
;	Hostile: No
;	Detrimental: No
;	Recover: No
;	FX Persist: No
;	Snap to Navmesh: No
;	No Recast: No?
;	No Hit Effect: Yes
;	No Death Dispel: No
;	No Duration: Yes
;	No Hit Event: Yes
;	No Magnitude: Yes
;	No Area: No
;	Painless: Yes
;	Gory Visuals: No
;	Hide in UI: Yes
;	Dispel Effects with these Keywords: No
;	Keywords: None
; 	Counter Effects: None
;	Target Conditions: None?
;	Menu Display Object: None
;	Casting Art: None
;	Casting Light: None
;	Hit Effect Art: None
;	Hit Shader: None
;	Enchant Art: None
;	Enchant Shader: None
;	Enchant Shader: None
;	Projectile: None?  See above.
;	Impact Data Set: None
;	Explosion: None
;	Image Space Mod: None
;	Power Affects Magnitude: No
;	Power Affects Duration: No
;	Area: 0
;	Casting Time: 0.00000
;	Score: 0
;	Delay Time: 0
;	Equip Ability: None
;	Sounds: None
;	Magic item description: When a recognized voice command is sent, spell is cast to activate a(n) [matching] NPC
;   Papyrus Scripts: NVA_Near_Activation_Script.psc (this file)

; Spell:
;   ID: NVA_FarActivationSpell
;   Name: NPC Voice Activation Spell (Far)
;   Type: Spell
;   Casting: Concentration
;   Delivery: Self
;   Menu Display Object: None
;   Equip Type: EitherHand
;   Casting Perk: None
;   Range: N/A
;   Disallow Spell Absorb/Reflect: Yes
;   Ignore Resistance: Yes
;   Area Effect Ignores LOS: No
;   PC Start Spell: No
;   No Dual Cast Modifications: Yes
;  	Description: When a recognized voice command is sent, spell is cast to activate a(n) [matching] NPC
;  	Effects:
;	[0] { 
;		Name: NPC Voice Activation Applying Effect (Far)
;		Magnitude: 25 (is in feet)
;		Area: N/A
;		Duration: 0
;		Cost: 0
;		Magic School: None?
;	}

; MagicEffect:
;	ID: NVA_FarActivationEffect
;	Name: NPC Voice Activation Applying Effect (Far)
;	Effect Archetype: Cloak
;	Casting Type: Constant Effect
;	Delivery: Self
;	Magic Skill: None
;   Minimum Skill Level: 0
;   Associated Item 1: None
;   Associated Item 2: None
;   2nd AV Weight: 0   
;	Resist Value: None
;	Perk to Apply: None
;	Base Cost: 0
;	Skill Usage Mult: 0
;	Taper Duration: 0
;	Taper Weight: 0
;	Taper Curve: 0
;	Hostile: No
;	Detrimental: No
;	Recover: No
;	FX Persist: No
;	Snap to Navmesh: No
;	No Recast: No?
;	No Hit Effect: Yes
;	No Death Dispel: No
;	No Duration: Yes
;	No Hit Event: Yes
;	No Magnitude: Yes
;	No Area: No
;	Painless: Yes
;	Gory Visuals: No
;	Hide in UI: Yes
;	Dispel Effects with these Keywords: No
;	Keywords: None
; 	Counter Effects: None
;	Target Conditions: None?
;	Menu Display Object: None
;	Casting Art: None
;	Casting Light: None
;	Hit Effect Art: None
;	Hit Shader: None
;	Enchant Art: None
;	Enchant Shader: None
;	Enchant Shader: None
;	Projectile: None
;	Impact Data Set: None
;	Explosion: None
;	Image Space Mod: None
;	Power Affects Magnitude: No
;	Power Affects Duration: No
;	Area: 0
;	Casting Time: 0.00000
;	Score: 0
;	Delay Time: 0
;	Equip Ability: None
;	Sounds: None
;	Magic item description: When a recognized voice command is sent, spell is cast to activate a(n) [matching] NPC
;   Papyrus Scripts: NVA_Activation_Script.psc (this file)