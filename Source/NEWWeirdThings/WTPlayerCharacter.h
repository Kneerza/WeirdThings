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

//class UMaterialInstanceDynamic;
class UMaterial;
class UMaterialInterface;
class AActor;
class AAction;
class AItemTemplate;
class ALocationTemplate;
class UWidgetComponent;
class UAttackDefenseComponent;
class UArrowComponent;
class UPaperSpriteComponent;
class UPaperFlipbookComponent;
class UPaperFlipbook;
class UTexture2D;

UCLASS()
class NEWWEIRDTHINGS_API AWTPlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWTPlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UTexture2D* CharacterPortrait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ActionPoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CurrentActionPoints = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MovementPoints = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		ALocationTemplate* CurrentLocation = nullptr;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		UAttackDefenseComponent* pAttackDefenseComponent;

	UPROPERTY(BlueprintReadOnly, Category = "State")
		TArray<EDurabilityState> Injuries;

	UPROPERTY(EditAnywhere, Category = Setup)
		UMaterialInterface* SelectedCharacterMaterial;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		UStaticMeshComponent* pCone;

	USceneComponent* pRootComponent;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
	//	UWidgetComponent* pHealthWidget;

	UMaterialInterface* UnselectedCharacterMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<AItemTemplate*> Backpack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		UPaperSpriteComponent* SelectingArrow;

	AItemTemplate* ActiveItem = nullptr;

	float DamageWidgetAlphaChannel = 1;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UPaperFlipbook* AvatarFlipbook;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void CreateAvatar();

	void UpdateAvatar();

	void GetItem(AItemTemplate* ItemToPick);

	void GetInjury(int32 InjuryAmountToGet);

	void GetHunger(int32 HungerAmountToGet);

	void GetInsanity(int32 InsanityAmountToGet);

	void GetExhaustion(int32 ExhaustionAmountToGet);

	void RemoveInsanity(int32 InsanityAmountToRemove);

	void RemoveHunger(int32 HungerAmountToRemove);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void RemoveExhaustion(int32 ExhaustionAmountToRemove);

	void SetSelected(bool IsSelected);

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


