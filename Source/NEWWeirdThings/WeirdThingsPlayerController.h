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
class AArrowTemplate;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<AActor> CampFireClassToSpawn;

	//------------------ Sunlight -----------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<ADirectionalLight> SunlightMorningClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<ADirectionalLight> SunlightNoonClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<ADirectionalLight> SunlightEveningClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<ADirectionalLight> SunlightNightClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		ADirectionalLight* SunlightMorning;// = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		ADirectionalLight* SunlightNoon;// = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		ADirectionalLight* SunlightEvening;// = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		ADirectionalLight* SunlightNight;// = nullptr;
	//---------------------------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UDeckManager* DeckManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<AAttackDefenseActor> AttackDefenceActorClass;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	//	UPaperFlipbook* FlipbookToSet;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void LeftClickEvents();

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void RightClickEvents();

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void GetComponentUnderCursor(AActor* &ClickedActor, FString &ClickedActorClassName);
	/*
	UFUNCTION(BlueprintCallable, Category = "Custom")
		void GetComponentUnderCursorWhileCharacterSelected();
*/
	UFUNCTION(BlueprintCallable, Category = "Custom")
		void RefreshCharacterMP();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		AWTPlayerCharacter* PlayerCharacterRef = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	//	int32 ResultPlayerDamage;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	//	int32 ResultEnemyDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<ADirectionalLight> DirectionalLight_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<AWTPlayerCharacter*> PlayerCharacters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<AActor> BurningEffectClass = nullptr;

	TArray<ALocationTemplate*> AllLocationsInPlay;
	
	TArray<AEncounter_Dead*>Encounter_DeadsInPlay;

	TArray<AEncounter_Bad*>Encounter_BadInPlay;

	void GetCurrentLocationOfActor(AActor* Actor, ALocationTemplate* &CurrentLocation);

	void MoveCharacter(AWTPlayerCharacter* CharacterToMove, ALocationTemplate* LocationToMoveTo);
	void TeleportCharacter(AWTPlayerCharacter* CharacterToMove, ALocationTemplate* LocationToMoveTo);

	bool SpawnEnemy(AAction* ActionInstigator);
	ALocationTemplate* SpawnLocation(AAction* Action, bool IsSpawningOnRight, bool IsPlotLocation);
	bool SpawnGoodEnc(AAction* ActionInstigator);

	void ClickedArrowTemplateHandle(AArrowTemplate* ClickedArrow);
	void ClickedActionHandle(AAction* CurrentAction);

	UPROPERTY(EditAnywhere, Category = Setup)
		FVector SpawnedLocationOffsetY = FVector(0.f, 3000.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<FString> Goals = { "","","" };

	UPROPERTY(EditAnywhere, Category = Setup)
		FVector SpawnedLocationOffsetZ = FVector(0.f, 0.f, 2000.f);

	UPROPERTY(EditAnywhere, Category = Setup)
		float LocationsRowLimit = 65000.f;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void SelectCharacter(AActor* CharacterToSelect);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void DeselectCharacter(AActor* CharacterToDeselect);

	void Combat(AWTPlayerCharacter* PlayerCharacter, AEncounter* Enemy);
	UFUNCTION(BlueprintCallable, Category = "Custom")
		void InitiateCombat();

		void InitiateCombat(AEncounter* Initiator);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void EndCombat();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		AAction* CurrentlyHoveredByMouseAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		AEncounter_Good* CurrentlyHoveredByMouseEncounter_Good = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		AEncounter_Bad* CurrentlyHoveredByMouseEncounter_Bad = nullptr;

	void SetCurrentlyHoveredByMouseAction(bool IsHovered, AAction* ActionToSet);
	void SetCurrentlyHoveredByMouseEncounter_Good(bool IsHovered, AEncounter_Good* Encounter_GoodToSet);
	void SetCurrentlyHoveredByMouseEncounter_Bad(bool IsHovered, AEncounter_Bad* Encounter_BadToSet);

	AEncounter* CombatInitiator;

	void AttackDefenseEvent(AWTPlayerCharacter* Attacker, AEncounter* Defender, bool IsFightingBack);
	void AttackDefenseEvent(AEncounter* Attacker, AWTPlayerCharacter* Defender);
	void FightBack(AEncounter* Enemy, AWTPlayerCharacter* PlayerCharacter);

	bool AreOnSameLocation(AActor* Actor1, AActor* Actor2);

	bool Trade(EActionType ResultOfTrading, EActionLockType ItemRequiredToTrade);
	bool PerformAction(AAction* Action, int32 Modifier);
	void TryToUnlock(AAction* CurrentAction);

	bool CanFirstActiveItemUnlock(EItemType BackpackItemType);
	bool CanSecondActiveItemUnlock(EItemType BackpackItemType);

	bool FindAndUseItemToUnlock(EItemType BackpackItemType);
	bool FindAndUseItemToUnlock(EItemValue BackpackItemValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		bool bIsCombatOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	AWTPlayerCharacter* CharacterPickingToFight = nullptr;

	bool bIsCharacterPickingToFight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	TArray<AWTPlayerCharacter*> PlayersChosenToFight = { nullptr, nullptr, nullptr, nullptr, nullptr };

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void PlayerCharactersAttack(TArray<AWTPlayerCharacter*> CharactersAttackers); //, AEncounter* Defender);

	void PassItemToPlayer(EItemValue ItemValue);

	void PassItemToPlayer(AItemTemplate* ItemsToPick);

	void ItemDurabilityCheck(AWTPlayerCharacter* ItemOwner, AItemTemplate* ItemToCheck, EItemType ItemTypeToCheck);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		bool ConsumeFood(int32 FoodAmountToConsume, AWTPlayerCharacter* AffectedCharacter, int32 ActionPointsRequired);
	bool ConsumeWood(int32 WoodAmountToConsume, AWTPlayerCharacter* AffectedCharacter, int32 ActionPointsRequired);

	bool RemoveFood(int32 FoodAmountToConsume, AWTPlayerCharacter* AffectedCharacter, int32 ActionPointsRequired);

	bool GetFood(int32 FoodAmountToGet);
	bool GetWood(int32 WoodAmountToGet);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	//ALocationTemplate* SelectedCharacter_CurrentLocation;

	UPROPERTY(BlueprintReadWrite, Category = Setup)
	AWTPlayerCharacter* pSelectedCharacter;

	ADirectionalLight* pDirectionalLight;
	UDirectionalLightComponent* pDirectionalLightComponent;

	AActor* pClickedActor;
	FString ClickedActorClassName;

	AActor* pRightClickedActor;
	FString RightClickedActorClassName;

	UPrimitiveComponent* pComponentUnderCursor;

	AArrowTemplate* pArrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		ETimeOfDay CurrentTimeOfDay;

	FVector OnLocationOffset = FVector(-400.f, -400.f, -10.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		float CharacterMovementLimit = 3000.f;

	bool CharacterIsSelected = false;

	bool AreClickEventsDisabled = false;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void MakeCampFire(AWTPlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void Sleep(AWTPlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void UseItem(AItemTemplate* ItemToUse, AWTPlayerCharacter* ItemOwner);


private:

	void SetTimeMorning();
	void SetTimeNoon();
	void SetTimeEvening();
	void SetTimeNight();

	void AddActionPointToEveryCharacter();
};