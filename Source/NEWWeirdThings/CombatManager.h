// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackDefenseActor.h"
#include "GameFramework/Actor.h"
#include "CombatManager.generated.h"

class ALocationTemplate;
class AWTPlayerCharacter;
class AWeirdThingsPlayerController;
class AEncounter;
class AAttackDefenseActor;

UCLASS()
class NEWWEIRDTHINGS_API ACombatManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<AAttackDefenseActor> AttackDefenceActorClass;

	AWeirdThingsPlayerController* PlayerController = nullptr;
	ALocationTemplate* CurrentLocation = nullptr;
	AActor* CombatInstigator = nullptr;
	TArray<AWTPlayerCharacter*> CharactersInCombat = {};
	TArray<AEncounter*> EncountersInCombat = {};

	AAttackDefenseActor* SpawnAttackDefenseActor(FVector LocationToSpawn, EDefenseType DefenseTypeToSpawn);
	AAttackDefenseActor* SpawnAttackDefenseActor(
		AAttackDefenseActor* ActorToSpawn,
		AWTPlayerCharacter* CharacterAttacker, 
		FVector LocationToSpawn,
		EAttackType AttackTypeToSpawn
	);
	AAttackDefenseActor* SpawnAttackDefenseActor(
		AAttackDefenseActor* ActorToSpawn,
		AEncounter* EncounterAttacker,
		FVector LocationToSpawn,
		EAttackType AttackTypeToSpawn
	);

	void GetCharactersSelectedForCombat(TArray<AWTPlayerCharacter*> &OUTCharactersSelectedForCombat);
	void GetEncountersReadyForCombat(TArray<AEncounter*> &OUTEncountersReadyForCombat);
	void GetDefenders(TArray<AWTPlayerCharacter*> CharactersSelectedForCombat, TArray<AEncounter*> &OUTDefenders);
	void GetDefenders(TArray<AEncounter*> EncountersReadyForCombat, TArray<AWTPlayerCharacter*> &OUTDefenders);
	void ShowCharactersInCombat();
	void PlayerCharactersAttack();
	void EncountersAttack();

	void Refresh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
