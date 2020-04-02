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

	UE_LOG(LogTemp, Error, TEXT("Refreshinggggg"))

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
					//if (Encounter_BadInPlay[i]->IsAggressive) {
						EncountersInCombat.Add(Encounter_BadInPlay[i]);
						Encounter_BadInPlay[i]->IsInCombat = true;
					//}
					//else {
					//	Encounter_BadInPlay[i]->Deactivate();
					//}
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
				if ((Encounter_DeadsInPlay[i]->CurrentLocation == CurrentLocation) && (Encounter_DeadsInPlay[i]->IsAwake))
				{
					//if (Encounter_DeadsInPlay[i]->IsAggressive) {
						EncountersInCombat.Add(Encounter_DeadsInPlay[i]);
						Encounter_DeadsInPlay[i]->IsInCombat = true;
					//}
					//else {
					//	Encounter_DeadsInPlay[i]->Deactivate();
					//}
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

					if (IsInitiatedByCharacter) {
						EncountersInCombat.Add(Encounter_GoodInPlay[i]);
						Encounter_GoodInPlay[i]->IsInCombat = true;
					}
					else {
						if (!Encounter_GoodInPlay[i]->IsHired) {
							Encounter_GoodInPlay[i]->Deactivate();
						}
					}
				}
			}
		}
	}

	if (!EncountersInCombat.IsValidIndex(0))
	{

		PlayerController->Message[0] = "No enemies to attack";

			PlayerController->CombatManagersInPlay.Remove(this);

		for (int32 i = 0; i < PlayerCharacters.Num(); i++)
		{
			if (PlayerCharacters[i])
			{
				if (PlayerCharacters[i]->CurrentLocation == CurrentLocation)
				{
					PlayerCharacters[i]->SetSelectedForCombat(false, nullptr, nullptr);
					PlayerCharacters[i]->CurrentCombatManager = nullptr;
					PlayerCharacters[i]->IsInCombat = false;
				}
			}
		}

		PlayerController->CombatManagersInPlay.Remove(this);
		IsCombatManagerDestroyed = true;
		Destroy();
		return;
	}

	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		if (PlayerCharacters[i])
		{
			if ((PlayerCharacters[i]->IsDied) || (PlayerCharacters[i]->IsSurvived)) { continue; }
			if (PlayerCharacters[i]->CurrentLocation == CurrentLocation)
			{
				CharactersInCombat.Add(PlayerCharacters[i]);
				if (!(EncountersInCombat.Contains(PlayerCharacters[i]->CurrentEnemyToAttack)))
				{
				PlayerCharacters[i]->SetSelectedForCombat(false, nullptr, nullptr);
				}

				PlayerCharacters[i]->CurrentCombatManager = this;
				PlayerCharacters[i]->IsInCombat = true;
				//PlayerController->DeselectCharacter(PlayerCharacters[i]);
			}
		}
	}

	if (CharactersInCombat.IsValidIndex(0)) {
		UE_LOG(LogTemp, Error, TEXT("Characters in combat: %i"), CharactersInCombat.Num())
	}
	if (!CharactersInCombat.IsValidIndex(0))
	{

		UE_LOG(LogTemp, Warning, TEXT("Characters are not on location"))
		
		PlayerController->CombatManagersInPlay.Remove(this);
		IsCombatManagerDestroyed = true;
		Destroy();
	}
}

void ACombatManager::ShowCharactersInCombat()
{
	/*
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
	*/
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
		else 
			if (OUTDefenders.Last() == CharactersSelectedForCombat[j]->CurrentEnemyToAttack)
		{

		}
		else {
			OUTDefenders.Add(CharactersSelectedForCombat[j]->CurrentEnemyToAttack);
		}
	}
}

void ACombatManager::GetDefenders(TArray<AEncounter*> EncountersReadyForCombat, TArray<AWTPlayerCharacter*> &OUTDefenders)
{
	if (!EncountersReadyForCombat.IsValidIndex(0)) { return; }
	if (!CharactersInCombat.IsValidIndex(0)) { return; }

	for (int32 i = 0; i < CharactersInCombat.Num(); i++)
	{
		if (CharactersInCombat[i]) {
			UE_LOG(LogTemp, Error, TEXT("Defenders: %s"), *CharactersInCombat[i]->GetName())
		}
	}

	for (int32 j = 0; j < EncountersReadyForCombat.Num(); j++)
	{
		if (!EncountersReadyForCombat.IsValidIndex(j)) { UE_LOG(LogTemp, Error, TEXT("No Encounters ready for combat")) return; }
		if (!EncountersReadyForCombat[j]) { return; }
		EncountersReadyForCombat[j]->CurrentEnemyToAttack = CharactersInCombat[FMath::RandRange(0, (CharactersInCombat.Num()-1))];

		if (!OUTDefenders.Last()) {
			OUTDefenders.Last() = EncountersReadyForCombat[j]->CurrentEnemyToAttack;
		}
		else 
			if (OUTDefenders.Last() == EncountersReadyForCombat[j]->CurrentEnemyToAttack)
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
	if (!ActorToSpawn) {
		UE_LOG(LogTemp, Warning, TEXT("AttackDefense not spawned in: %s"),*EncounterAttacker->GetName())
		return nullptr; }
	if (!EncounterAttacker) {

		UE_LOG(LogTemp, Warning, TEXT("No SpawnedActor"))
			return nullptr;

	}
	if (!EncounterAttacker) {

		UE_LOG(LogTemp, Warning, TEXT("No EncounterAttacker"))
			return nullptr;

	}
	ActorToSpawn->StartLocation = EncounterAttacker->GetActorLocation();

	if (!EncounterAttacker->CurrentEnemyToAttack) { UE_LOG(LogTemp, Error, TEXT("No Enemy to attack for encounter")) return nullptr; }
	ActorToSpawn->EndLocation = EncounterAttacker->CurrentEnemyToAttack->GetActorLocation();

	ActorToSpawn->AttackState = AttackTypeToSpawn;

	ActorToSpawn->Initialize();

	return ActorToSpawn;
}

AAttackDefenseActor* ACombatManager::SpawnAttackDefenseActor(FVector LocationToSpawn, EDefenseType DefenseTypeToSpawn)
{
	auto SpawnedAttackDefenseActor = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));

	SpawnedAttackDefenseActor->DefenseState = DefenseTypeToSpawn;
	SpawnedAttackDefenseActor->Initialize();

	return SpawnedAttackDefenseActor;
}

void ACombatManager::PlayerCharactersAttack()
{
	UE_LOG(LogTemp, Error, TEXT("Characters attacking"))

		if (IsFightInProgress) { PlayerController->Message[0] = "Fight in progress"; return; }
	Refresh();
	if (IsCombatManagerDestroyed) { return; }

	UE_LOG(LogTemp, Error, TEXT("Characters attacking after refreshing"))

	TArray<AWTPlayerCharacter*> CharactersAttackers;
	GetCharactersSelectedForCombat(CharactersAttackers);
	if (!CharactersAttackers.IsValidIndex(0)) { 
		PlayerController->Message[0] = "Pick characters for fight";
		return; }
	
	TArray<AEncounter*> Defenders = { nullptr };

	GetDefenders(CharactersAttackers, Defenders);
	if (!Defenders.IsValidIndex(0)) { 
		
		return; }
	
	int32 CountsOfCreatedActors = 1;
	//int32 DelayBeforeSpawnNext = 1;
	//float HoldTime = 0;
	for (int32 j = 0; j < CharactersAttackers.Num(); j++)
	{
		TArray<EAttackType> AttackRowToGenerate = CharactersAttackers[j]->pAttackDefenseComponent->AttackPoolRow_1;

		if (CharactersAttackers[j]->ItemPickedForFight)
		{
			AttackRowToGenerate = CharactersAttackers[j]->ItemPickedForFight->AttackPoolRow_1;
		}

		if (CharactersAttackers[j]->HiredCompanion)
		{
			AttackRowToGenerate.Append(Cast<AEncounter_Good>(CharactersAttackers[j]->HiredCompanion)->pAttackDefenseComponent->AttackPoolRow_2);
		}

		TArray<AAttackDefenseActor*> AttackRowActors;
		AttackRowActors.Init(nullptr, AttackRowToGenerate.Num());
		
		//float LocalTime = 0;
		for (int32 i = 0; i < AttackRowToGenerate.Num(); i++)
		{
			//int32 DelayBeforeSpawnNext = 0;
			FVector LocationToSpawn = (CharactersAttackers[j]->GetActorLocation() + FVector(0.f, 0.f, 300.f) + FVector(0.f, 0.f, 140.f*i));

			if (!ensure(AttackDefenceActorClass)) { UE_LOG(LogTemp, Warning, TEXT("AttackDefence actor class is not set in CombatManager")) return; }

			AttackRowActors[i] = SpawnAttackDefenseActor(AttackRowActors[i], CharactersAttackers[j], LocationToSpawn, AttackRowToGenerate[i]);
			if (!ensure(AttackRowActors[i])) { return; }

			//DelayBeforeSpawnNext++;
			
			if (AttackRowActors[i]) {
				AttackRowActors[i]->HoldTime = (j*0.15*AttackRowToGenerate.Num());// *GetWorld()->GetDeltaSeconds() * 30;// *(/*10*jHoldTime +*/ 0.2*AttackRowToGenerate.Num());// *GetWorld()->GetDeltaSeconds() * 2;//DelayBeforeSpawnNext;
				//LocalTime = AttackRowActors[i]->HoldTime;
				AttackRowActors[i]->NumberOfActorsInSameRow = AttackRowToGenerate.Num();
				CountsOfCreatedActors++;
			}
			//DelayBeforeSpawnNext++;
		}
		//HoldTime = HoldTime + LocalTime;
		
		if (CharactersAttackers[j]->ItemPickedForFight)
		{
			CharactersAttackers[j]->ItemPickedForFight = PlayerController->ItemDurabilityCheck(CharactersAttackers[j], CharactersAttackers[j]->ItemPickedForFight);
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
					FVector LocationToSpawnDefense = (Defenders[j]->GetActorLocation() + FVector(0.f, 0.f, 500.f) + FVector(0.f, 0.f, 140.f*i));
					DefenseRowActors[i] = SpawnAttackDefenseActor(LocationToSpawnDefense, DefenseRowToGenerate[i]);
			}
		}
	auto Timer = GetWorld()->SpawnActor<ATimer>(FVector(), FRotator(0.0f, 180.0f, 0.0f));
	//UE_LOG(LogTemp, Warning, TEXT("Timer is spawned"))
		Timer->SetCombatManager(this);
		Timer->bIsFightingBack = true;
		Timer->LifeTime = 1.5+(CountsOfCreatedActors*0.2);// (CharactersAttackers.Num())*0.001f;
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

	//Timer->CombatManager = this;
	Timer->PlayerCharacter = CharacterUnderAttack;

	//IsFightInProgress = true;
}

void ACombatManager::EncountersAttack()
{
	//if (IsFightInProgress) { return; }
	/*
	if (!IsInitiatedByCharacter) {
		auto PlayerCharacters = PlayerController->PlayerCharacters;
		for (int32 i = 0; i < PlayerCharacters.Num(); i++)
		{
			if (PlayerCharacters[i])
			{
				if ((PlayerCharacters[i]->IsDied) || (PlayerCharacters[i]->IsSurvived)) { continue; }
				if (PlayerCharacters[i]->CurrentLocation == CurrentLocation)
				{
					CharactersInCombat.Add(PlayerCharacters[i]);
					if (!(EncountersInCombat.Contains(PlayerCharacters[i]->CurrentEnemyToAttack)))
					{
						PlayerCharacters[i]->SetSelectedForCombat(false, nullptr, nullptr);
					}

					PlayerCharacters[i]->CurrentCombatManager = this;
					PlayerCharacters[i]->IsInCombat = true;
					//PlayerController->DeselectCharacter(PlayerCharacters[i]);
				}
			}
		}

		if (!CharactersInCombat.IsValidIndex(0))
		{

			UE_LOG(LogTemp, Warning, TEXT("Characters are not on location"))

				PlayerController->CombatManagersInPlay.Remove(this);
			Destroy();
		}
		IsInitiatedByCharacter = true;
	}
	*/
	Refresh();
	if (IsCombatManagerDestroyed) { return; }
	UE_LOG(LogTemp, Error, TEXT("Encounters attack"))

	TArray<AEncounter*> EncounterAttackers;
	GetEncountersReadyForCombat(EncounterAttackers);
	if (!EncounterAttackers.IsValidIndex(0)) { return; }

	TArray<AWTPlayerCharacter*> Defenders = { nullptr };
	//if (!Defenders.IsValidIndex(0)) { return; }

	GetDefenders(EncounterAttackers, Defenders);

	if (Defenders.IsValidIndex(0)) {
		UE_LOG(LogTemp, Error, TEXT("Yes Defenders"))
		for (int32 i = 0; i < Defenders.Num(); i++)
		{
			if (Defenders[i]) {
				UE_LOG(LogTemp, Error, TEXT("Defenders: %s"), *Defenders[i]->GetName())
			}
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No defenders"))
	}


	int32 CountsOfCreatedActors = 1;

	if (!EncounterAttackers.IsValidIndex(0)) { return; }
	for (int32 j = 0; j < EncounterAttackers.Num(); j++)
	{
		TArray<EAttackType> AttackRowToGenerate = EncounterAttackers[j]->pAttackDefenseComponent->AttackPoolRow_1;

		TArray<AAttackDefenseActor*> AttackRowActors;
		AttackRowActors.Init(nullptr, AttackRowToGenerate.Num());

		for (int32 i = 0; i < AttackRowToGenerate.Num(); i++)
		{
			//int32 DelayBeforeSpawnNext = 0;
			FVector LocationToSpawn = (EncounterAttackers[j]->GetActorLocation() + FVector(0.f, 0.f, 600.f) + FVector(0.f, 0.f, 140.f*i));

			if (!ensure(AttackDefenceActorClass)) { UE_LOG(LogTemp, Warning, TEXT("AttackDefence actor class is not set in CombatManager")) return; }

			AttackRowActors[i] = SpawnAttackDefenseActor(AttackRowActors[i], EncounterAttackers[j], LocationToSpawn, AttackRowToGenerate[i]);
			if (!ensure(AttackRowActors[i])) { return; }
			AttackRowActors[i]->StartLocationOffset = 400;
			IsFightInProgress = true;

			//DelayBeforeSpawnNext++;

			if (AttackRowActors[i]) {
				AttackRowActors[i]->HoldTime = (j*0.15*AttackRowToGenerate.Num());
				AttackRowActors[i]->NumberOfActorsInSameRow = AttackRowToGenerate.Num();
				CountsOfCreatedActors++;
				//AttackRowActors[i]->HoldTime = j * 0.3f*AttackRowToGenerate.Num();
			}
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Attack created"))

		for (int32 j = 0; j < Defenders.Num(); j++) {
			if (!Defenders[j]) { continue; }
			TArray<EDefenseType> DefenseRowToGenerate;

			int32 BackpackArmorIndex = 3;
			TArray<AItemTemplate*> ArmorPiecesOnCharacter;
			for (int32 i = 0; i < BackpackArmorIndex; i++) {
				auto Armor = Defenders[j]->Backpack[i];
				if (Armor) {
					DefenseRowToGenerate.Append(Armor->DefensePoolRow_1);
					ArmorPiecesOnCharacter.Add(Armor);
				}
			}
			if (ArmorPiecesOnCharacter.Num() <= 0) { DefenseRowToGenerate = { EDefenseType::No_Defense }; }
			// = Defenders[j]->pAttackDefenseComponent->DefensePoolRow_1;

			TArray<AAttackDefenseActor*> DefenseRowActors;
			DefenseRowActors.Init(nullptr, DefenseRowToGenerate.Num());

			for (int32 i = 0; i < DefenseRowToGenerate.Num(); i++)
			{
				FVector LocationToSpawnDefense = (Defenders[j]->GetActorLocation() + FVector(0.f, 0.f, 350.f) + FVector(0.f, 0.f, 140.f*i));
				DefenseRowActors[i] = SpawnAttackDefenseActor(LocationToSpawnDefense, DefenseRowToGenerate[i]);
			}

			// Checking Durability for random Armor piece in combat
			UE_LOG(LogTemp, Error, TEXT("Checking Armor to break"))
			if (ArmorPiecesOnCharacter.IsValidIndex(0)) {
				auto Range = FMath::RandRange(0, (ArmorPiecesOnCharacter.Num() - 1));
				PlayerController->ItemDurabilityCheck(Defenders[j], ArmorPiecesOnCharacter[Range]);
				UE_LOG(LogTemp, Error, TEXT("Armor is broken"))
			}

		}
	auto Timer = GetWorld()->SpawnActor<ATimer>(FVector(), FRotator(0.0f, 180.0f, 0.0f));
	Timer->SetCombatManager(this);
	//UE_LOG(LogTemp, Warning, TEXT("Timer is spawned"))
		Timer->bIsFightingBack = false;
	Timer->LifeTime = 1 + (CountsOfCreatedActors*0.2);



	//Timer->Enemy = Defenders[j];

	//int32 Min = 0;

	/*
	AWTPlayerCharacter* CharacterUnderAttack = nullptr;
	while (CharacterUnderAttack == nullptr)
	{
		auto RandomlyPickedCharacter = FMath::RandRange(Min, (CharactersAttackers.Num() - 1));
		if (CharactersAttackers[RandomlyPickedCharacter])
		{
			CharacterUnderAttack = CharactersAttackers[RandomlyPickedCharacter];
		}
	}
	*/

	//Timer->CombatManager = this;
	//Timer->PlayerCharacter = CharacterUnderAttack;

	//IsFightInProgress = true;
}