// -sam2099


#include "AbilitySystem/Abilities/AuraFirebolt.h"


FString UAuraFirebolt::GetDescription(int32 Level)
{
	const float ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(
			TEXT(
				"<Title>FIRE BOLT</>"
				"\n<Small>Level: </><Level>%.1d</>"
				"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
				"\n<Small>Cooldown: </><Cooldown>%.1f</>"
				"\n\nLaunches a bolt of fire, exploding on impact and dealing <Damage>%d</> fire damage with a chance to burn"),
			Level, ManaCost, Cooldown, ScaledDamage);
	}
	else
	{
		return FString::Printf(
			TEXT(
				"<Title>FIRE BOLT</>"
				"\n<Small>Level: </><Level>%.1d</>"
				"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
				"\n<Small>Cooldown: </><Cooldown>%.1f</>"
				"\n\nLaunches %d bolts of fire, exploding on impact and dealing <Damage>%d</> fire damage with a chance to burn"),
			Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), ScaledDamage);
	}
}

FString UAuraFirebolt::GetNextLevelDescription(int32 Level)
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
				"\n\nLaunches %d bolts of fire, exploding on impact and dealing <Damage>%d</> fire damage with a chance to burn"),
			Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), ScaledDamage);
}
