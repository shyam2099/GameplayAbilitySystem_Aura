// -sam2099


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const float ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(
			TEXT(
				"<Title>ELECTROCUTE</>"
				"\n<Small>Level: </><Level>%.1d</>"
				"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
				"\n<Small>Cooldown: </><Cooldown>%.1f</>"
				"\n\nEmits a beam of lightning, connecting with the target, repeatedly causing <Damage>%d</> lightning damage with a chance to stun."),
			Level, ManaCost, Cooldown, ScaledDamage);
	}
	else
	{
		return FString::Printf(
			TEXT(
				"<Title>ELECTROCUTE</>"
				"\n<Small>Level: </><Level>%.1d</>"
				"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
				"\n<Small>Cooldown: </><Cooldown>%.1f</>"
				"\n\nEmits a beam of lightning, propagating to %d additional targets nearby, causing <Damage>%d</> fire damage with a chance to stun."),
			Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShockTargets - 1), ScaledDamage);
	}
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	const float ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(
			TEXT(
				"<Title>NEXT LEVEL</>"
				"\n<Small>Level: </><Level>%.1d</>"
				"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
				"\n<Small>Cooldown: </><Cooldown>%.1f</>"
				"\n\nEmits a beam of lightning, propagating to %d additional targets nearby, causing <Damage>%d</> fire damage with a chance to stun."),
			Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShockTargets - 1), ScaledDamage);
}