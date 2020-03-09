// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action.h"
#include "Encounter.h"
#include "Encounter_Good.generated.h"

/**
 * 
 */

UCLASS()
class NEWWEIRDTHINGS_API AEncounter_Good : public AEncounter
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UTexture2D* ItemToSell = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<AItemTemplate> ItemToSellClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
		EActionType LBTradingAction = EActionType::Get_Wood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
		EActionType RBTradingAction = EActionType::Get_Wood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		EActionLockType LBTradingRequiredItem = EActionLockType::Need_Item_C;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		EActionLockType RBTradingRequiredItem = EActionLockType::Need_Item_C;
	
	UFUNCTION(BlueprintCallable, Category = "Custom")
		void SetIsHovered(bool IsHovered);
	
};
