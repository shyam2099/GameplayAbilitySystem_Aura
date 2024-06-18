// -sam2099

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilitySelectedSignature, UAuraUserWidget*, AbilityButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGlobeSelectedSignature, FString, DescriptionString, FString, NextLevelDescriptionString);

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

	UFUNCTION(BlueprintCallable, Category = "GAS|Spells Menu")
	void SelectAbility(UAuraUserWidget* AbilityButton, const FGameplayTag& AbilityTag);
        
	UPROPERTY(BlueprintAssignable, Category = "GAS|Spells Menu")
	FAbilitySelectedSignature AbilitySelectedDelegate;
	
	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed(const FGameplayTag& AbilityTag);

	UPROPERTY(BlueprintAssignable)
	FGlobeSelectedSignature GlobeSelectedDelegate;
	
};
