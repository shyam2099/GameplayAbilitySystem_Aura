// -sam2099


#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	const float ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(
			TEXT(
				"<Title>ARCANE SHARDS</>"
				"\n<Small>Level: </><Level>%.1d</>"
				"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
				"\n<Small>Cooldown: </><Cooldown>%.1f</>"
				"\n\nSummons a shard of arcane energy causing radial arcane damage of <Damage>%d</> at the shard origin."),
			Level, ManaCost, Cooldown, ScaledDamage);
	}
	else
	{
		return FString::Printf(
			TEXT(
				"<Title>ARCANE SHARDS</>"
				"\n<Small>Level: </><Level>%.1d</>"
				"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
				"\n<Small>Cooldown: </><Cooldown>%.1f</>"
				"\n\nSummon %d shards of arcane energy causing radial arcane damage of <Damage>%d</> at the shard origin."),
			Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards), ScaledDamage);
	}
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
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
				"\n\nSummon %d shards of arcane energy causing radial arcane damage of <Damage>%d</> at the shard origin."),
			Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards), ScaledDamage);
}