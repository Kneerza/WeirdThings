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
	ArrowRight_Good,
	ArrowRight_Bad,
	ArrowRight_Ugly,
	ArrowUp_Plot,
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
	No_Need
};

class USphereComponent;
class UPaperFlipbook;
class UPaperFlipbookComponent;
class AInteractiveLocationDecoration;

UCLASS()
class NEWWEIRDTHINGS_API AAction : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAction();

	UPaperFlipbook* pLockFood;
	UPaperFlipbook* pLockWood;
	UPaperFlipbook* pLockTool;
	UPaperFlipbook* pLockAxe;
	UPaperFlipbook* pLockItem_C;
	UPaperFlipbook* pLockItem_S;
	UPaperFlipbook* pLockItem_G;
	UPaperFlipbook* pLockExhaustion;
	UPaperFlipbook* pLockInsanity;
	UPaperFlipbook* pActionForced;


	UPaperFlipbookComponent* pConnector;
	UPaperFlipbookComponent* pActionForcedComponent;

	AInteractiveLocationDecoration* EntangledInteractiveLocationDecoration = nullptr;

	TArray<UPaperFlipbookComponent*> ActionLock;

	UPROPERTY(EditAnywhere, Category = Setup)
		TArray<EActionLockType> ActionLockType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		USphereComponent* CollisionSphere;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		UPaperFlipbookComponent* ActionFlipBookComponent;

	UPROPERTY(EditAnywhere, Category = Setup)
		bool IsForced = false;

	USceneComponent* pRootComponent;

	UPROPERTY(EditAnywhere, Category = Setup)
		EActionType ActionType = EActionType::Get_Item_C;

	void ConstructActionLocks();  // BeginPlay only

	void Unlock();

	void Deactivate();

	void Activate();

	void GetTypeOfLock();

	AAction* Child = nullptr;

	int32 CurrentLockTypeIndex;
	int32 CurrentLockIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float ActionLocksOrbit = 150.f;
	float ActionLockOffset_X = -20;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
