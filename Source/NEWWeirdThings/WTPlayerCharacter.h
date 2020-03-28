// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WTPlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EDurabilityState : uint8
{
	Injury,
	Exhaustion,
	Insanity,
	Hunger,
	Empty
};

class AActor;
class AAction;
class AWeirdThingsPlayerController;
class AItemTemplate;
class AEncounter;
class UWidgetComponent;
class UAttackDefenseComponent;
class UPaperSprite;
class UPaperSpriteComponent;
class UPaperFlipbookComponent;
class UPaperFlipbook;
class UTexture2D;
class ACombatManager;

UCLASS()
class NEWWEIRDTHINGS_API AWTPlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWTPlayerCharacter();

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		bool IsSurvived = false;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		ACombatManager* CurrentCombatManager = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		bool IsDied = false;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
	bool IsInCombat = false;
	UPROPERTY(BlueprintReadWrite, Category = Setup)
	bool IsPickingEnemyToFight = false;
	bool IsSelectedForCombat = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UTexture2D* CharacterPortrait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UPaperSpriteComponent* DiedCharacterSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ActionPoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CurrentActionPoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MovementPoints = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		AActor* CurrentLocation = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		UAttackDefenseComponent* pAttackDefenseComponent;

	UPROPERTY(BlueprintReadOnly, Category = "State")
		TArray<EDurabilityState> Injuries;

	UPROPERTY(EditAnywhere, Category = Setup)
		UMaterialInterface* SelectedCharacterMaterial;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		UStaticMeshComponent* pCone;

	AActor* HiredCompanion = nullptr;

	AWeirdThingsPlayerController* PlayerController = nullptr;

	bool SetHiredCompanion(AActor* CompanionToHire);

	USceneComponent* pRootComponent;

	UPaperSprite* SelectingArrowSprite;
	UPaperSprite* SelectingArrowForCombatSprite;
	UPaperSprite* SelectingArrowForPickingEnemyToFight;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
	//	UWidgetComponent* pHealthWidget;

	UMaterialInterface* UnselectedCharacterMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<AItemTemplate*> Backpack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		UPaperSpriteComponent* SelectingArrow;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
	AItemTemplate* LeftActiveItem = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
	AItemTemplate* RightActiveItem = nullptr;

	AItemTemplate* ItemPickedForFight = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
	AEncounter* CurrentEnemyToAttack = nullptr;

	float DamageWidgetAlphaChannel = 1;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		bool DoesNeedToConsumeFood = false;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		bool DoesNeedToSleep = false;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		bool DoesNeedFire = false;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	//	int32 HealthPoints = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		int32 MaxDamage = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		int32 Food = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		int32 Wood = 2;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void RefreshItems(); // called when need to update Items Stats 

	UFUNCTION(BlueprintCallable, Category = "Custom")
		UTexture2D* GetCharacterPortrait();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<AAction> ActionClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UPaperFlipbookComponent* AvatarComponent;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	//	UPaperFlipbook* AvatarFlipbook;

	//UFUNCTION(BlueprintCallable, Category = "Custom")
	//	void CreateAvatar();

	//void UpdateAvatar();

	void GetItem(AItemTemplate* ItemToPick);

	void GetInjury(int32 InjuryAmountToGet);

	void GetHunger(int32 HungerAmountToGet);

	void GetInsanity(int32 InsanityAmountToGet);

	void GetExhaustion(int32 ExhaustionAmountToGet);

	void GetActionPoints(int32 ActionPointsAmountToGet);

	void RemoveInsanity(int32 InsanityAmountToRemove);

	void RemoveHunger(int32 HungerAmountToRemove);

	bool RemoveInjury(int32 InjuryAmountToRemove);

	void SortInjuries();

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void RemoveExhaustion(int32 ExhaustionAmountToRemove);

	void SetSelected(bool IsSelected);
	void SetSelectedForCombat(bool IsSelected, AItemTemplate* ItemPicked, AEncounter* CurrentEnemy);
	void SetSelectedForPickingEnemy(bool IsSelected);

	void Survive();
	void Die();

	//	void ShowDamage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};


