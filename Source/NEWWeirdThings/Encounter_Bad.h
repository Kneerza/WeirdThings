// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Encounter.h"
#include "Encounter_Bad.generated.h"

/**
 * 
 */
UCLASS()
class NEWWEIRDTHINGS_API AEncounter_Bad : public AEncounter
{
	GENERATED_BODY()


		UFUNCTION(BlueprintCallable, Category = "Custom")
			void SetIsHovered(bool IsHovered);
		
	virtual void Deactivate() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
