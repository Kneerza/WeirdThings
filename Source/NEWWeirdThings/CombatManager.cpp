// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatManager.h"
#include "Timer.h"
#include "AttackDefenseActor.h"
#include "Encounter.h"
#include "Encounter_Good.h"
#include "Encounter_Bad.h"
#include "Encounter_Dead.h"
#include "WeirdThingsPlayerController.h"
#include "LocationTemplate.h"
#include "WTPlayerCharacter.h"
#include "Runtime/Engine/Classes/Engine/World.h"


// Sets default values
ACombatManager::ACombatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACombatManager::BeginPlay()
{
	Super::BeginPlay();
	

	PlayerController = Cast<AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerController->CombatManagersInPlay.Add(this);

	//Refresh();

	UE_LOG(LogTemp, Warning, TEXT("CombatManager is watching"))
}

// Called every frame
void ACombatManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatManager::Refresh()
{

	CharactersInCombat.Empty();
	EncountersInCombat.Empty();

	auto PlayerCharacters = PlayerController->PlayerCharacters;
	auto Encounter_BadInPlay = PlayerController->Encounter_BadInPlay;
	auto Encounter_GoodInPlay = PlayerController->Encounter_GoodInPlay;
	auto Encounter_DeadsInPlay = PlayerController->Encounter_DeadsInPlay;

	if (Encounter_BadInPlay.IsValidIndex(0))
	{
		for (int32 i = 0; i < Encounter_BadInPlay.Num(); i++)
		{
			if (Encounter_BadInPlay[i])
			{
				if (Encounter_BadInPlay[i]->CurrentLocation == CurrentLocation)
				{
					EncountersInCombat.Add(Encounter_BadInPlay[i]);
					Encounter_BadInPlay[i]->IsInCombat = true;
				}
			}
		}
	}

	if (Encounter_DeadsInPlay.IsValidIndex(0))
	{
		for (int32 i = 0; i < Encounter_DeadsInPlay.Num(); i++)
		{
			if (Encounter_DeadsInPlay[i])
			{
				if (Encounter_DeadsInPlay[i]->CurrentLocation == CurrentLocation)
				{
					EncountersInCombat.Add(Encounter_DeadsInPlay[i]);
					Encounter_DeadsInPlay[i]->IsInCombat = true;
				}
			}
		}
	}

	if (Encounter_GoodInPlay.IsValidIndex(0))
	{
		for (int32 i = 0; i < Encounter_GoodInPlay.Num(); i++)
		{
			if (Encounter_GoodInPlay[i])
			{
				if (Encounter_GoodInPlay[i]->CurrentLocation == CurrentLocation)
				{
					EncountersInCombat.Add(Encounter_GoodInPlay[i]);
					Encounter_GoodInPlay[i]->IsInCombat = true;
				}
			}
		}
	}

	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		if (PlayerCharacters[i])
		{
			if (PlayerCharacters[i]->CurrentLocation == CurrentLocation)
			{
				CharactersInCombat.Add(PlayerCharacters[i]);
				if (!(EncountersInCombat.Contains(PlayerCharacters[i]->CurrentEnemyToAttack)))
				{
					PlayerCharacters[i]->SetSelectedForCombat(false, nullptr, nullptr);
				}
				PlayerCharacters[i]->IsInCombat = true;
			}
		}
	}
}

void ACombatManager::ShowCharactersInCombat()
{

	for (int32 i = 0; i < EncountersInCombat.Num(); i++)
	{
		if (EncountersInCombat[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("Encounter in combat: %s"), *EncountersInCombat[i]->GetName())
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Encounter in combat: NONE"))
		}
	}
}

void ACombatManager::GetCharactersSelectedForCombat(TArray<AWTPlayerCharacter*> &OUTCharactersSelectedForCombat)
{
	for (int32 i = 0; i < CharactersInCombat.Num(); i++)
	{
		if (!CharactersInCombat[i]) { continue; }
		if (CharactersInCombat[i]->IsSelectedForCombat) {
			OUTCharactersSelectedForCombat.Add(CharactersInCombat[i]);
		}
	}

	//return;
}

void ACombatManager::GetEncountersReadyForCombat(TArray<AEncounter*> &OUTEncountersReadyForCombat)
{
	OUTEncountersReadyForCombat = EncountersInCombat;
}

void ACombatManager::GetDefenders(TArray<AWTPlayerCharacter*> CharactersSelectedForCombat, TArray<AEncounter*> &OUTDefenders)
{
	for (int32 j = 0; j < CharactersSelectedForCombat.Num(); j++)
	{
		if (!CharactersSelectedForCombat[j]) { break; }

		if (!OUTDefenders.Last()) {
			OUTDefenders.Last() = CharactersSelectedForCombat[j]->CurrentEnemyToAttack;
		}
		else if (OUTDefenders.Last() == CharactersSelectedForCombat[j]->CurrentEnemyToAttack)
		{

		}
		else {
			OUTDefenders.Add(CharactersSelectedForCombat[j]->CurrentEnemyToAttack);
		}
	}
}

void ACombatManager::GetDefenders(TArray<AEncounter*> EncountersReadyForCombat, TArray<AWTPlayerCharacter*> &OUTDefenders)
{
	for (int32 j = 0; j < EncountersReadyForCombat.Num(); j++)
	{
		if (!EncountersReadyForCombat[j]) { break; }

		EncountersReadyForCombat[j]->CurrentEnemyToAttack = CharactersInCombat[FMath::RandRange(0, (CharactersInCombat.Num()-1))];

		if (!OUTDefenders.Last()) {
			OUTDefenders.Last() = EncountersReadyForCombat[j]->CurrentEnemyToAttack;
		}
		else if (OUTDefenders.Last() == EncountersReadyForCombat[j]->CurrentEnemyToAttack)
		{

		}
		else {
			OUTDefenders.Add(EncountersReadyForCombat[j]->CurrentEnemyToAttack);
		}
	}
}

AAttackDefenseActor* ACombatManager::SpawnAttackDefenseActor(
	AAttackDefenseActor* ActorToSpawn,
	AWTPlayerCharacter* CharacterAttacker,
	FVector LocationToSpawn,
	EAttackType AttackTypeToSpawn
)
{
	//FVector LocationToSpawn = (CharacterAttacker->GetActorLocation() + FVector(0.f, 0.f, 300.f) + FVector(0.f, 0.f, 140.f*IndexOfGeneratedAttackRow));
	ActorToSpawn = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));


	ActorToSpawn->StartLocation = CharacterAttacker->GetActorLocation();
	ActorToSpawn->EndLocation = CharacterAttacker->CurrentEnemyToAttack->GetActorLocation();

		ActorToSpawn->AttackState = AttackTypeToSpawn;
	
	ActorToSpawn->Initialize();

	return ActorToSpawn;
}

AAttackDefenseActor* ACombatManager::SpawnAttackDefenseActor(
	AAttackDefenseActor* ActorToSpawn,
	AEncounter* EncounterAttacker,
	FVector LocationToSpawn,
	EAttackType AttackTypeToSpawn
)
{
	ActorToSpawn = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));


	ActorToSpawn->StartLocation = EncounterAttacker->GetActorLocation();
	ActorToSpawn->EndLocation = EncounterAttacker->CurrentEnemyToAttack->GetActorLocation();

	ActorToSpawn->AttackState = AttackTypeToSpawn;

	ActorToSpawn->Initialize();

	return ActorToSpawn;
}

AAttackDefenseActor* ACombatManager::SpawnAttackDefenseActor(FVector LocationToSpawn, EDefenseType DefenseTypeToSpawn)
{
	auto SpawnedAttackDefenseActor = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));

	SpawnedAttackDefenseActor->DefenseState = EDefenseType::Absorb;
	SpawnedAttackDefenseActor->Initialize();

	return SpawnedAttackDefenseActor;
}

void ACombatManager::PlayerCharactersAttack()
{
	Refresh();

	TArray<AWTPlayerCharacter*> CharactersAttackers;
	GetCharactersSelectedForCombat(CharactersAttackers);

	TArray<AEncounter*> Defenders = { nullptr };

	GetDefenders(CharactersAttackers, Defenders);

	for (int32 j = 0; j < CharactersAttackers.Num(); j++)
	{
		TArray<EAttackType> AttackRowToGenerate = CharactersAttackers[j]->pAttackDefenseComponent->AttackPoolRow_1;

		if (CharactersAttackers[j]->ItemPickedForFight)
		{
			AttackRowToGenerate.Append(CharactersAttackers[j]->ItemPickedForFight->AttackPoolRow_1);
		}

		if (CharactersAttackers[j]->HiredCompanion)
		{
			AttackRowToGenerate.Append(Cast<AEncounter_Good>(CharactersAttackers[j]->HiredCompanion)->pAttackDefenseComponent->AttackPoolRow_2);
		}

		TArray<AAttackDefenseActor*> AttackRowActors;
		AttackRowActors.Init(nullptr, AttackRowToGenerate.Num());

		for (int32 i = 0; i < AttackRowToGenerate.Num(); i++)
		{
			int32 DelayBeforeSpawnNext = 0;
			FVector LocationToSpawn = (CharactersAttackers[j]->GetActorLocation() + FVector(0.f, 0.f, 300.f) + FVector(0.f, 0.f, 140.f*i));

			if (!ensure(AttackDefenceActorClass)) { UE_LOG(LogTemp, Warning, TEXT("AttackDefence actor class is not set in CombatManager")) return; }

			AttackRowActors[i] = SpawnAttackDefenseActor(AttackRowActors[i], CharactersAttackers[j], LocationToSpawn, AttackRowToGenerate[i]);
			if (!ensure(AttackRowActors[i])) { return; }

			DelayBeforeSpawnNext++;
			
			if (AttackRowActors[i]) {
				AttackRowActors[i]->HoldTime = j * 0.2f*DelayBeforeSpawnNext;
			}
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Attack created"))

		for (int32 j = 0; j < Defenders.Num(); j++) {
			if (!Defenders[j]) { continue; }
			TArray<EDefenseType> DefenseRowToGenerate = Defenders[j]->pAttackDefenseComponent->DefensePoolRow_1;

			TArray<AAttackDefenseActor*> DefenseRowActors;
			DefenseRowActors.Init(nullptr, DefenseRowToGenerate.Num());

			for (int32 i = 0; i < DefenseRowToGenerate.Num(); i++)
			{
					FVector LocationToSpawnDefense = (Defenders[j]->GetActorLocation() + FVector(0.f, 0.f, 300.f) + FVector(0.f, 0.f, 140.f*i));
					DefenseRowActors[i] = SpawnAttackDefenseActor(LocationToSpawnDefense, DefenseRowToGenerate[i]);
			}
		}
	auto Timer = GetWorld()->SpawnActor<ATimer>(FVector(), FRotator(0.0f, 180.0f, 0.0f));
	UE_LOG(LogTemp, Warning, TEXT("Timer is spawned"))
		Timer->bIsFightingBack = true;
	Timer->LifeTime += (CharactersAttackers.Num() - 1)*0.2f;
	//Timer->Enemy = Defenders[j];

	int32 Min = 0;


	AWTPlayerCharacter* CharacterUnderAttack = nullptr;
	while (CharacterUnderAttack == nullptr)
	{
		auto RandomlyPickedCharacter = FMath::RandRange(Min, (CharactersAttackers.Num() - 1));
		if (CharactersAttackers[RandomlyPickedCharacter])
		{
			CharacterUnderAttack = CharactersAttackers[RandomlyPickedCharacter];
		}
	}

	Timer->CombatManager = this;
	Timer->PlayerCharacter = CharacterUnderAttack;
}

void ACombatManager::EncountersAttack()
{
	Refresh();
	UE_LOG(LogTemp, Error, TEXT("Encounters attack"))

	TArray<AEncounter*> EncounterAttackers;
	GetEncountersReadyForCombat(EncounterAttackers);

	TArray<AWTPlayerCharacter*> Defenders = { nullptr };

	GetDefenders(EncounterAttackers, Defenders);
	
	for (int32 j = 0; j < EncounterAttackers.Num(); j++)
	{
		TArray<EAttackType> AttackRowToGenerate = EncounterAttackers[j]->pAttackDefenseComponent->AttackPoolRow_1;

		TArray<AAttackDefenseActor*> AttackRowActors;
		AttackRowActors.Init(nullptr, AttackRowToGenerate.Num());

		for (int32 i = 0; i < AttackRowToGenerate.Num(); i++)
		{
			int32 DelayBeforeSpawnNext = 0;
			FVector LocationToSpawn = (EncounterAttackers[j]->GetActorLocation() + FVector(0.f, 0.f, 300.f) + FVector(0.f, 0.f, 140.f*i));

			if (!ensure(AttackDefenceActorClass)) { UE_LOG(LogTemp, Warning, TEXT("AttackDefence actor class is not set in CombatManager")) return; }

			AttackRowActors[i] = SpawnAttackDefenseActor(AttackRowActors[i], EncounterAttackers[j], LocationToSpawn, AttackRowToGenerate[i]);
			if (!ensure(AttackRowActors[i])) { return; }

			DelayBeforeSpawnNext++;

			if (AttackRowActors[i]) {
				AttackRowActors[i]->HoldTime = j * 0.2f*DelayBeforeSpawnNext;
			}
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Attack created"))

		for (int32 j = 0; j < Defenders.Num(); j++) {
			if (!Defenders[j]) { continue; }
			TArray<EDefenseType> DefenseRowToGenerate = Defenders[j]->pAttackDefenseComponent->DefensePoolRow_1;

			TArray<AAttackDefenseActor*> DefenseRowActors;
			DefenseRowActors.Init(nullptr, DefenseRowToGenerate.Num());

			for (int32 i = 0; i < DefenseRowToGenerate.Num(); i++)
			{
				FVector LocationToSpawnDefense = (Defenders[j]->GetActorLocation() + FVector(0.f, 0.f, 300.f) + FVector(0.f, 0.f, 140.f*i));
				DefenseRowActors[i] = SpawnAttackDefenseActor(LocationToSpawnDefense, DefenseRowToGenerate[i]);
			}
		}
	
}