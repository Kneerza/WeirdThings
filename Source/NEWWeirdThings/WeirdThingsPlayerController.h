// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Action.h"
#include "ItemTemplate.h"
#include "WeirdThingsPlayerController.generated.h"

UENUM(BlueprintType)
enum class ETimeOfDay : uint8
{
	Morning,
	Noon,
	Evening,
	Night
};

class UPrimitiveComponent;
class ALocationTemplate;
class AWTPlayerCharacter;
class AItemTemplate;
class AActor;
class UClass;
class UDirectionalLightComponent;
class UMaterial;
class ADirectionalLight;
class AAttackDefenseActor;
class UDeckManager;
class AEncounter;
class AEncounter_Bad;
class AEncounter_Dead;
class AEncounter_Good;
class ACombatManager;



/**
 *
 */
UCLASS()
class NEWWEIRDTHINGS_API AWeirdThingsPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	AWeirdThingsPlayerController();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		TArray<FString> Message;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		bool IsGameFinished = false;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		bool IsGameLost = false;


	UPROPERTY(BlueprintReadWrite, Category = Setup)
	FString RollVsDurabilityString = "Roll 0 vs 0 Durability";










	//---------------- Click response ----------------

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		AWTPlayerCharacter* pSelectedCharacter;

	bool AreClickEventsDisabled = false;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void SelectCharacter(AActor* CharacterToSelectActor);
	UFUNCTION(BlueprintCallable, Category = "Custom")
		void DeselectCharacter(AActor* CharacterToDeselectActor);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void Encounter_BadRightClickResponse(AEncounter_Bad* ClickedEncounter_Bad);
	UFUNCTION(BlueprintCallable, Category = "Custom")
		void Encounter_BadLeftClickResponse(AEncounter_Bad* ClickedEncounter_Bad);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void Encounter_GoodRightClickResponse(AEncounter_Good* ClickedEncounter_Good);
	UFUNCTION(BlueprintCallable, Category = "Custom")
		void Encounter_GoodLeftClickResponse(AEncounter_Good* ClickedEncounter_Good);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void Encounter_DeadRightClickResponse(AEncounter_Dead* ClickedEncounter_Dead);
	UFUNCTION(BlueprintCallable, Category = "Custom")
		void Encounter_DeadLeftClickResponse(AEncounter_Dead* ClickedEncounter_Dead);
	
	UFUNCTION(BlueprintCallable, Category = "Custom")
		void ActionLeftClickResponse(AAction* ClickedAction);
	UFUNCTION(BlueprintCallable, Category = "Custom")
		void ActionRightClickResponse(AAction* ClickedAction);

	void ClickedActionHandle(AAction* CurrentAction);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void LocationRightClickResponse(ALocationTemplate* ClickedLocation);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void InteractiveLocationDecorationRightClickResponse(AInteractiveLocationDecoration* ClickedILD);
	UFUNCTION(BlueprintCallable, Category = "Custom")
		void InteractiveLocationDecorationLeftClickResponse(AInteractiveLocationDecoration* ClickedILD);
	//------------------------------------------------------




	//------------ Goals --------------

	int32 CharactersFoodInPlay = 0;
	int32 FoodRequired = 3;

	int32 CharactersWoodInPlay = 0;
	int32 WoodRequired = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<FString> Goals = { "","","" };

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		bool FoodGoalCompleted = false;
	UPROPERTY(BlueprintReadWrite, Category = Setup)
		bool FoodGoalFailed = false;
	UPROPERTY(BlueprintReadWrite, Category = Setup)
		bool WoodGoalCompleted = false;
	UPROPERTY(BlueprintReadWrite, Category = Setup)
		bool WoodGoalFailed = false;

	void UpdateCharactersFoodInPlay();
	void UpdateCharactersWoodInPlay();
	void UpdateGameGoals();
	//-------------------------------




	//---------------------- Currently hovered by mouse actors ------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		AAction* CurrentlyHoveredByMouseAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		AEncounter_Good* CurrentlyHoveredByMouseEncounter_Good = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		AEncounter_Bad* CurrentlyHoveredByMouseEncounter_Bad = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		AEncounter_Dead* CurrentlyHoveredByMouseEncounter_Dead = nullptr;

	void SetCurrentlyHoveredByMouseAction(bool IsHovered, AAction* ActionToSet);
	void SetCurrentlyHoveredByMouseEncounter_Good(bool IsHovered, AEncounter_Good* Encounter_GoodToSet);
	void SetCurrentlyHoveredByMouseEncounter_Bad(bool IsHovered, AEncounter_Bad* Encounter_BadToSet);
	void SetCurrentlyHoveredByMouseEncounter_Dead(bool IsHovered, AEncounter_Dead* Encounter_DeadToSet);
	//---------------------------------------------------------------------------------




	//------------------------------- Movements ---------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		float CharacterMovementLimit = 3000.f;

	void Move_Encounter_Dead(AEncounter_Dead* Encounter_DeadToMove);
	void MoveCharacter(AWTPlayerCharacter* CharacterToMove, ALocationTemplate* LocationToMoveTo);
	void TeleportCharacter(AWTPlayerCharacter* CharacterToMove, AActor* LocationToMoveToActor);
	//---------------------------------------------------------------------------------------------




	//------------------------------- Combat ---------------------------------

	AEncounter* CombatInitiator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<AAttackDefenseActor> AttackDefenceActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<ACombatManager> CombatManagerClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		bool bIsCombatOn = false;

	bool bIsCharacterPickingToFight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		AWTPlayerCharacter* CharacterPickingToFight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<AWTPlayerCharacter*> PlayersChosenToFight = { nullptr, nullptr, nullptr, nullptr, nullptr };

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void InitiateCombat(AWTPlayerCharacter* Initiator);

	void InitiateCombat(AEncounter* Initiator);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void EndCombat();

	void Encounter_DeadLookForPlayerToAttack(AEncounter_Dead* Encounter_Dead);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void Combat(ACombatManager* CombatManager);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void FleeFromCombat(AWTPlayerCharacter* FleeingCharacter);

	void RefreshCombatManager(ACombatManager* CombatManagerToRefresh);
	//------------------------------------------------------------------------




	//------------------------------- Items -------------------------------------------

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void UseItem(AItemTemplate* ItemToUse, AWTPlayerCharacter* ItemOwner);

	UFUNCTION(BlueprintCallable, Category = "Custom")
	bool DropItemOnLocation(AActor* LocationToDropItemOn, TSubclassOf<AAction> ActionItemToDropClass);

	bool PassItemToPlayer(EItemValue ItemValue, int32 Modifier);

	bool PassItemToPlayer(AItemTemplate* ItemsToPick);

	bool PassItemToPlayer(TSubclassOf<AItemTemplate> ItemToPickClass);

	void ItemDurabilityCheck(AWTPlayerCharacter* ItemOwner, AItemTemplate* ItemToCheck, EItemType ItemTypeToCheck);


	AItemTemplate* ItemDurabilityCheck(AWTPlayerCharacter* ItemOwner, AItemTemplate* ItemToCheck);
	//---------------------------------------------------------------------------------



	//------------------------------- Spawning ----------------------------------------


	UPROPERTY(EditAnywhere, Category = Setup)
		FVector SpawnedLocationOffsetY = FVector(0.f, 3000.f, 0.f);

	UPROPERTY(EditAnywhere, Category = Setup)
		FVector SpawnedLocationOffsetZ = FVector(0.f, 0.f, 2000.f);

	UPROPERTY(EditAnywhere, Category = Setup)
		float LocationsRowLimit = 10000.f;

	
	ALocationTemplate* SpawnLocation(AAction* Action, bool IsSpawningOnRight, bool IsPlotLocation);
	AItemTemplate* SpawnItem(TSubclassOf<AItemTemplate> ItemToSpawnClass);
	ACombatManager* SpawnCombatManager(ALocationTemplate* CurrentLocationOfCombat, bool IsInitiatedByEncounter);// , AActor* CombatInstigator);
	bool SpawnEnemy(AAction* ActionInstigator);
	bool SpawnGoodEnc(AAction* ActionInstigator);
	//---------------------------------------------------------------------------------




	//------------------------------- Actions -----------------------------------------

	bool Trade(EActionType ResultOfTrading, EActionLockType ItemRequiredToTrade, bool IsRight);
	bool PerformAction(AAction* Action, int32 Modifier);
	void TryToUnlock(AAction* CurrentAction);

	void DeactivateAction(AActor* ActionToDeactivate);
	void ActivateAction(AActor* ActionToActivate);

	bool CanLeftActiveItemUnlock(EItemType BackpackItemType);
	bool CanRightActiveItemUnlock(EItemType BackpackItemType);

	bool CanLeftActiveItemUnlock(EItemValue BackpackItemValue);
	bool CanRightActiveItemUnlock(EItemValue BackpackItemValue);

	bool FindAndUseItemToUnlock(EItemType BackpackItemType);
	bool FindAndUseItemToUnlock(EItemValue BackpackItemValue);

	FTransform GetAvailableSocketDynamicPlayerActionTransform(AActor* LocationWithSocketActor);
	void CreateDynamicAction(AActor* CurrentLocationActor, TSubclassOf<AAction> ActionClass, AEncounter_Dead* EntangledDead);

	void ActivateEntangledILD(AAction* ActionEntangledWithILD);
	void DeactivateEntangledILD(AAction* ActionEntangledWithILD);
	void CreateDoor(AActor* LocationWithDoorCreatedActor, TSubclassOf<AInteractiveLocationDecoration> DoorToCreateClass, TSubclassOf<AAction> TeleportActionToCreateClass, AActor* LocationInstigatorActor);

	// Letting the location actor know what child action is forced if any
	void SetForcedActionForLocation(AActor* LocationToSetForcedActionOn, AActor* ActionToSetAsForced);

	void EntangleActionWithActor(UChildActorComponent* Action, UChildActorComponent* InteractiveLocationDecoration);
	//---------------------------------------------------------------------------------




	//------------------------------- Day/Night cycle ---------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		ETimeOfDay CurrentTimeOfDay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<ADirectionalLight> SunlightMorningClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<ADirectionalLight> SunlightNoonClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<ADirectionalLight> SunlightEveningClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<ADirectionalLight> SunlightNightClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		ADirectionalLight* SunlightMorning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		ADirectionalLight* SunlightNoon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		ADirectionalLight* SunlightEvening;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		ADirectionalLight* SunlightNight;

	void SetTimeMorning();
	void SetTimeNoon();
	void SetTimeEvening();
	void SetTimeNight();


	//---------------------------------------------------------------------------------




	//------------------------------- Character interaction ---------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		AWTPlayerCharacter* PlayerCharacterRef = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void RefreshCharacterMP();

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void RemoveActionPointsFromCharacter(AWTPlayerCharacter* AffectedCharacter, int32 AmountToRemove);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		bool ConsumeFood(int32 FoodAmountToConsume, AWTPlayerCharacter* AffectedCharacter, int32 ActionPointsRequired);
	bool ConsumeWood(int32 WoodAmountToConsume, AWTPlayerCharacter* AffectedCharacter, int32 ActionPointsRequired);
	bool RemoveFood(int32 FoodAmountToConsume, AWTPlayerCharacter* AffectedCharacter, int32 ActionPointsRequired);
	bool GetFood(int32 FoodAmountToGet);
	bool GetWood(int32 WoodAmountToGet);

	void AddActionPointToEveryCharacter();

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void MakeCampFire(AWTPlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void Sleep(AWTPlayerCharacter* PlayerCharacter);
	//---------------------------------------------------------------------------------




	//----------------------------- Actors currently in play --------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<AWTPlayerCharacter*> PlayerCharacters;

	TArray<AActor*> AllLocationsInPlay;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
	TArray<AEncounter_Dead*> Encounter_DeadsInPlay;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
	TArray<AEncounter_Bad*> Encounter_BadInPlay;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
	TArray<AEncounter_Good*> Encounter_GoodInPlay;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
	TArray<AActor*> CampfiresInPlay;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
	TArray<ACombatManager*> CombatManagersInPlay;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		TArray<AWTPlayerCharacter*> SurvivedCharacters;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
		TArray<AWTPlayerCharacter*> DiedCharacters;
	//---------------------------------------------------------------------------------




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<AActor> CampFireClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UDeckManager* DeckManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<AActor> BurningEffectClass = nullptr;

private:

};