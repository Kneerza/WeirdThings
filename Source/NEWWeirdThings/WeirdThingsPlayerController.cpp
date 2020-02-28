// Fill out your copyright notice in the Description page of Project Settings.

#define GETENUMSTRING(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetEnumName((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )

#include "WeirdThingsPlayerController.h"
#include "DeckManager.h"
#include "InteractiveLocationDecoration.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Timer.h"
#include "Encounter.h"
#include "ArrowTemplate.h"
#include "LocationTemplate.h"
#include "ItemTemplate.h"
#include "Encounter_Good.h"
#include "Encounter_Bad.h"
#include "Encounter_Dead.h"
#include "Action.h"
#include "AttackDefenseComponent.h"
#include "AttackDefenseActor.h"
#include "Runtime/CoreUObject/Public/UObject/Class.h"
#include "WTPlayerCharacter.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Engine/DirectionalLight.h"
#include "Runtime/Engine/Classes/Components/DirectionalLightComponent.h"


AWeirdThingsPlayerController::AWeirdThingsPlayerController()
{
	DeckManager = CreateDefaultSubobject<UDeckManager>(TEXT("DeckManager"));
}

void AWeirdThingsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ...



	/*
	// Create SunLight
	if (!ensure(DirectionalLight_BP)) { return; }
	pDirectionalLight = GetWorld()->SpawnActor<ADirectionalLight>(DirectionalLight_BP);

	pDirectionalLight->SetActorLocation(FVector(0.f));

	pDirectionalLightComponent = pDirectionalLight->FindComponentByClass<UDirectionalLightComponent>();

	pDirectionalLightComponent->bUseTemperature = true;
	*/
}

void AWeirdThingsPlayerController::LeftClickEvents()
{

	if (AreClickEventsDisabled) { return; }
	//for (int32 i = 0; i < AllLocationsInPlay.Num() - 1; i++)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *AllLocationsInPlay[i]->GetName())
	//}
	GetComponentUnderCursor(pClickedActor, ClickedActorClassName);

	if (!pClickedActor) { return; }
	if (bIsCombatOn) {

		if (ClickedActorClassName == "WTPlayerCharacter") {
			
			if (!(PlayersChosenToFight.Contains(pClickedActor))) {
			auto ReadyForCombatCharacter = Cast<AWTPlayerCharacter>(pClickedActor);
				PlayersChosenToFight.Add(ReadyForCombatCharacter);
				ReadyForCombatCharacter->SetSelectedForCombat(true);
			}
			for (int32 i = 0; i < PlayersChosenToFight.Num(); i++)
			{
				if (PlayersChosenToFight[i]) {
					UE_LOG(LogTemp, Warning, TEXT("%s is ready to fight"), *PlayersChosenToFight[i]->GetName())
				}
			}
		}

	}else if (ClickedActorClassName == "WTPlayerCharacter") {

		SelectCharacter(pClickedActor);
	}
	
}

void AWeirdThingsPlayerController::RightClickEvents()
{

	if (AreClickEventsDisabled) { return; }
	GetComponentUnderCursor(pClickedActor, ClickedActorClassName);
	if (bIsCombatOn) {

		if (ClickedActorClassName == "WTPlayerCharacter") {

			if (PlayersChosenToFight.Contains(pClickedActor)) {
				auto RemovedFromCombatCharacter = Cast<AWTPlayerCharacter>(pClickedActor);
				PlayersChosenToFight.Remove(RemovedFromCombatCharacter);
				RemovedFromCombatCharacter->SetSelectedForCombat(false);
			}
			UE_LOG(LogTemp, Warning, TEXT("--------------------------"))
			for (int32 i = 0; i < PlayersChosenToFight.Num(); i++)
			{
				if (PlayersChosenToFight[i]) {
					UE_LOG(LogTemp, Warning, TEXT("%s is ready to fight"), *PlayersChosenToFight[i]->GetName())
				}
			}
		}
		else if (ClickedActorClassName == "Encounter_Bad")
		{
			Combat(pSelectedCharacter, Cast<AEncounter>(pClickedActor));
		}
		else if (ClickedActorClassName == "Encounter_Good")
		{
			Combat(pSelectedCharacter, Cast<AEncounter>(pClickedActor));
		}
		else if (ClickedActorClassName == "Encounter_Dead")
		{
			if (Cast<AEncounter_Dead>(pClickedActor)->IsAwake) {
				Combat(pSelectedCharacter, Cast<AEncounter>(pClickedActor));
			}
		}
	}else if (CharacterIsSelected) {
		GetComponentUnderCursor(pClickedActor, ClickedActorClassName);
		if (!pClickedActor) { return; }
		if (ClickedActorClassName == "ArrowTemplate")
		{
			pArrow = Cast<AArrowTemplate>(pClickedActor);


			ClickedArrowTemplateHandle(pArrow);

		}
		else if (ClickedActorClassName == "LocationTemplate")
		{
			// TODO collapse all ifs to function
			auto ClickedLocation = Cast<ALocationTemplate>(pClickedActor);
			if (pSelectedCharacter->CurrentLocation == ClickedLocation) { return; }
			if (!(pSelectedCharacter->CurrentLocation->IsRestricting && ((ClickedLocation->VerticalIndex - pSelectedCharacter->CurrentLocation->VerticalIndex) == 1)))
			{
				MoveCharacter(pSelectedCharacter, ClickedLocation);
			}
		}
		else if (ClickedActorClassName == "Action")
		{
			ClickedActionHandle(Cast <AAction>(pClickedActor));
		}
		else if (ClickedActorClassName == "Encounter_Bad")
		{
			Combat(pSelectedCharacter, Cast<AEncounter>(pClickedActor));
		}
		else if (ClickedActorClassName == "Encounter_Good")
		{
			Combat(pSelectedCharacter, Cast<AEncounter>(pClickedActor));
		}
		else if (ClickedActorClassName == "Encounter_Dead")
		{
			if (Cast<AEncounter_Dead>(pClickedActor)->IsAwake) {
				Combat(pSelectedCharacter, Cast<AEncounter>(pClickedActor));
			}
		}
		else if (ClickedActorClassName == "InteractiveLocationDecoration")
		{
			UE_LOG(LogTemp, Warning, TEXT("ILD is clicked"))
			ClickedActionHandle(Cast<AInteractiveLocationDecoration>(pClickedActor)->EntangledAction);
		}
	}

}

void AWeirdThingsPlayerController::ClickedArrowTemplateHandle(AArrowTemplate* ClickedArrow)
{
	if (!ensure(ClickedArrow->GetParentActor() == pSelectedCharacter->CurrentLocation)) { return; }


	if (DeckManager->AvailableLocationsClasses.Num() != 0) {
		TSubclassOf<ALocationTemplate> LocationClass = nullptr;
		if (ClickedArrow->ArrowType == EArrowType::Plot)
		{
			LocationClass = DeckManager->DrawPlotLocationFromDeck();
		}
		else {
			LocationClass = DeckManager->DrawLocationFromDeck();
		}

		ClickedArrow->SpawnLocation(ClickedArrow->GetActorRotation().Roll >= 0, LocationClass);

		ClickedArrow->DeactivateArrow();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No Available Locations"))
	}
}

void AWeirdThingsPlayerController::ClickedActionHandle(AAction* CurrentAction)
{
	if (pSelectedCharacter->CurrentLocation != CurrentAction->GetParentActor()) { return; }

	auto ActionPointsRequired = CurrentAction->ActionPointsRequired;
	if (pSelectedCharacter->CurrentActionPoints < ActionPointsRequired)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentActionPoints: %i"),pSelectedCharacter->CurrentActionPoints)
		UE_LOG(LogTemp, Warning, TEXT("Not enough AP"))
			return;
	}

	CurrentAction->GetTypeOfLock();

	if ((CurrentAction->ActionLockType[CurrentAction->CurrentLockTypeIndex]) == EActionLockType::No_Need)
	{
		if (PerformAction(CurrentAction, CurrentAction->Modifier)) {
			pSelectedCharacter->CurrentActionPoints -= ActionPointsRequired;
			CurrentAction->IsWorkedOut = true;
			CurrentAction->Deactivate();
			if (!(CurrentAction->EntangledInteractiveLocationDecoration)) {
				UE_LOG(LogTemp, Error, TEXT("No EntangledILP for this action"))
				return; 
			}
			
			CurrentAction->EntangledInteractiveLocationDecoration->ChangeState_InteractiveLocationDecoration();
				
		}
	}
	else {
		CurrentAction->GetTypeOfLock();
		TryToUnlock(CurrentAction);
	}
}

void AWeirdThingsPlayerController::SelectCharacter(AActor* CharacterToSelect)
{

	if (!ensure(CharacterToSelect)) { return; }
	if (pSelectedCharacter) { pSelectedCharacter->SetSelected(false); } // Make sure previous SelectedCharacter is unselected
	pSelectedCharacter = Cast <AWTPlayerCharacter>(CharacterToSelect);
	if (pSelectedCharacter)
	{
		pSelectedCharacter->SetSelected(true);
		CharacterIsSelected = true;
		//Possess(pSelectedCharacter);
	}

}

void AWeirdThingsPlayerController::GetComponentUnderCursor(AActor* &ClickedActor, FString &ClickedActorClassName)
{

	FHitResult HitResult;
	if (this->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult))
	{
		ClickedActor = HitResult.GetActor();

		ClickedActorClassName = ClickedActor->GetClass()->GetSuperClass()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ClickedActorClassName)
			UE_LOG(LogTemp, Warning, TEXT("Hit component: %s"), *HitResult.GetComponent()->GetName())
	}
	else { ClickedActor = nullptr; }

}

void AWeirdThingsPlayerController::GetCurrentLocationOfActor(AActor* Actor, ALocationTemplate* &CurrentLocation) // TODO replace 
{

	TArray<AActor*> OverlappedLocation;
	if (!Actor) { return; }
	Actor->GetOverlappingActors(OverlappedLocation, TSubclassOf<ALocationTemplate>());

	if (!OverlappedLocation[0]) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Here"))
		CurrentLocation = Cast<ALocationTemplate>(OverlappedLocation[0]);

}

bool AWeirdThingsPlayerController::AreOnSameLocation(AActor* Actor1, AActor* Actor2)
{

	/*
		TArray<AActor*> OverlappedLocation;
		if (!Actor1) { return false; }
		if (!Actor2) { return false; }
		Actor1->GetOverlappingActors(OverlappedLocation);
		if (!OverlappedLocation[0]) { return false; }
		return (Actor2->IsOverlappingActor(OverlappedLocation[0]));
		*/
	UE_LOG(LogTemp, Warning, TEXT("SHOULD NOT BE HERE! DELETE THIS FUNCTION!!!!!!"))
		return false;
}

void AWeirdThingsPlayerController::MoveCharacter(AWTPlayerCharacter* CharacterToMove, ALocationTemplate* LocationToMoveTo)
{
	if (CharacterToMove->MovementPoints < 1) { return; }

	//auto LocationOfLocationTemplate = LocationToMoveTo->GetActorLocation();
	auto LocationOfCharacter = CharacterToMove->CurrentLocation;

	// Check if another location is too far 
	if (abs(LocationToMoveTo->HorizontalIndex - LocationOfCharacter->HorizontalIndex) > 1) { return; }
	if ((abs(LocationToMoveTo->VerticalIndex - LocationOfCharacter->VerticalIndex) == 1) && (LocationToMoveTo->HorizontalIndex != 0)) { return; }
	if (abs(LocationToMoveTo->VerticalIndex - LocationOfCharacter->VerticalIndex) > 1) { return; }
	if ((LocationToMoveTo->HorizontalIndex != LocationOfCharacter->HorizontalIndex) && (LocationToMoveTo->VerticalIndex != LocationOfCharacter->VerticalIndex)) { return; }

	CharacterToMove->SetActorLocation(LocationToMoveTo->AvailableSocketPlayer[0]->GetComponentLocation());

	CharacterToMove->CurrentLocation = LocationToMoveTo;
	if (auto ForcedAction = LocationToMoveTo->ForcedAction)
	{
		PerformAction(ForcedAction, ForcedAction->Modifier);
		//ForcedAction->Deactivate();
		if (!(ForcedAction->EntangledInteractiveLocationDecoration)) {
			UE_LOG(LogTemp, Error, TEXT("No EntangledILP for this action"))
				return;
		}
		ForcedAction->EntangledInteractiveLocationDecoration->ChangeState_InteractiveLocationDecoration();
	}
	CharacterToMove->MovementPoints--;
	CharacterToMove->UpdateAvatar();
}

void AWeirdThingsPlayerController::TeleportCharacter(AWTPlayerCharacter* CharacterToMove, ALocationTemplate* LocationToMoveTo)
{
	CharacterToMove->SetActorLocation(LocationToMoveTo->AvailableSocketPlayer[0]->GetComponentLocation());

	CharacterToMove->CurrentLocation = LocationToMoveTo;
	if (auto ForcedAction = LocationToMoveTo->ForcedAction)
	{
		PerformAction(ForcedAction, ForcedAction->Modifier);
		//ForcedAction->Deactivate();
		if (!(ForcedAction->EntangledInteractiveLocationDecoration)) {
			UE_LOG(LogTemp, Error, TEXT("No EntangledILP for this action"))
				return;
		}
		ForcedAction->EntangledInteractiveLocationDecoration->ChangeState_InteractiveLocationDecoration();
	}
	CharacterToMove->MovementPoints--;
	CharacterToMove->UpdateAvatar();
}

void AWeirdThingsPlayerController::FightBack(AEncounter* Enemy, AWTPlayerCharacter* PlayerCharacter)
{
	if (Enemy->HealthPoints <= 0) { return; }
	AttackDefenseEvent(Enemy, PlayerCharacter);
}

void AWeirdThingsPlayerController::Combat(AWTPlayerCharacter* PlayerCharacter, AEncounter* Enemy)
{

	if (!PlayersChosenToFight.IsValidIndex(0)) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Players are ready to fight"))
		PlayerCharactersAttack(PlayersChosenToFight, Enemy);
	
}

void AWeirdThingsPlayerController::PlayerCharactersAttack(TArray<AWTPlayerCharacter*> CharactersAttackers, AEncounter* Defender)
{
	for (int32 j = 0; j < CharactersAttackers.Num(); j++)
	{
		TArray<EAttackType> AttackRowToGenerate = CharactersAttackers[j]->pAttackDefenseComponent->AttackPoolRow_1;
		TArray<AAttackDefenseActor*> AttackRowActors;
		AttackRowActors.Init(nullptr, AttackRowToGenerate.Num());



		for (int32 i = 0; i < AttackRowToGenerate.Num(); i++)
		{
			FVector LocationToSpawn = (CharactersAttackers[j]->GetActorLocation() + FVector(0.f, 0.f, 300.f) + FVector(0.f, 0.f, 140.f*i));

			// TODO move Rows generation to AAttackDefenseActor
			if (!ensure(AttackDefenceActorClass)) { UE_LOG(LogTemp, Warning, TEXT("AttackDefence actor class is not set in PlayerController")) }
			switch (AttackRowToGenerate[i])
			{
			case EAttackType::No_Attack:

				break;

			case EAttackType::Sharp:
				AttackRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));
				if (!ensure(AttackRowActors[i])) { return; }

				AttackRowActors[i]->StartLocation = CharactersAttackers[j]->GetActorLocation();
				AttackRowActors[i]->EndLocation = Defender->GetActorLocation();

				AttackRowActors[i]->AttackState = EAttackType::Sharp;
				AttackRowActors[i]->Initialize();
				break;

			case EAttackType::Blunt:
				AttackRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));
				if (!ensure(AttackRowActors[i])) { return; }

				AttackRowActors[i]->StartLocation = CharactersAttackers[j]->GetActorLocation();
				AttackRowActors[i]->EndLocation = Defender->GetActorLocation();

				AttackRowActors[i]->AttackState = EAttackType::Blunt;
				AttackRowActors[i]->Initialize();
				break;

			case EAttackType::Unavoidable:
				AttackRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));
				if (!ensure(AttackRowActors[i])) { return; }

				AttackRowActors[i]->StartLocation = CharactersAttackers[j]->GetActorLocation();
				AttackRowActors[i]->EndLocation = Defender->GetActorLocation();

				AttackRowActors[i]->AttackState = EAttackType::Unavoidable;
				AttackRowActors[i]->Initialize();
				break;

			case EAttackType::Tricky:
				AttackRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));
				if (!ensure(AttackRowActors[i])) { return; }

				AttackRowActors[i]->StartLocation = CharactersAttackers[j]->GetActorLocation();
				AttackRowActors[i]->EndLocation = Defender->GetActorLocation();

				AttackRowActors[i]->AttackState = EAttackType::Tricky;
				AttackRowActors[i]->Initialize();
				break;

			default:
				break;
			}
			AttackRowActors[i]->HoldTime = j*0.2f;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Attack created"))

		TArray<EDefenseType> DefenseRowToGenerate = Defender->pAttackDefenseComponent->DefensePoolRow_1;
	TArray<AAttackDefenseActor*> DefenseRowActors;
	DefenseRowActors.Init(nullptr, DefenseRowToGenerate.Num());

	for (int32 i = 0; i < DefenseRowToGenerate.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *GETENUMSTRING("EUsesEnum", DefenseRowToGenerate[i]))

			FVector LocationToSpawnDefense = (Defender->GetActorLocation() + FVector(0.f, 0.f, 300.f) + FVector(0.f, 0.f, 140.f*i));

		switch (DefenseRowToGenerate[i])
		{
		case EDefenseType::No_Defense:

			break;

		case EDefenseType::Absorb:
			DefenseRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawnDefense, FRotator(0.0f, 180.0f, 0.0f));

			DefenseRowActors[i]->DefenseState = EDefenseType::Absorb;
			DefenseRowActors[i]->Initialize();

			if (DefenseRowToGenerate[i] != EDefenseType::No_Defense) {
				DefenseRowActors[i]->IsAttacking = false;
				DefenseRowActors[i]->StartLocation = Defender->GetActorLocation();
			}
			break;

		case EDefenseType::Evade:
			DefenseRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawnDefense, FRotator(0.0f, 180.0f, 0.0f));

			DefenseRowActors[i]->DefenseState = EDefenseType::Evade;
			DefenseRowActors[i]->Initialize();

			if (DefenseRowToGenerate[i] != EDefenseType::No_Defense) {
				DefenseRowActors[i]->IsAttacking = false;
				DefenseRowActors[i]->StartLocation = Defender->GetActorLocation();
			}
			break;

		default:
			break;
		}
	}
	auto Timer = GetWorld()->SpawnActor<ATimer>(FVector(), FRotator(0.0f, 180.0f, 0.0f));
	UE_LOG(LogTemp, Warning, TEXT("Timer is spawned"))
		Timer->bIsFightingBack = true;
	Timer->LifeTime += (CharactersAttackers.Num()-1)*0.2f;
	Timer->Enemy = Defender;

	int32 Min = 0;
	Timer->PlayerCharacter = CharactersAttackers[FMath::RandRange(Min, (CharactersAttackers.Num()-1))];
}

void AWeirdThingsPlayerController::InitiateCombat()
{
	bIsCombatOn = true;
	if (pSelectedCharacter) { pSelectedCharacter->SetSelected(false); }
	UE_LOG(LogTemp, Error, TEXT("Combat initiated"))
}

void AWeirdThingsPlayerController::EndCombat()
{
	bIsCombatOn = false;
	if (!PlayersChosenToFight.IsValidIndex(0)) { return; }
	for (int32 i = 0; i < PlayersChosenToFight.Num(); i++)
	{
		PlayersChosenToFight[i]->SetSelectedForCombat(false);
	}
	PlayersChosenToFight.Empty();

}

void AWeirdThingsPlayerController::ItemDurabilityCheck(AWTPlayerCharacter* ItemOwner)
{
	auto ActiveItem = ItemOwner->ActiveItem;
	auto Backpack = ItemOwner->Backpack;
	if (!ActiveItem) { return; }

	auto Rand = FMath::RandRange(1, 6);

	for (int32 i = 0; i < ActiveItem->ItemType.Num(); i++)
	{
		if (ActiveItem->ItemType[i] == EItemType::Weapon)
		{
			if (ActiveItem->ItemDurabilityByType[i] < Rand) {//FMath::RandRange(1, 6)) {

				ActiveItem->Destroy();
				Backpack[7] = nullptr;  // [7] - currently active item
				ItemOwner->Backpack[7] = Backpack[7];  //// [7] - currently active item
			}
			UE_LOG(LogTemp, Error, TEXT("%i"), Rand)
		}

	}
}

void AWeirdThingsPlayerController::AttackDefenseEvent(AWTPlayerCharacter* Attacker, AEncounter* Defender, bool IsFightingBack)
{

	TArray<EAttackType> AttackRowToGenerate = Attacker->pAttackDefenseComponent->AttackPoolRow_1;
	TArray<AAttackDefenseActor*> AttackRowActors;
	AttackRowActors.Init(nullptr, AttackRowToGenerate.Num());

	TArray<EDefenseType> DefenseRowToGenerate = Defender->pAttackDefenseComponent->DefensePoolRow_1;
	TArray<AAttackDefenseActor*> DefenseRowActors;
	DefenseRowActors.Init(nullptr, DefenseRowToGenerate.Num());

	for (int32 i = 0; i < AttackRowToGenerate.Num(); i++)
	{
		FVector LocationToSpawn = (Attacker->GetActorLocation() + FVector(0.f, 0.f, 300.f) + FVector(0.f, 0.f, 140.f*i));

		// TODO move Rows generation to AAttackDefenseActor
		if (!ensure(AttackDefenceActorClass)){UE_LOG(LogTemp, Warning, TEXT("AttackDefence actor class is not set in PlayerController")) }
		switch (AttackRowToGenerate[i])
		{
		case EAttackType::No_Attack:

			break;

		case EAttackType::Sharp:
			AttackRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));
			if (!ensure(AttackRowActors[i])) { return; }

			AttackRowActors[i]->StartLocation = Attacker->GetActorLocation();
			AttackRowActors[i]->EndLocation = Defender->GetActorLocation();

			AttackRowActors[i]->AttackState = EAttackType::Sharp;
			AttackRowActors[i]->Initialize();
			break;

		case EAttackType::Blunt:
			AttackRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));
			if (!ensure(AttackRowActors[i])) { return; }

			AttackRowActors[i]->StartLocation = Attacker->GetActorLocation();
			AttackRowActors[i]->EndLocation = Defender->GetActorLocation();

			AttackRowActors[i]->AttackState = EAttackType::Blunt;
			AttackRowActors[i]->Initialize();
			break;

		case EAttackType::Unavoidable:
			AttackRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));
			if (!ensure(AttackRowActors[i])) { return; }

			AttackRowActors[i]->StartLocation = Attacker->GetActorLocation();
			AttackRowActors[i]->EndLocation = Defender->GetActorLocation();

			AttackRowActors[i]->AttackState = EAttackType::Unavoidable;
			AttackRowActors[i]->Initialize();
			break;

		case EAttackType::Tricky:
			AttackRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));
			if (!ensure(AttackRowActors[i])) { return; }

			AttackRowActors[i]->StartLocation = Attacker->GetActorLocation();
			AttackRowActors[i]->EndLocation = Defender->GetActorLocation();

			AttackRowActors[i]->AttackState = EAttackType::Tricky;
			AttackRowActors[i]->Initialize();
			break;

		default:
			break;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Attack created"))
	for (int32 i = 0; i < DefenseRowToGenerate.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *GETENUMSTRING("EUsesEnum", DefenseRowToGenerate[i]))

			FVector LocationToSpawnDefense = (Defender->GetActorLocation() + FVector(0.f, 0.f, 300.f) + FVector(0.f, 0.f, 140.f*i));

		switch (DefenseRowToGenerate[i])
		{
		case EDefenseType::No_Defense:

			break;

		case EDefenseType::Absorb:
			DefenseRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawnDefense, FRotator(0.0f, 180.0f, 0.0f));

			DefenseRowActors[i]->DefenseState = EDefenseType::Absorb;
			DefenseRowActors[i]->Initialize();

			if (DefenseRowToGenerate[i] != EDefenseType::No_Defense) {
				DefenseRowActors[i]->IsAttacking = false;
				DefenseRowActors[i]->StartLocation = Defender->GetActorLocation();
			}
			break;

		case EDefenseType::Evade:
			DefenseRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawnDefense, FRotator(0.0f, 180.0f, 0.0f));

			DefenseRowActors[i]->DefenseState = EDefenseType::Evade;
			DefenseRowActors[i]->Initialize();

			if (DefenseRowToGenerate[i] != EDefenseType::No_Defense) {
				DefenseRowActors[i]->IsAttacking = false;
				DefenseRowActors[i]->StartLocation = Defender->GetActorLocation();
			}
			break;

		default:
			break;
		}
	}
	auto Timer = GetWorld()->SpawnActor<ATimer>(FVector(), FRotator(0.0f, 180.0f, 0.0f));
	UE_LOG(LogTemp, Warning, TEXT("Timer is spawned"))
		Timer->bIsFightingBack = IsFightingBack;
		Timer->Enemy = Defender;
	Timer->PlayerCharacter = Attacker;
}

void AWeirdThingsPlayerController::AttackDefenseEvent(AEncounter* Attacker, AWTPlayerCharacter* Defender)
{
	InitiateCombat();
	TArray<EAttackType> AttackRowToGenerate = Attacker->pAttackDefenseComponent->AttackPoolRow_1;
	TArray<AAttackDefenseActor*> AttackRowActors;
	AttackRowActors.Init(nullptr, AttackRowToGenerate.Num());

	TArray<EDefenseType> DefenseRowToGenerate = Defender->pAttackDefenseComponent->DefensePoolRow_1;
	TArray<AAttackDefenseActor*> DefenseRowActors;
	DefenseRowActors.Init(nullptr, DefenseRowToGenerate.Num());

	for (int32 i = 0; i < AttackRowToGenerate.Num(); i++)
	{
		FVector LocationToSpawn = (Attacker->GetActorLocation() + FVector(0.f, 0.f, 300.f) + FVector(0.f, 0.f, 140.f*i));

		// TODO move Rows generation to AAttackDefenseActor

		switch (AttackRowToGenerate[i])
		{
		case EAttackType::No_Attack:

			break;

		case EAttackType::Sharp:
			AttackRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));
			UE_LOG(LogTemp, Warning, TEXT("Spawned"))
				if (!ensure(AttackRowActors[i])) { return; }

			AttackRowActors[i]->StartLocation = Attacker->GetActorLocation();
			AttackRowActors[i]->EndLocation = Defender->GetActorLocation();

			AttackRowActors[i]->AttackState = EAttackType::Sharp;
			AttackRowActors[i]->Initialize();
			break;

		case EAttackType::Blunt:
			AttackRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));
			if (!ensure(AttackRowActors[i])) { return; }

			AttackRowActors[i]->StartLocation = Attacker->GetActorLocation();
			AttackRowActors[i]->EndLocation = Defender->GetActorLocation();

			AttackRowActors[i]->AttackState = EAttackType::Blunt;
			AttackRowActors[i]->Initialize();
			break;

		case EAttackType::Unavoidable:
			AttackRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));
			if (!ensure(AttackRowActors[i])) { return; }

			AttackRowActors[i]->StartLocation = Attacker->GetActorLocation();
			AttackRowActors[i]->EndLocation = Defender->GetActorLocation();

			AttackRowActors[i]->AttackState = EAttackType::Unavoidable;
			AttackRowActors[i]->Initialize();
			break;

		case EAttackType::Tricky:
			AttackRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawn, FRotator(0.0f, 180.0f, 0.0f));
			if (!ensure(AttackRowActors[i])) { return; }

			AttackRowActors[i]->StartLocation = Attacker->GetActorLocation();
			AttackRowActors[i]->EndLocation = Defender->GetActorLocation();

			AttackRowActors[i]->AttackState = EAttackType::Tricky;
			AttackRowActors[i]->Initialize();
			break;

		default:
			break;
		}
	}

	for (int32 i = 0; i < DefenseRowToGenerate.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *GETENUMSTRING("EUsesEnum", DefenseRowToGenerate[i]))



			FVector LocationToSpawnDefense = (Defender->GetActorLocation() + FVector(0.f, 0.f, 300.f) + FVector(0.f, 0.f, 140.f*i));

		switch (DefenseRowToGenerate[i])
		{
		case EDefenseType::No_Defense:

			break;

		case EDefenseType::Absorb:
			DefenseRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawnDefense, FRotator(0.0f, 180.0f, 0.0f));

			DefenseRowActors[i]->DefenseState = EDefenseType::Absorb;
			DefenseRowActors[i]->Initialize();

			if (DefenseRowToGenerate[i] != EDefenseType::No_Defense) {
				DefenseRowActors[i]->IsAttacking = false;
				DefenseRowActors[i]->StartLocation = Defender->GetActorLocation();
			}
			break;

		case EDefenseType::Evade:
			DefenseRowActors[i] = GetWorld()->SpawnActor<AAttackDefenseActor>(AttackDefenceActorClass, LocationToSpawnDefense, FRotator(0.0f, 180.0f, 0.0f));

			DefenseRowActors[i]->DefenseState = EDefenseType::Evade;
			DefenseRowActors[i]->Initialize();

			if (DefenseRowToGenerate[i] != EDefenseType::No_Defense) {
				DefenseRowActors[i]->IsAttacking = false;
				DefenseRowActors[i]->StartLocation = Defender->GetActorLocation();
			}
			break;

		default:
			break;
		}
	}
}

void AWeirdThingsPlayerController::RefreshCharacterMP()
{
	/* To DELETE----------------------------------------
	for (int32 i = 0; i < pSelectedCharacter->Backpack.Num(); i++)
	{
		if (!ensure(pSelectedCharacter->Backpack[i])) { return; }
		UE_LOG(LogTemp, Warning, TEXT("%s") ,*pSelectedCharacter->Backpack[i]->GetName())
	}
	*/

	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		if (!PlayerCharacters[i]) { continue; }
			PlayerCharacters[i]->MovementPoints = 3;
	}
	switch (CurrentTimeOfDay)
	{
	case ETimeOfDay::Morning:
		SetTimeNoon();
		break;

	case ETimeOfDay::Noon:
		SetTimeEvening();
		break;

	case ETimeOfDay::Evening:
		SetTimeNight();
		break;

	case ETimeOfDay::Night:
		SetTimeMorning();
		break;

	default:

		break;
	}

	//}
}

void AWeirdThingsPlayerController::SpawnEnemy(AAction* ActionInstigator)
{
	auto ParentLocation = Cast<ALocationTemplate>(ActionInstigator->GetParentActor());
	auto SpawningSocketTransform = ParentLocation->AvailableSocketEncounter[0]->GetComponentTransform();

	SpawningSocketTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

	FActorSpawnParameters SpawnParameters;

	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	auto SpawningClass = DeckManager->DrawEnemyFromDeck();

	if (!ensure(SpawningClass)) {
		UE_LOG(LogTemp, Warning, TEXT("Enemy Not Spawned"))
			return;
	}
	auto SpawnedEnemy = GetWorld()->SpawnActor<AEncounter_Bad>(SpawningClass, SpawningSocketTransform, SpawnParameters);
	if (SpawnedEnemy) {
		SpawnedEnemy->CurrentLocation = ParentLocation;
	}
}

ALocationTemplate* AWeirdThingsPlayerController::SpawnLocation(AAction* Action, bool IsSpawningOnRight, bool IsPlotLocation)
{

	// TODO IsSpawningOnRight and IsPlotLocation can be collapsed in one variable

	UE_LOG(LogTemp, Warning, TEXT("Started Spawning Location"))
		FActorSpawnParameters SpawningLocationParameters;
	SpawningLocationParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	if (DeckManager->AvailableLocationsClasses.Num() != 0) {
		TSubclassOf<ALocationTemplate> LocationClass = nullptr;
		if (IsPlotLocation)
		{
			LocationClass = DeckManager->DrawPlotLocationFromDeck();
		}
		else {
			LocationClass = DeckManager->DrawLocationFromDeck();
		}
		//-------SpawnLocation---------
		if (Action->GetAttachParentActor()) {

			// Set Transform for spawned location
			FTransform ParentlLocationTransform = Action->GetAttachParentActor()->GetActorTransform();
			FTransform SpawnedLocationTransform;
			SpawnedLocationTransform.SetScale3D(FVector(1.f, 1.f, 1.f));
			if (IsSpawningOnRight) {
				SpawnedLocationTransform.SetLocation(ParentlLocationTransform.GetLocation() + SpawnedLocationOffsetY);
			}
			else {
				SpawnedLocationTransform.SetLocation(ParentlLocationTransform.GetLocation() + SpawnedLocationOffsetZ);
			}
			SpawnedLocationTransform.SetRotation(ParentlLocationTransform.GetRotation());

			if ((SpawnedLocationTransform.GetLocation().Y > LocationsRowLimit)) { return nullptr; }


			return GetWorld()->SpawnActor<ALocationTemplate>(LocationClass, SpawnedLocationTransform, SpawningLocationParameters);

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("No ParentActor"))
				return nullptr;
		}

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No Available Locations"))
			return nullptr;
	}

}

void AWeirdThingsPlayerController::SpawnGoodEnc(AAction* ActionInstigator)
{
	auto ParentLocation = Cast<ALocationTemplate>(ActionInstigator->GetParentActor());
	auto SpawningSocketTransform = ParentLocation->AvailableSocketEncounter[0]->GetComponentTransform();

	SpawningSocketTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

	FActorSpawnParameters SpawnParameters;

	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	auto SpawningClass = DeckManager->DrawGoodEncFromDeck();

	if (!ensure(SpawningClass)) { return; }
	auto SpawnedGoodEnc = GetWorld()->SpawnActor<AEncounter_Good>(SpawningClass, SpawningSocketTransform, SpawnParameters);
	if (SpawnedGoodEnc) {
		SpawnedGoodEnc->CurrentLocation = ParentLocation;
	}
}

void AWeirdThingsPlayerController::PassItemToPlayer(EItemValue ItemValue)
{
	auto ItemToPickClass = DeckManager->DrawItemFromDeck(ItemValue);

	if (!ItemToPickClass) { return; }

	AItemTemplate* ItemToPick = GetWorld()->SpawnActor<AItemTemplate>(ItemToPickClass);
	pSelectedCharacter->GetItem(ItemToPick);
}

bool AWeirdThingsPlayerController::PerformAction(AAction* Action, int32 Modifier)
{
	auto ActionType = Action->ActionType;

	switch (ActionType)
	{
	case EActionType::Get_Item_C:
		for (int32 i = 0; i < Modifier; i++) {
			PassItemToPlayer(EItemValue::C);
		}
		return true;
		break;

	case EActionType::Get_Item_S:

		PassItemToPlayer(EItemValue::S);
		return true;
		break;

	case EActionType::Get_Item_G:

		PassItemToPlayer(EItemValue::G);
		return true;
		break;

	case EActionType::Get_EncounterBad:

		SpawnEnemy(Action);
		return true;
		break;

	case EActionType::Get_EncounterGood:

		SpawnGoodEnc(Action);
		return true;
		break;

	case EActionType::Get_EncounterRandom:

		if (FMath::RandBool())
		{
			SpawnGoodEnc(Action);
		}
		else {
			SpawnEnemy(Action);
		}
		return true;
		break;

	case EActionType::Get_Food:

		GetFood(Modifier);

		return true;
		break;

	case EActionType::Get_Wood:

		GetWood(Modifier);
		return true;
		break;

	case EActionType::Relief:

		pSelectedCharacter->RemoveInsanity(Modifier);
		return true;
		break;

	case EActionType::Fishing:

		if (FindAndUseItemToUnlock(EItemType::Tool)) {
			GetFood(Modifier);
			return true;
		}
		UE_LOG(LogTemp, Warning, TEXT("No Tool for fishing"))
			return false;
		break;

	case EActionType::Burn:
		
		if (pSelectedCharacter->Wood > 0) {
			ConsumeWood(1, pSelectedCharacter);
			if (Action->EntangledDeadEncounter)
			{
				FTransform EffectTransform;
				EffectTransform.SetLocation(Action->EntangledDeadEncounter->GetActorLocation());
				Encounter_DeadsInPlay.RemoveSingle(Action->EntangledDeadEncounter);
				Action->EntangledDeadEncounter->Destroy();
				if (!(Action->EntangledDeadEncounter->IsOnPlot)) {
					Action->EntangledDeadEncounter->CreatedAction->UnregisterComponent();
				}
				if (BurningEffectClass) {

					GetWorld()->SpawnActor<AActor>(BurningEffectClass, EffectTransform);
				}
			}else //if (Action->GetParentActor()->GetClass()->GetSuperClass()->GetName() == "Encounter_Dead") {
			{
				FTransform EffectTransform;
				EffectTransform.SetLocation(Action->GetParentActor()->GetActorLocation());
				Action->GetParentActor()->Destroy();
				if (BurningEffectClass) {

					GetWorld()->SpawnActor<AActor>(BurningEffectClass, EffectTransform);
				}
			}
			return true;
		}
		UE_LOG(LogTemp, Warning, TEXT("Not enough wood"))
			return false;
		break;

	case EActionType::Get_Injury:

		pSelectedCharacter->GetInjury(Modifier);

		return true;
		break;


	case EActionType::ArrowRight_Good:

		MoveCharacter(pSelectedCharacter, SpawnLocation(Action, true, false));


		return true;
		break;

	case EActionType::ArrowRight_Bad:

		MoveCharacter(pSelectedCharacter, SpawnLocation(Action, true, false));

		return true;
		break;

	case EActionType::ArrowRight_Ugly:

		MoveCharacter(pSelectedCharacter, SpawnLocation(Action, true, false));

		return true;
		break;

	case EActionType::ArrowUp_Plot:

		MoveCharacter(pSelectedCharacter, SpawnLocation(Action, false, true));

		return true;
		break;

	case EActionType::Arrow_Move:

		//MoveCharacter(pSelectedCharacter, Action->LocationArrowPointsTo);
		TeleportCharacter(pSelectedCharacter, Action->LocationArrowPointsTo);

		return true;
		break;

	case EActionType::Teleport:

		Action->SetTeleport();
		TeleportCharacter(pSelectedCharacter, Action->LocationArrowPointsTo);


		return true;
		break;

	default:

		return false;
		break;
	}
}

void AWeirdThingsPlayerController::TryToUnlock(AAction* CurrentAction)
{
	switch (CurrentAction->ActionLockType[CurrentAction->CurrentLockIndex])
	{
	case EActionLockType::Need_Food:
		if (ConsumeFood(1, pSelectedCharacter)) {
			CurrentAction->Unlock();
		}
		break;

	case EActionLockType::Need_Wood:
		if (ConsumeWood(1, pSelectedCharacter)) {
			CurrentAction->Unlock();
		}
		break;

	case EActionLockType::Need_Tool:

		if (FindAndUseItemToUnlock(EItemType::Tool)) {
			CurrentAction->Unlock();
		}

		break;

	case EActionLockType::Need_Axe:

		if (FindAndUseItemToUnlock(EItemType::Axe)) {
			CurrentAction->Unlock();
		}

		break;

	case EActionLockType::Need_Exhaustion:

		pSelectedCharacter->GetExhaustion(1);
		CurrentAction->Unlock();

		break;

	case EActionLockType::Need_Insanity:

		pSelectedCharacter->GetInsanity(1);
		CurrentAction->Unlock();

		break;

	case EActionLockType::Need_Item_C:

		if (FindAndUseItemToUnlock(EItemValue::C)) {
			CurrentAction->Unlock();
		}

		break;

	case EActionLockType::Need_Item_S:

		if (FindAndUseItemToUnlock(EItemValue::S)) {
			CurrentAction->Unlock();
		}

		break;

	case EActionLockType::Need_Item_G:

		if (FindAndUseItemToUnlock(EItemValue::G)) {
			CurrentAction->Unlock();
		}

		break;

	default:

		break;
	}
}

bool AWeirdThingsPlayerController::FindAndUseItemToUnlock(EItemType BackpackItemType)
{
	if (pSelectedCharacter) {

		auto Backpack = pSelectedCharacter->Backpack;

		for (int32 i = 0; i < Backpack.Num(); i++)
		{

			if (!(Backpack[i])) { continue; }

			auto ItemType = Backpack[i]->ItemType;
			for (int32 j = 0; j < ItemType.Num(); j++)
			{
				if (ItemType[j] == BackpackItemType)
				{
					if (Backpack[i]->ItemDurabilityByType[j] < FMath::RandRange(1, 6)) {
						Backpack[i]->Destroy();
						Backpack[i] = nullptr;
						pSelectedCharacter->Backpack[i] = Backpack[i];
						//UE_LOG(LogTemp, Warning, TEXT("%s is destroyed"), *Backpack[i]->GetName())
						return true;
					}
					return true;
				}

			}
		}
		return false;
	}
	return false;
}

bool AWeirdThingsPlayerController::FindAndUseItemToUnlock(EItemValue BackpackItemValue)
{
	if (pSelectedCharacter) {

		auto Backpack = pSelectedCharacter->Backpack;

		for (int32 i = 0; i < Backpack.Num(); i++)
		{
			if (!(Backpack[i])) { continue; }

			if (Backpack[i]->ItemValue == BackpackItemValue) {
				Backpack[i]->Destroy();
				Backpack[i] = nullptr;
				pSelectedCharacter->Backpack[i] = Backpack[i];

				return true;
			}
		}
		return false;
	}
	return false;
}

void AWeirdThingsPlayerController::SetTimeMorning()
{
	//if (!ensure(pDirectionalLight)) { return; }
	//pDirectionalLight->SetActorRotation(FRotator(0.f, 0.f, 0.f));

	//if (!ensure(pDirectionalLightComponent)) { return; }

	//pDirectionalLightComponent->SetWorldRotation(FRotator(-8.f, -35.f, 6.f));

	//pDirectionalLightComponent->SetLightColor(FColor(255, 186, 182, 1));

	//pDirectionalLightComponent->SetTemperature(5000.f);

	//pDirectionalLightComponent->SetIntensity(1.f);

	if (!ensure(SunlightMorning)) { return; }
	if (!ensure(SunlightNoon)) { return; }
	if (!ensure(SunlightEvening)) { return; }
	if (!ensure(SunlightNight)) { return; }

	SunlightMorning->SetActorHiddenInGame(false);
	SunlightNoon->SetActorHiddenInGame(true);
	SunlightEvening->SetActorHiddenInGame(true);
	SunlightNight->SetActorHiddenInGame(true);

	AddActionPointToEveryCharacter();

	for (int32 i = 0; i < Encounter_DeadsInPlay.Num(); i++)
	{
		Encounter_DeadsInPlay[i]->SetAwakened(false);
	}


	CurrentTimeOfDay = ETimeOfDay::Morning;
}

void AWeirdThingsPlayerController::SetTimeNoon()
{
	//if (!ensure(pDirectionalLight)) { return; }
	//pDirectionalLight->SetActorRotation(FRotator(-30.f, 0.f, 0.f));

	//if(!ensure(pDirectionalLightComponent)) { return; }

	//pDirectionalLightComponent->SetLightColor(FColor(255, 244, 206, 1));

	//pDirectionalLightComponent->SetTemperature(6500.f);

	//pDirectionalLightComponent->SetIntensity(1.f);

	if (!ensure(SunlightMorning)) { return; }
	if (!ensure(SunlightNoon)) { return; }
	if (!ensure(SunlightEvening)) { return; }
	if (!ensure(SunlightNight)) { return; }

	SunlightMorning->SetActorHiddenInGame(true);
	SunlightNoon->SetActorHiddenInGame(false);
	SunlightEvening->SetActorHiddenInGame(true);
	SunlightNight->SetActorHiddenInGame(true);

	AddActionPointToEveryCharacter();

	CurrentTimeOfDay = ETimeOfDay::Noon;
}

void AWeirdThingsPlayerController::SetTimeEvening()
{
	//if (!ensure(pDirectionalLight)) { return; }
	//pDirectionalLight->SetActorRotation(FRotator(0.f, 65.f, 0.f));

	//if (!ensure(pDirectionalLightComponent)) { return; }

	//pDirectionalLightComponent->SetLightColor(FColor(174, 107, 65, 1));

	//pDirectionalLightComponent->SetTemperature(3500.f);

	//pDirectionalLightComponent->SetIntensity(10.f);

	if (!ensure(SunlightMorning)) { return; }
	if (!ensure(SunlightNoon)) { return; }
	if (!ensure(SunlightEvening)) { return; }
	if (!ensure(SunlightNight)) { return; }

	SunlightMorning->SetActorHiddenInGame(true);
	SunlightNoon->SetActorHiddenInGame(true);
	SunlightEvening->SetActorHiddenInGame(false);
	SunlightNight->SetActorHiddenInGame(true);

	AddActionPointToEveryCharacter();

	//TODO collapse to function
	for (int32 i = 0; i < Encounter_DeadsInPlay.Num(); i++)
	{
		Encounter_DeadsInPlay[i]->SetAwakened(true);
	}

	CurrentTimeOfDay = ETimeOfDay::Evening;
}

void AWeirdThingsPlayerController::SetTimeNight()
{
	//if (!ensure(pDirectionalLight)) { return; }
	//pDirectionalLight->SetActorRotation(FRotator(-30.f, 0.f, 0.f));

	//if (!ensure(pDirectionalLightComponent)) { return; }

	//pDirectionalLightComponent->bUseTemperature = false;



	//pDirectionalLightComponent->SetTemperature(4000.f);
	//pDirectionalLightComponent->SetIntensity(1.f);
	//pDirectionalLightComponent->SetLightColor(FColor(101.f, 102.f, 255.f, 1.f));

	if (!ensure(SunlightMorning)) { return; }
	if (!ensure(SunlightNoon)) { return; }
	if (!ensure(SunlightEvening)) { return; }
	if (!ensure(SunlightNight)) { return; }

	SunlightMorning->SetActorHiddenInGame(true);
	SunlightNoon->SetActorHiddenInGame(true);
	SunlightEvening->SetActorHiddenInGame(true);
	SunlightNight->SetActorHiddenInGame(false);

	AddActionPointToEveryCharacter();

	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		//if (PlayerCharacters[i]->Food <= 0) { PlayerCharacters[i]->GetHunger(1); }
		//else { ConsumeFood(1, PlayerCharacters[i]); }
		PlayerCharacters[i]->GetHunger(1);

		if (PlayerCharacters[i]->Wood <= 0) { PlayerCharacters[i]->GetInsanity(1); }
		//else { ConsumeWood(1, PlayerCharacters[i]); }
	}

	//TODO collapse to function
	for (int32 i = 0; i < Encounter_DeadsInPlay.Num(); i++)
	{
		Encounter_DeadsInPlay[i]->Move();
	}

	CurrentTimeOfDay = ETimeOfDay::Night;
}

bool AWeirdThingsPlayerController::ConsumeFood(int32 FoodAmountToConsume, AWTPlayerCharacter* AffectedCharacter) //TODO make applicable regardless selected character or not
{
	if (!AffectedCharacter) { return false; }

	if (AffectedCharacter->CurrentActionPoints < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough AP"))
			return false;
	}

	if ((AffectedCharacter->Food - FoodAmountToConsume) < 0)
	{
		// TODO "Not enough food"
		return false;
	}
	AffectedCharacter->Food -= FoodAmountToConsume;
	AffectedCharacter->RemoveHunger(1);

	AffectedCharacter->CurrentActionPoints--;

	return true;
}

bool AWeirdThingsPlayerController::ConsumeWood(int32 WoodAmountToConsume, AWTPlayerCharacter* AffectedCharacter) //TODO make applicable regardless selected character or not
{
	if (!AffectedCharacter) { return false; }
	if ((AffectedCharacter->Wood - WoodAmountToConsume) < 0)
	{
		// TODO "Not enough food"
		return false;
	}
	AffectedCharacter->Wood -= WoodAmountToConsume;
	return true;
}

bool AWeirdThingsPlayerController::GetFood(int32 FoodAmountToGet)
{
	if (!pSelectedCharacter) { return false; }
	/*if ((pSelectedCharacter->Food - FoodAmountToGet) < 0)
	{
		// TODO "Can't carry more"
		return false;
	}
	*/
	pSelectedCharacter->Food += FoodAmountToGet;
	return true;
}

bool AWeirdThingsPlayerController::GetWood(int32 WoodAmountToGet)
{
	if (!pSelectedCharacter) { return false; }
	/* if ((pSelectedCharacter->Wood - WoodAmountToGet) < 0)
	{
		// TODO "Can't carry more"
		return false;
	}
	*/
	pSelectedCharacter->Wood += WoodAmountToGet;
	return true;
}

 void AWeirdThingsPlayerController::MakeCampFire(AWTPlayerCharacter* PlayerCharacter)
{

	//GetWorld()->SpawnActor<AActor>(CampFireClassToSpawn, PlayerCharacter->CurrentLocation->SocketCampFire->GetComponentLocation());
	//PlayerCharacter->CurrentLocation->SocketCampFire;

	if (PlayerCharacter->CurrentActionPoints < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough AP"))
			return;
	}

	auto AvailableCampFireSocket = PlayerCharacter->CurrentLocation->AvailableSocketCampFire[0];
	if (!ensure(AvailableCampFireSocket)) { return; }

	auto SpawningSocketTransform = AvailableCampFireSocket->GetComponentTransform();

	SpawningSocketTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

	FActorSpawnParameters SpawnParameters;

	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	auto SpawningClass = CampFireClassToSpawn;

	if (!ensure(CampFireClassToSpawn)) {
		UE_LOG(LogTemp, Warning, TEXT("Campfire Not Spawned"))
			return;
	}
	if (ConsumeWood(1, PlayerCharacter))
	{
		GetWorld()->SpawnActor<AActor>(CampFireClassToSpawn, SpawningSocketTransform, SpawnParameters);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No wood available"))
	}

	PlayerCharacter->CurrentActionPoints--;
}

void AWeirdThingsPlayerController::Sleep(AWTPlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter->CurrentActionPoints < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough AP"))
			return;
	}

	PlayerCharacter->RemoveExhaustion(1);
	PlayerCharacter->CurrentActionPoints--;
}

void AWeirdThingsPlayerController::AddActionPointToEveryCharacter()
{
	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		PlayerCharacters[i]->CurrentActionPoints = PlayerCharacters[i]->ActionPoints;
	}
}