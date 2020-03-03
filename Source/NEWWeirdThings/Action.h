// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Action.generated.h"


UENUM(BlueprintType)
enum class EActionType : uint8
{
	Get_Food,
	Get_Wood,
	Get_Item_C,
	Get_Item_S,
	Get_Item_G,
	Get_EncounterBad,
	Get_EncounterGood,
	Get_EncounterRandom,
	Get_Injury,
	Get_Exhaustion,
	Portrait,
	ArrowRight_Good,
	ArrowRight_Bad,
	ArrowRight_Ugly,
	ArrowUp_Plot,
	Arrow_Move,
	Teleport,
	Relief,
	Fishing,
	Hide,
	Burn
};

UENUM(BlueprintType)
enum class EActionLockType : uint8
{
	Need_Wood,
	Need_Food,
	Need_Item_C,
	Need_Item_S,
	Need_Item_G,
	Need_Injury,
	Need_Insanity,
	Need_Tool,
	Need_Axe,
	Need_Exhaustion,
	Need_Shovel,
	No_Need
};

class USphereComponent;
class UPaperFlipbook;
class UPaperFlipbookComponent;
class AInteractiveLocationDecoration;
class ALocationTemplate;
class AEncounter_Dead;

UCLASS()
class NEWWEIRDTHINGS_API AAction : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAction();

	// --- Action setup parameters ---
	UPROPERTY(EditAnywhere, Category = Setup)
		bool IsForced = false; // Is action happens on it's own or should be activated 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		bool IsWorkedOut = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		bool IsInfinite = false;

	UPROPERTY(EditAnywhere, Category = Setup)
		int32 Modifier = 1;

	UPROPERTY(EditAnywhere, Category = Setup)
		int32 ActionPointsRequired = 1;

	UPROPERTY(EditAnywhere, Category = Setup)
		EActionType ActionType = EActionType::Get_Item_C; // Type of event happening when activated

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh) // TODO Delete?
		USphereComponent* CollisionSphere;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		UPaperFlipbookComponent* ActionFlipBookComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		UPaperFlipbookComponent* ModifierVisual;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		UPaperFlipbookComponent* UpdatedArrowVisual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<AInteractiveLocationDecoration> DoorToCreateClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<AAction> TeleportActionToCreateClass;

	AEncounter_Dead* EntangledDeadEncounter = nullptr;

	// --- Locks for Action ---
	UPROPERTY(EditAnywhere, Category = Setup)
		TArray<EActionLockType> ActionLockType;

		TArray<EActionLockType> StartingActionLockType;

	UPaperFlipbook* pLockFood;
	UPaperFlipbook* pLockWood;
	UPaperFlipbook* pLockTool;
	UPaperFlipbook* pLockAxe;
	UPaperFlipbook* pLockShovel;
	UPaperFlipbook* pLockItem_C;
	UPaperFlipbook* pLockItem_S;
	UPaperFlipbook* pLockItem_G;
	UPaperFlipbook* pLockExhaustion;
	UPaperFlipbook* pLockInsanity;
	UPaperFlipbook* pActionForced;
	

	TArray<UPaperFlipbookComponent*> ActionLock;

	int32 CurrentLockTypeIndex = 0;
	int32 CurrentLockIndex = 0;


	// --- Pointers ----
	UPaperFlipbookComponent* pConnector;
	AAction* FirstActionInChain = nullptr;
	AAction* Child = nullptr;
	UPaperFlipbookComponent* pActionForcedComponent;
	AInteractiveLocationDecoration* EntangledInteractiveLocationDecoration = nullptr;
	ALocationTemplate* LocationArrowPointsTo = nullptr;

	// --- Lock handling functions ---
	void GetTypeOfLock();
	void ConstructActionLocks(); 
	void ConstructModifierVisual();
	void Unlock();


	// --- Action handling functions ---
	void Deactivate();
	void Activate();
	void ForcedActionHandling();

	//ALocationTemplate* LocationTeleportTo = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void UpdateArrowActionVisual();

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void SetEntangledDeadEncounter(AEncounter_Dead* EntangledDeadEncounterToSet);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void SetIsHovered(bool IsHovered);

	void SetTeleport();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// --- Parameters for Locks placing ---
	float ActionLocksOrbit = 150.f;
	float ActionLockOffset_X = -20;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	USceneComponent* pRootComponent;

};
