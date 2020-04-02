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
	PickUpItem,
	Portrait,
	ArrowRight_Good,
	ArrowRight_Bad,
	ArrowRight_Ugly,
	ArrowUp_Plot,
	Arrow_Move,
	Teleport,
	Relief,
	Heal,
	Fishing,
	Hide,
	Hire,
	Burn,
	Survive,
	No_Action
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
	Need_Weapon,
	No_Need
};

class USphereComponent;
class UPaperFlipbook;
class UPaperFlipbookComponent;
class AInteractiveLocationDecoration;
class AWeirdThingsPlayerController;
class AItemTemplate;
//class ALocationTemplate;

UCLASS()
class NEWWEIRDTHINGS_API AAction : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAction();

	UPROPERTY(BlueprintReadWrite, Category = Custom)
	bool IsDeactivated = false;

	AWeirdThingsPlayerController* PlayerController = nullptr;

	TArray<AAction*> ChildrenOfNexus = {};

	AAction* Nexus;

	// --- Action setup parameters ---
	UPROPERTY(EditAnywhere, Category = Setup)
		bool IsForced = false; // Is action happens on it's own or should be activated 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		bool IsWorkedOut = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		bool IsInfinite = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		bool IsOneTimeUse;

	UPROPERTY(EditAnywhere, Category = Setup)
		int32 Modifier = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<AItemTemplate> ItemToGetClass;

	AActor* EntangledDeadEncounter = nullptr;

	// --- Locks for Action ---
	UPROPERTY(EditAnywhere, Category = Setup)
		TArray<EActionLockType> ActionLockType;

		TArray<EActionLockType> StartingActionLockType;

	TArray<UPaperFlipbookComponent*> ActionLock;

	int32 CurrentLockTypeIndex = 0;
	int32 CurrentLockIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	bool IsLocked = false;


	// --- Pointers ----
	UPaperFlipbookComponent* pConnector;
	AAction* FirstActionInChain = nullptr;
	AAction* Child = nullptr;
	UPaperFlipbookComponent* pActionForcedComponent;
	AActor* EntangledInteractiveLocationDecoration = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = Setup)
	AActor* LocationArrowPointsTo = nullptr;
	//ALocationTemplate* LocationArrowPointsTo = nullptr;

	// --- Lock handling functions ---
	void GetTypeOfLock();
	void ConstructActionLocks(); 
	void ConstructModifierVisual();
	void Unlock();


	// --- Action handling functions ---
	void Deactivate();
	void Activate();
	void SetUpActionAsForced();

	//ALocationTemplate* LocationTeleportTo = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void UpdateArrowActionVisual();

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void SetEntangledDeadEncounter(AActor* EntangledDeadEncounterToSet);


	UFUNCTION(BlueprintCallable, Category = "Custom")
		// Pass information about being activated to WeirdThingsPlayerController
		void SetIsHovered(bool IsHovered); 	

	void SetTeleport(TArray<AActor*> LocationsInPlay);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// --- Parameters for Locks placing ---
	float ActionLocksOrbit = 80.f;
	float ActionLockOffset_X = -20;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	void CreateComponents();
	USceneComponent* pRootComponent;

};
