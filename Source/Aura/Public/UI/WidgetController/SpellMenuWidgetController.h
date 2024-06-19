// -sam2099

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilitySelectedSignature, UAuraUserWidget*, AbilityButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGlobeSelectedSignature, FString, DescriptionString, FString, NextLevelDescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassignedSignature, const FGameplayTag&, AbilityTag);


struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatUpdatedSignature SpellPointsChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipSelectionDelegate;

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitForEquipDelegate;

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeReassignedSignature SpellGlobeReassignedDelegate;
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Spells Menu")
	void SelectAbility(UAuraUserWidget* AbilityButton, const FGameplayTag& AbilityTag);
        
	UPROPERTY(BlueprintAssignable, Category = "GAS|Spells Menu")
	FAbilitySelectedSignature AbilitySelectedDelegate;
	
	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed(const FGameplayTag& AbilityTag);

	UPROPERTY(BlueprintAssignable)
	FGlobeSelectedSignature GlobeSelectedDelegate;

	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);
	
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

private:
	FSelectedAbility SelectedAbility = { FAuraGameplayTags::Get().Abilities_Type_None,  FAuraGameplayTags::Get().Abilities_Status_Locked };
	bool bWaitingForEquipSelection = false;
	FGameplayTag SelectedSlot;
	
};
