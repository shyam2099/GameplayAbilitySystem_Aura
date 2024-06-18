// -sam2099


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();

	SpellPointsChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
	{
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
	{
		SpellPointsChangedDelegate.Broadcast(SpellPoints);
	});
}

void USpellMenuWidgetController::SelectAbility(UAuraUserWidget* AbilityButton, const FGameplayTag& AbilityTag)
{
	AbilitySelectedDelegate.Broadcast(AbilityButton);
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	GlobeSelectedDelegate.Broadcast(Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed(const FGameplayTag& AbilityTag)
{
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoint(AbilityTag);
	}
	
}
