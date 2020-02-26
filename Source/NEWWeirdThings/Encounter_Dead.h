// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Encounter.h"
#include "Encounter_Dead.generated.h"

/**
 * 
 */

class AAction;
class UPaperFlipbookComponent;
class AWeirdThingsPlayerController;

UCLASS()
class NEWWEIRDTHINGS_API AEncounter_Dead : public AEncounter
{
	GENERATED_BODY()
	

public:
	AEncounter_Dead();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<AAction> ActionClassToSpawn;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		UPaperFlipbookComponent* AwakenedDeadFlipbookComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		UPaperFlipbookComponent* SleepingDeadFlipbookComponent;

	UChildActorComponent* CreatedAction = nullptr;

	AWeirdThingsPlayerController* PlayerController = nullptr;

	bool IsAwake = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		bool IsOnPlot = false;

	void SetAwakened(bool IsAwakened);

	void FindPlayerToAttack();

	void CreateAction();

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void SetAction(UChildActorComponent* ActionToSet);

	void Move();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
