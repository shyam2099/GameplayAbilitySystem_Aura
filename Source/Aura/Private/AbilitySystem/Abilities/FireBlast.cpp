// -sam2099


#include "AbilitySystem/Abilities/FireBlast.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"

FString UFireBlast::GetDescription(int32 Level)
{
	const float ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(
		TEXT(
			"<Title>FIREBLAST</>"
			"\n<Small>Level: </><Level>%.1d</>"
			"\n<Small>Mana Cost: </><ManaCost>%.1f</>"
			"\n<Small>Cooldown: </><Cooldown>%.1f</>"
			"\n\nLaunches %d fireballs, each coming back and exploding upon return and causing <Damage>%d</> radial fire damage with a chance to burn."),
		Level, ManaCost, Cooldown, NumFireBalls, ScaledDamage);
}

FString UFireBlast::GetNextLevelDescription(int32 Level)
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
			"\n\nLaunches %d fireballs, each coming back and exploding upon return and causing <Damage>%d</> radial fire damage with a chance to burn."),
		Level, ManaCost, Cooldown, NumFireBalls, ScaledDamage);
}

TArray<AAuraFireBall*> UFireBlast::SpawnFireballs()
{
	TArray<AAuraFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);
	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireballClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();

		FireBall->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->SetOwner(GetAvatarActorFromActorInfo());
		
		FireBalls.Add(FireBall);

		FireBall->FinishSpawning(SpawnTransform);
	}
	return FireBalls;
}
