// -sam2099


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Player/AuraPlayerController.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute());

	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPen, GetArmorPenAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CritChance, GetCritChanceAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CritDamage, GetCritDamageAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CritResist, GetCritResistAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegen, GetHealthRegenAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegen, GetManaRegenAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute());

	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane, GetArcaneResistAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetFireResistAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, GetLightningResistAttribute());
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistAttribute());
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritResist, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	// Vitals
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	// Resistances
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResist, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResist, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResist, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResist, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, NewValue));
	}
	if (Attribute == GetMaxManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, NewValue));
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		if (ICombatInterface::Execute_IsDead(Props.TargetCharacter))
		{
			return;
		}
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void UAuraAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0.f;
		if (!bFatal)
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
			Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);

			const FVector& KnockbackForce = UAuraAbilitySystemLibrary::GetKnockbackForce(Props.EffectContextHandle);
			if (!KnockbackForce.IsNearlyZero(1.f))
			{
				Props.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
			}
		}
		else
		{
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
			if (CombatInterface)
			{
				CombatInterface->Die(UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));
			}
			SendXPEvent(Props);
		}

		const bool bBlock = UAuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bCrit = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCrit);
		if (UAuraAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
		{
			Debuff(Props);
		}
	}
}

void UAuraAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Incoming XP Changed"));

	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);
		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(
			Props.SourceCharacter, CurrentXP + LocalIncomingXP);

		const int32 NumLevelUps = NewLevel - CurrentLevel;
		if (NumLevelUps > 0)
		{
			int32 AttributePointsReward = IPlayerInterface::Execute_GetAttributePointsReward(
				Props.SourceCharacter, CurrentLevel);
			int32 SpellPointsReward = IPlayerInterface::Execute_GetSpellPointsReward(
				Props.SourceCharacter, CurrentLevel);

			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointsReward);

			bTopOffHealth = true;
			bTopOffMana = true;

			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}

		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}

void UAuraAttributeSet::Debuff(const FEffectProperties& Props)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);

	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);

	FInheritedTagContainer TagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& Component = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	TagContainer.Added.AddTag(GameplayTags.DamageTypesToDebuffs[DamageType]);
	Component.SetAndApplyTargetTagChanges(TagContainer);

	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = UAuraAttributeSet::GetIncomingDamageAttribute();

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1))
	{
		FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContext.Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		AuraContext->SetDamageType(DebuffDamageType);
		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}


void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& Props, const float Damage, bool bBlockedHit,
                                         bool bCritHit) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.SourceCharacter->GetController()))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCritHit);
			return;
		}
		if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.TargetCharacter->GetController()))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCritHit);
		}
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		const int32 XPReward = UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(
			Props.TargetCharacter, TargetClass, TargetLevel);

		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter,
		                                                         GameplayTags.Attributes_Meta_IncomingXP, Payload);
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
                                            FEffectProperties& EffectProperties) const
{
	// source is the causer, target is who got hit with it (it will be whoever has this attribute set)

	EffectProperties.EffectContextHandle = Data.EffectSpec.GetContext();
	EffectProperties.SourceASC = EffectProperties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(EffectProperties.SourceASC) && EffectProperties.SourceASC->AbilityActorInfo.IsValid() &&
		EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.SourceAvatarActor = EffectProperties.SourceASC->AbilityActorInfo->AvatarActor.Get();
		EffectProperties.SourceController = EffectProperties.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (EffectProperties.SourceController == nullptr && EffectProperties.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(EffectProperties.SourceAvatarActor))
			{
				EffectProperties.SourceController = Pawn->GetController();
			}
		}
		if (EffectProperties.SourceController)
		{
			EffectProperties.SourceCharacter = Cast<ACharacter>(EffectProperties.SourceController->GetCharacter());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		EffectProperties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		EffectProperties.TargetCharacter = Cast<ACharacter>(EffectProperties.TargetAvatarActor);
		EffectProperties.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectProperties.TargetAvatarActor);
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPen(const FGameplayAttributeData& OldArmorPen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPen, OldArmorPen);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CritChance(const FGameplayAttributeData& OldCritChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritChance, OldCritChance);
}

void UAuraAttributeSet::OnRep_CritDamage(const FGameplayAttributeData& OldCritDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritDamage, OldCritDamage);
}

void UAuraAttributeSet::OnRep_CritResist(const FGameplayAttributeData& OldCritResist) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritResist, OldCritResist);
}

void UAuraAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegen, OldHealthRegen);
}

void UAuraAttributeSet::OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegen, OldManaRegen);
}

void UAuraAttributeSet::OnRep_FireResist(const FGameplayAttributeData& OldFireResist) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResist, OldFireResist);
}

void UAuraAttributeSet::OnRep_ArcaneResist(const FGameplayAttributeData& OldArcaneResist) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResist, OldArcaneResist);
}

void UAuraAttributeSet::OnRep_PhysicalResist(const FGameplayAttributeData& OldPhysicalResist) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResist, OldPhysicalResist);
}

void UAuraAttributeSet::OnRep_LightningResist(const FGameplayAttributeData& OldLightningResist) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResist, OldLightningResist);
}
