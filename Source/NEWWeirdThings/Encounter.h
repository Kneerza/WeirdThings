// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Encounter.generated.h"

class UWidgetComponent;
class UUserWidget;
class AItemTemplate;
class AAction;
class ALocationTemplate;
class UQuitManagement;
class UAttackDefenseComponent;
class AWeirdThingsPlayerController;
class AWTPlayerCharacter;

UCLASS()
class NEWWEIRDTHINGS_API AEncounter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEncounter();

	//bool IsAggressive = true;

	bool IsInCombat = false;

	AWeirdThingsPlayerController* PlayerController = nullptr;

	AWTPlayerCharacter* CurrentEnemyToAttack = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<TSubclassOf<AItemTemplate>> LootClassesToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
		TArray<TSubclassOf<AAction>> Loot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		UAttackDefenseComponent* pAttackDefenseComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		UQuitManagement* pQuitManagementComponent;

	USceneComponent* pRootComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		UWidgetComponent* pHealthWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		int32 HealthPoints = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	AActor* CurrentLocation = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void LooseHealth();

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void CreateInfoWidget(UUserWidget* WidgetToCreate, FVector2D WidgetOffset);

	void DestroyHealthWidget();

	virtual void Deactivate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	//void InitializeLoot();
	
};
