// Fill out your copyright notice in the Description page of Project Settings.

#define GETENUMSTRING(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetEnumName((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )

#include "WeirdThingsPlayerController.h"
#include "DeckManager.h"
#include "CombatManager.h"
#include "InteractiveLocationDecoration.h"
#include "QuitManagement.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Encounter.h"
#include "LocationTemplate.h"
#include "ItemTemplate.h"
#include "Encounter_Good.h"
#include "Encounter_Bad.h"
#include "Encounter_Dead.h"
#include "Action.h"
#include "Runtime/CoreUObject/Public/UObject/Class.h"
#include "WTPlayerCharacter.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Engine/DirectionalLight.h"
#include "Runtime/Engine/Classes/Components/DirectionalLightComponent.h"


AWeirdThingsPlayerController::AWeirdThingsPlayerController()
{
	DeckManager = CreateDefaultSubobject<UDeckManager>(TEXT("DeckManager"));

	Message.Init("", 3);
}

void AWeirdThingsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ...

	UpdateGameGoals();

}

//------------------- Click events responses --------------------------------------

void AWeirdThingsPlayerController::Encounter_BadRightClickResponse(AEncounter_Bad* ClickedEncounter_Bad)
{
	if (AreClickEventsDisabled) { return; }

	// Player interaction with Bad Encounter
	if (pSelectedCharacter)
	{
		if (ClickedEncounter_Bad->CurrentLocation != pSelectedCharacter->CurrentLocation) { Message[0] = "Not on the same location"; return; }

		if (pSelectedCharacter->IsPickingEnemyToFight)
		{
			pSelectedCharacter->SetSelectedForCombat(true, pSelectedCharacter->RightActiveItem, ClickedEncounter_Bad);
			
			pSelectedCharacter = nullptr;
		}
		else if (!pSelectedCharacter->IsInCombat)
		{
			InitiateCombat(pSelectedCharacter);
		}
	}
}

void AWeirdThingsPlayerController::Encounter_BadLeftClickResponse(AEncounter_Bad* ClickedEncounter_Bad)
{
	if (AreClickEventsDisabled) { return; }

	// Player interaction with Bad Encounter
	if (pSelectedCharacter)
	{
		if (ClickedEncounter_Bad->CurrentLocation != pSelectedCharacter->CurrentLocation) { Message[0] = "Not on the same location"; return; }

		if (pSelectedCharacter->IsPickingEnemyToFight)
		{
			pSelectedCharacter->SetSelectedForCombat(true, pSelectedCharacter->LeftActiveItem, (ClickedEncounter_Bad));

			pSelectedCharacter = nullptr;
		}
	}
}

void AWeirdThingsPlayerController::Encounter_GoodRightClickResponse(AEncounter_Good* ClickedEncounter_Good)
{
	if (AreClickEventsDisabled) { return; }

		// Player interaction with Good Encounter
		if (pSelectedCharacter) {
			if (ClickedEncounter_Good->CurrentLocation != pSelectedCharacter->CurrentLocation) { Message[0] = "Not on the same location"; return; }

			if (CurrentlyHoveredByMouseEncounter_Good)
			{
				if (pSelectedCharacter->IsPickingEnemyToFight)
				{
					pSelectedCharacter->SetSelectedForCombat(true, pSelectedCharacter->RightActiveItem, ClickedEncounter_Good);

					pSelectedCharacter = nullptr;
				}
				else if (pSelectedCharacter->IsInCombat) {

				}
				else if (Trade(CurrentlyHoveredByMouseEncounter_Good->RBTradingAction, CurrentlyHoveredByMouseEncounter_Good->RBTradingRequiredItem, true))
				{
					CurrentlyHoveredByMouseEncounter_Good->RBTradingAction = EActionType::No_Action;
					CurrentlyHoveredByMouseEncounter_Good->RBTradingRequiredItem = EActionLockType::No_Need;
				}
			}
		}

}

void AWeirdThingsPlayerController::Encounter_GoodLeftClickResponse(AEncounter_Good* ClickedEncounter_Good)
{
	if (AreClickEventsDisabled) { return; }
	/*
	if (!pSelectedCharacter) { return; }
	if (Cast<AEncounter>(pClickedActor)->CurrentLocation != pSelectedCharacter->CurrentLocation) { return; }
	if (bIsCharacterPickingToFight)
	{
		if (!CharacterPickingToFight) { return; }

		//AddToPlayersChoosenForFight(CharacterPickingToFight, Cast<AEncounter>(pClickedActor), CharacterPickingToFight->LeftActiveItem);
		bIsCharacterPickingToFight = false;
	}
	else if (bIsCombatOn) {

	}
	else {
		if (CurrentlyHoveredByMouseEncounter_Good)
		{
			if (Trade(CurrentlyHoveredByMouseEncounter_Good->LBTradingAction, CurrentlyHoveredByMouseEncounter_Good->LBTradingRequiredItem, false))
			{
				CurrentlyHoveredByMouseEncounter_Good->LBTradingAction = EActionType::No_Action;
				CurrentlyHoveredByMouseEncounter_Good->LBTradingRequiredItem = EActionLockType::No_Need;
			}
		}
	}
	*/
	if (pSelectedCharacter) {
		if (ClickedEncounter_Good->CurrentLocation != pSelectedCharacter->CurrentLocation) { return; }
		if (CurrentlyHoveredByMouseEncounter_Good)
		{
			if (pSelectedCharacter->IsPickingEnemyToFight) {
				pSelectedCharacter->SetSelectedForCombat(true, pSelectedCharacter->LeftActiveItem, ClickedEncounter_Good);

				pSelectedCharacter = nullptr;
			}
			else if (pSelectedCharacter->IsInCombat) {

			}
			else if (Trade(CurrentlyHoveredByMouseEncounter_Good->LBTradingAction, CurrentlyHoveredByMouseEncounter_Good->LBTradingRequiredItem,false))
			{
				CurrentlyHoveredByMouseEncounter_Good->LBTradingAction = EActionType::No_Action;
				CurrentlyHoveredByMouseEncounter_Good->LBTradingRequiredItem = EActionLockType::No_Need;
			}
		}
	}
}

void AWeirdThingsPlayerController::Encounter_DeadRightClickResponse(AEncounter_Dead* ClickedEncounter_Dead)
{
	if (AreClickEventsDisabled) { return; }
	
	if (!pSelectedCharacter) { return; }
	if (ClickedEncounter_Dead->CurrentLocation != pSelectedCharacter->CurrentLocation) { return; }
	if (!ClickedEncounter_Dead->IsAwake)
	{
		
		if (pSelectedCharacter->IsInCombat) { return; }
		if (pSelectedCharacter->CurrentActionPoints < 1) { Message[0] = "No Action Points"; return; };
		auto DynamicAction = Cast<AAction>((ClickedEncounter_Dead->CreatedAction->GetChildActor()));
		
		PerformAction(DynamicAction, 1);
	}else
	if (pSelectedCharacter)
	{

		if (pSelectedCharacter->IsPickingEnemyToFight)
		{
			UE_LOG(LogTemp, Error, TEXT("Character is picking enemy"))
			pSelectedCharacter->SetSelectedForCombat(true, pSelectedCharacter->RightActiveItem, ClickedEncounter_Dead);
			//pSelectedCharacter->CurrentEnemyToAttack = Cast<AEncounter>(pClickedActor);
			pSelectedCharacter = nullptr;
			//AddToPlayersChoosenForFight(pSelectedCharacter, Cast<AEncounter>(pClickedActor), pSelectedCharacter->RightActiveItem);
		}
		else if (!(pSelectedCharacter->IsInCombat))
		{
			InitiateCombat(pSelectedCharacter);
		}
	}
}

void AWeirdThingsPlayerController::Encounter_DeadLeftClickResponse(AEncounter_Dead* ClickedEncounter_Dead)
{
	if (AreClickEventsDisabled) { return; }
	if (!pSelectedCharacter) { return; }
	if (ClickedEncounter_Dead->CurrentLocation != pSelectedCharacter->CurrentLocation) { return; }
	if (pSelectedCharacter && ClickedEncounter_Dead->IsAwake)
	{
		if (pSelectedCharacter->IsPickingEnemyToFight)
		{
			pSelectedCharacter->SetSelectedForCombat(true, pSelectedCharacter->LeftActiveItem, ClickedEncounter_Dead);
			//pSelectedCharacter->CurrentEnemyToAttack = Cast<AEncounter>(pClickedActor);
			pSelectedCharacter = nullptr;
			//AddToPlayersChoosenForFight(pSelectedCharacter, Cast<AEncounter>(pClickedActor), pSelectedCharacter->RightActiveItem);
		}
	}
	/*
	if (bIsCharacterPickingToFight)
	{
		if (!CharacterPickingToFight) { return; }

		//AddToPlayersChoosenForFight(CharacterPickingToFight, Cast<AEncounter>(pClickedActor), CharacterPickingToFight->LeftActiveItem);
		bIsCharacterPickingToFight = false;
	}
	else if (bIsCombatOn) {

	}
	else if (CharacterIsSelected) {

	}
	else {

	}
	*/
}

void AWeirdThingsPlayerController::ActionLeftClickResponse(AAction* ClickedAction)
{
	if (AreClickEventsDisabled) { return; }
	//if (bIsCharacterPickingToFight)
	//{

	//}
	//else if (bIsCombatOn) {

	//}
	//else {
	if (pSelectedCharacter) {
		if (pSelectedCharacter->IsInCombat) { return; }
		if (ClickedAction->IsDeactivated) { return; }
		//CurrentAction->GetTypeOfLock();
		if (ClickedAction->ActionLock[ClickedAction->CurrentLockIndex]) {
			if (pSelectedCharacter->LeftActiveItem)
			{
				ClickedAction->Unlock();
				ItemDurabilityCheck(pSelectedCharacter, pSelectedCharacter->LeftActiveItem);
				//pSelectedCharacter->LeftActiveItem->Destroy();
				//pSelectedCharacter->LeftActiveItem = nullptr;
				//pSelectedCharacter->Backpack[pSelectedCharacter->Backpack.Num() - 1] = nullptr;
				SetCurrentlyHoveredByMouseAction(true, ClickedAction);
			}
			//else {
			//	TryToUnlock(CurrentAction);
			//}
		}
	}
}

void AWeirdThingsPlayerController::ActionRightClickResponse(AAction* ClickedAction)
{

	//if (bIsCharacterPickingToFight)
	//{

	//}
	//else if (bIsCombatOn) {

	//}
	//else


	if (AreClickEventsDisabled) { return; }



	if (pSelectedCharacter) {
		if (pSelectedCharacter->IsInCombat) { Message[0] = "Can't while in combat"; return; }
		ClickedAction;
		if (ClickedAction->IsDeactivated) { return; }
		//CurrentAction->GetTypeOfLock();
		if (ClickedAction->ActionLock[ClickedAction->CurrentLockIndex]) {
			if (pSelectedCharacter->RightActiveItem)
			{
				ClickedAction->Unlock();
				ItemDurabilityCheck(pSelectedCharacter, pSelectedCharacter->RightActiveItem);
				//pSelectedCharacter->RightActiveItem->Destroy();
				//pSelectedCharacter->RightActiveItem = nullptr;
				//pSelectedCharacter->Backpack[pSelectedCharacter->Backpack.Num() - 2] = nullptr;
				SetCurrentlyHoveredByMouseAction(true, ClickedAction);
			}
			else {

				TryToUnlock(ClickedAction);
			}
		}
		else {
			ClickedActionHandle(ClickedAction);
		}
	}



	/*
	if (pSelectedCharacter) {
		if (pSelectedCharacter->IsInCombat) { Message[0] = "Can't while in combat"; return; }
		auto CurrentAction = Cast<AAction>(pClickedActor);
		if (CurrentAction->IsDeactivated) { return; }
		//CurrentAction->GetTypeOfLock();
		if (CurrentAction->ActionLock[CurrentAction->CurrentLockIndex]) {
			if (pSelectedCharacter->RightActiveItem)
			{				
				CurrentAction->Unlock();
				ItemDurabilityCheck(pSelectedCharacter, pSelectedCharacter->RightActiveItem);
				//pSelectedCharacter->RightActiveItem->Destroy();
				//pSelectedCharacter->RightActiveItem = nullptr;
				//pSelectedCharacter->Backpack[pSelectedCharacter->Backpack.Num() - 2] = nullptr;
				SetCurrentlyHoveredByMouseAction(true, CurrentAction);
			}
			else {
				
				TryToUnlock(CurrentAction);
			}
		}
		else {
			ClickedActionHandle(CurrentAction);
		}
	}
	*/
}

void AWeirdThingsPlayerController::LocationRightClickResponse(ALocationTemplate* ClickedLocation)
{
	if (AreClickEventsDisabled) { return; }
//	if (bIsCharacterPickingToFight)
//	{

//	}
	//else if (bIsCombatOn) {

//	}
	//else 
	if (pSelectedCharacter) {

		if (pSelectedCharacter->IsInCombat) { return; }

		if (pSelectedCharacter->CurrentLocation == ClickedLocation) { return; }

		auto CurrentLocation = Cast<ALocationTemplate>(pSelectedCharacter->CurrentLocation);

		if (!(CurrentLocation->IsRestricting && ((ClickedLocation->VerticalIndex - CurrentLocation->VerticalIndex) == 1)))
		{
			MoveCharacter(pSelectedCharacter, ClickedLocation);
		}
	}
	else {
			
	}
	
}

void AWeirdThingsPlayerController::InteractiveLocationDecorationRightClickResponse(AInteractiveLocationDecoration* ClickedILD)
{

	if (AreClickEventsDisabled) { return; }
	/*
	if (pSelectedCharacter) {
		if (!(pSelectedCharacter->IsInCombat)) {
			ClickedActionHandle(Cast<AInteractiveLocationDecoration>(pClickedActor)->EntangledAction);
		}
	}
	*/

	if (pSelectedCharacter) {
		if (pSelectedCharacter->IsInCombat) { return; }
		auto CurrentAction = ClickedILD->EntangledAction;
		if (!CurrentAction) { return; }
		//CurrentAction->GetTypeOfLock();
		if (CurrentAction->ActionLock[CurrentAction->CurrentLockIndex]) {
			if (pSelectedCharacter->RightActiveItem)
			{
				CurrentAction->Unlock();
				pSelectedCharacter->RightActiveItem->Destroy();
				pSelectedCharacter->RightActiveItem = nullptr;
				pSelectedCharacter->Backpack[pSelectedCharacter->Backpack.Num() - 2] = nullptr;
				SetCurrentlyHoveredByMouseAction(true, CurrentAction);
			}
			else {
				TryToUnlock(CurrentAction);
			}
		}
		else {
			ClickedActionHandle(CurrentAction);
		}
	}
}

void AWeirdThingsPlayerController::InteractiveLocationDecorationLeftClickResponse(AInteractiveLocationDecoration* ClickedILD)
{
	if (AreClickEventsDisabled) { return; }
	/*
	if (pSelectedCharacter) {
		if (!(pSelectedCharacter->IsInCombat)) {
			ClickedActionHandle(Cast<AInteractiveLocationDecoration>(pClickedActor)->EntangledAction);
		}
	}
	*/

	if (pSelectedCharacter) {
		if (pSelectedCharacter->IsInCombat) { return; }
		auto CurrentAction = ClickedILD->EntangledAction;
		if (!CurrentAction) { return; }
		//CurrentAction->GetTypeOfLock();
		if (CurrentAction->ActionLock[CurrentAction->CurrentLockIndex]) {
			if (pSelectedCharacter->LeftActiveItem)
			{
				CurrentAction->Unlock();
				pSelectedCharacter->LeftActiveItem->Destroy();
				pSelectedCharacter->LeftActiveItem = nullptr;
				pSelectedCharacter->Backpack[pSelectedCharacter->Backpack.Num() - 1] = nullptr;
				SetCurrentlyHoveredByMouseAction(true, CurrentAction);
			}
			else {
				TryToUnlock(CurrentAction);
			}
		}
	}
}

void AWeirdThingsPlayerController::ClickedActionHandle(AAction* CurrentAction)
{
	//if (!pSelectedCharacter) { return; }
	if (!CurrentAction) { return; }

	if (CurrentAction->IsLocked) {

		Message[0] = "Action is locked";
		//UE_LOG(LogTemp, Error, TEXT("Action is locked"))
	}

	if (!((pSelectedCharacter->CurrentLocation == CurrentAction->GetParentActor()) || (pSelectedCharacter->CurrentLocation == CurrentAction->GetAttachParentActor()))) {
		Message[0] = "Not on the same location";
		return; }

	auto ActionPointsRequired = CurrentAction->ActionPointsRequired;
	if (pSelectedCharacter->CurrentActionPoints < ActionPointsRequired)
	{
		Message[0] = "No Action Points";
			return;
	}

	CurrentAction->GetTypeOfLock();

	
	if (((CurrentAction->ActionLockType[CurrentAction->CurrentLockTypeIndex]) == EActionLockType::No_Need) || (!(CurrentAction->ActionLock[CurrentAction->CurrentLockIndex])))
	{
		auto CharacterPerformingAction = pSelectedCharacter;
		if (PerformAction(CurrentAction, CurrentAction->Modifier)) {
			if (CharacterPerformingAction) {
				RemoveActionPointsFromCharacter(CharacterPerformingAction, ActionPointsRequired);
				//CharacterPerformingAction->CurrentActionPoints -= ActionPointsRequired;
			}
			CurrentAction->IsWorkedOut = true;
			CurrentAction->Deactivate();
			if (!(CurrentAction->EntangledInteractiveLocationDecoration)) {
				UE_LOG(LogTemp, Error, TEXT("No EntangledILP for this action"))
				return; 
			}
			
			Cast<AInteractiveLocationDecoration>(CurrentAction->EntangledInteractiveLocationDecoration)->ChangeState();
				
		}
	}
	else {
		

		CurrentAction->GetTypeOfLock();
		TryToUnlock(CurrentAction);
		
			//CurrentAction->Unlock();
		
	}
}

void AWeirdThingsPlayerController::SelectCharacter(AActor* CharacterToSelectActor)
{
	if (AreClickEventsDisabled) { return; }

	if (!ensure(CharacterToSelectActor)) { return; }

	auto CharacterToSelect = Cast<AWTPlayerCharacter>(CharacterToSelectActor);
	if (CharacterToSelect->IsSleeping) { return; }
	if (CharacterToSelect->IsSelectedForCombat) { return; }

	if (pSelectedCharacter && (pSelectedCharacter!= CharacterToSelect)){
		DeselectCharacter(pSelectedCharacter);
	}
	
	pSelectedCharacter = CharacterToSelect;

	if (CharacterToSelect->IsInCombat) {
		pSelectedCharacter->SetSelectedForPickingEnemy(true);
	}
	else {
		pSelectedCharacter->SetSelected(true);
	}
}

void AWeirdThingsPlayerController::DeselectCharacter(AActor* CharacterToDeselectActor)
{
	if (AreClickEventsDisabled) { return; }

	if (!ensure(CharacterToDeselectActor)) { return; }
	auto CharacterToDeselect = Cast<AWTPlayerCharacter>(CharacterToDeselectActor);

	if (CharacterToDeselectActor != pSelectedCharacter) {
		
		if (pSelectedCharacter) {
			pSelectedCharacter->SetSelected(false);
			pSelectedCharacter->SetSelectedForCombat(false, nullptr, nullptr);
			pSelectedCharacter->SetSelectedForPickingEnemy(false);

			pSelectedCharacter = nullptr;
		}
		if (CharacterToDeselect->IsSelectedForCombat) {
			CharacterToDeselect->SetSelected(false);
			CharacterToDeselect->SetSelectedForCombat(false, nullptr, nullptr);
			CharacterToDeselect->SetSelectedForPickingEnemy(false);
		}
		//auto CharacterToDeselect = Cast<AWTPlayerCharacter>(CharacterToDeselectActor);
		//if (CharacterToDeselect)
		//{
		//	CharacterToDeselect->SetSelected(false);
		//	CharacterToDeselect->SetSelectedForCombat(false, nullptr, nullptr);
		//	CharacterToDeselect->SetSelectedForPickingEnemy(false);
		//}
		//return; }
		//CharacterToDeselect->SetSelectedForCombat(false, nullptr, nullptr);
		//if (PlayersChosenToFight.Contains(CharacterToDeselect))
		//{
		//	int32 i = 0;
		//	PlayersChosenToFight.Find(CharacterToDeselect, i);
		//	PlayersChosenToFight[i] = nullptr;
		//}
		//CharacterToDeselect->SetSelected(false);
		//pSelectedCharacter = nullptr;
		//if (CharacterToDeselect->IsSelectedForCombat){
			//CharacterToDeselect->SetSelected(false);
			//CharacterToDeselect->SetSelectedForCombat(false, nullptr, nullptr);
			//CharacterToDeselect->SetSelectedForPickingEnemy(false);
			//if (PlayersChosenToFight.Contains(CharacterToDeselect))
			//{
			//	int32 i = 0;
			//	PlayersChosenToFight.Find(CharacterToDeselect, i);
			//	PlayersChosenToFight[i] = nullptr;
			//}
		//}
		return;
	}
	
		pSelectedCharacter = nullptr;
	
	CharacterToDeselect->SetSelected(false);
	CharacterToDeselect->SetSelectedForCombat(false, nullptr, nullptr);
	CharacterToDeselect->SetSelectedForPickingEnemy(false);

}



//------------------------------- Movements -----------------------------------------------------------------------

void AWeirdThingsPlayerController::MoveCharacter(AWTPlayerCharacter* CharacterToMove, ALocationTemplate* LocationToMoveTo)
{
	//UE_LOG(LogTemp, Error, TEXT("LocationToMoveTo: %s"), *LocationToMoveTo->GetName())
	//	UE_LOG(LogTemp, Error, TEXT("LocationToMoveTo HorizontalIndex: %i"), LocationToMoveTo->HorizontalIndex)
		//UE_LOG(LogTemp, Error, TEXT("LocationToMoveTo: %s"), *LocationToMoveTo->GetName())
	if (!LocationToMoveTo) { return; }
	if (CharacterToMove->MovementPoints < 1) { Message[0] = "No Movement Points"; return; }

	if (!ensure(LocationToMoveTo)) { return; }
	//auto LocationOfLocationTemplate = LocationToMoveTo->GetActorLocation();
	auto LocationOfCharacter = Cast<ALocationTemplate>(CharacterToMove->CurrentLocation);

	// Check if another location is too far 
	if (abs(LocationToMoveTo->HorizontalIndex - LocationOfCharacter->HorizontalIndex) > 1) { Message[0] = "Too far"; return; }
	if ((abs(LocationToMoveTo->VerticalIndex - LocationOfCharacter->VerticalIndex) == 1) && (LocationToMoveTo->HorizontalIndex != 0)) { Message[0] = "Too far"; return; }
	if (abs(LocationToMoveTo->VerticalIndex - LocationOfCharacter->VerticalIndex) > 1) { Message[0] = "Too far"; return; }
	if ((LocationToMoveTo->HorizontalIndex != LocationOfCharacter->HorizontalIndex) && (LocationToMoveTo->VerticalIndex != LocationOfCharacter->VerticalIndex)) { Message[0] = "Too far"; return; }

	CharacterToMove->SetActorLocation(LocationToMoveTo->AvailableSocketPlayer[0]->GetComponentLocation());
	
	if (CharacterToMove->HiredCompanion) {
		CharacterToMove->HiredCompanion->SetActorLocation(CharacterToMove->GetActorLocation() + FVector(1.f, 30.f, 0.f));
	}
	
	CharacterToMove->CurrentLocation = LocationToMoveTo;
	auto ForcedAction = LocationToMoveTo->ForcedAction;
	if (ForcedAction && !(ForcedAction->IsWorkedOut))
	{
		if (PerformAction(ForcedAction, ForcedAction->Modifier))
		{
			if (!ForcedAction->IsInfinite)
			{
				ForcedAction->IsWorkedOut = true;
			}
			ForcedAction->Deactivate();
		}
		
		auto EntangledILD = Cast<AInteractiveLocationDecoration>(ForcedAction->EntangledInteractiveLocationDecoration);
		if (EntangledILD) {
			EntangledILD->ChangeState();
		}
		
	}

	if (CurrentTimeOfDay == ETimeOfDay::Evening)
	{
		if (Cast<ALocationTemplate>(CharacterToMove->CurrentLocation)->AvailableSocketCampFire.IsValidIndex(0))
		{
			CharacterToMove->DoesNeedFire = true;
		}
		else {
			CharacterToMove->DoesNeedFire = false;
		}
	}

	for (int32 i = 0; i < CombatManagersInPlay.Num(); i++)
	{
		if (CombatManagersInPlay[i])
		{
			if (CombatManagersInPlay[i]->CurrentLocation == LocationToMoveTo)
			{
				CombatManagersInPlay[i]->CharactersInCombat.Add(CharacterToMove);
				CombatManagersInPlay[i]->Refresh();

				//DeselectCharacter(CharacterToMove);
				//CharacterToMove->IsInCombat = true;
			}
		}
	}

	CharacterToMove->MovementPoints--;
	//CharacterToMove->UpdateAvatar();
	
	for (int32 i = 0; i < Encounter_DeadsInPlay.Num(); i++)
	{
		if (!Encounter_DeadsInPlay[i]) { continue; }
		if (!Encounter_DeadsInPlay[i]->IsAwake) { continue; }
		if (LocationToMoveTo == Encounter_DeadsInPlay[i]->CurrentLocation)
		{
			Encounter_DeadLookForPlayerToAttack(Encounter_DeadsInPlay[i]);
			return;
		}
	}

}

void AWeirdThingsPlayerController::TeleportCharacter(AWTPlayerCharacter* CharacterToMove, AActor* LocationToMoveToActor)
{

	auto LocationToMoveTo = Cast<ALocationTemplate>(LocationToMoveToActor);

	if (!LocationToMoveTo) { return; }
	if (CharacterToMove->MovementPoints < 1) { Message[0] = "No Movement Points"; return; }

	CharacterToMove->SetActorLocation(LocationToMoveTo->AvailableSocketPlayer[0]->GetComponentLocation());
	if (CharacterToMove->HiredCompanion) {
		CharacterToMove->HiredCompanion->SetActorLocation(CharacterToMove->GetActorLocation() + FVector(1.f, 30.f, 0.f));
	}

	CharacterToMove->CurrentLocation = LocationToMoveTo;
	/*
	if (auto ForcedAction = LocationToMoveTo->ForcedAction)
	{
		PerformAction(ForcedAction, ForcedAction->Modifier);
		//ForcedAction->Deactivate();
		if (!(ForcedAction->EntangledInteractiveLocationDecoration)) {
			UE_LOG(LogTemp, Error, TEXT("No EntangledILP for this action"))
				return;
		}
		Cast<AInteractiveLocationDecoration>(ForcedAction->EntangledInteractiveLocationDecoration)->ChangeState();
	}
	*/

	if (CurrentTimeOfDay == ETimeOfDay::Evening)
	{
		if (Cast<ALocationTemplate>(CharacterToMove->CurrentLocation)->AvailableSocketCampFire.IsValidIndex(0))
		{
			CharacterToMove->DoesNeedFire = true;
		}
		else {
			CharacterToMove->DoesNeedFire = false;
		}
	}

	for (int32 i = 0; i < CombatManagersInPlay.Num(); i++)
	{
		if (CombatManagersInPlay[i])
		{
			if (CombatManagersInPlay[i]->CurrentLocation == LocationToMoveTo)
			{
				CombatManagersInPlay[i]->CharactersInCombat.Add(CharacterToMove);
				CombatManagersInPlay[i]->Refresh();

				//DeselectCharacter(CharacterToMove);
				//CharacterToMove->IsInCombat = true;
			}
		}
	}



	CharacterToMove->MovementPoints--;
//	CharacterToMove->UpdateAvatar();

	for (int32 i = 0; i < Encounter_DeadsInPlay.Num(); i++)
	{
		if (!Encounter_DeadsInPlay[i]) { continue; }
		if (LocationToMoveTo == Encounter_DeadsInPlay[i]->CurrentLocation)
		{
			Encounter_DeadLookForPlayerToAttack(Encounter_DeadsInPlay[i]);
			return;
		}
	}
}

void AWeirdThingsPlayerController::Move_Encounter_Dead(AEncounter_Dead* Encounter_DeadToMove)
{
	auto CurrentLocation = Cast<ALocationTemplate>(Encounter_DeadToMove->CurrentLocation);
	auto CurrentLocationHorizontalIndex = CurrentLocation->HorizontalIndex;
	auto CurrentLocationVerticalIndex = CurrentLocation->VerticalIndex;


	if (!CurrentLocation) {
		UE_LOG(LogTemp, Warning, TEXT("CurrentLocation of Dead does not exist"))
			return;
	}
	if (CurrentLocationHorizontalIndex > 0)
	{
		for (int32 i = 0; i < AllLocationsInPlay.Num(); i++)
		{
			if (!AllLocationsInPlay[i]) {
				UE_LOG(LogTemp, Warning, TEXT("Can't access AllLocationsInPlay[i]"))
					continue;
			}
			if ((CurrentLocationVerticalIndex == Cast<ALocationTemplate>(AllLocationsInPlay[i])->VerticalIndex) && ((CurrentLocation->HorizontalIndex - Cast<ALocationTemplate>(AllLocationsInPlay[i])->HorizontalIndex) == 1))
			{
				Encounter_DeadToMove->CurrentLocation = AllLocationsInPlay[i];
				Encounter_DeadToMove->CreatedAction->UnregisterComponent();
				Encounter_DeadToMove->CreatedAction = nullptr;
				Encounter_DeadToMove->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				Encounter_DeadToMove->AttachToActor(Encounter_DeadToMove->CurrentLocation, FAttachmentTransformRules::KeepWorldTransform);
				Encounter_DeadToMove->SetActorLocation(Cast<ALocationTemplate>(Encounter_DeadToMove->CurrentLocation)->AvailableSocketEncounter[0]->GetComponentLocation());
				Encounter_DeadToMove->CreateDynamicAction();
				
				Encounter_DeadLookForPlayerToAttack(Encounter_DeadToMove);
				return;
			}
		}
	}
}




//------------------------------- Combat ---------------------------------------------------------------------------

void AWeirdThingsPlayerController::Combat(ACombatManager* CombatManager)
{
	if (CombatManager)
	{
		CombatManager->PlayerCharactersAttack();
	}
/*
	if(CombatManagersInPlay.IsValidIndex(0))
	{
		for (int32 i = 0; i < CombatManagersInPlay.Num(); i++)
		{
			if (CombatManagersInPlay[i])
			{
				CombatManagersInPlay[i]->PlayerCharactersAttack();
			}
		}
	}
	*/
}

void AWeirdThingsPlayerController::InitiateCombat(AWTPlayerCharacter* Initiator)
{
	//if (Initiator->IsSleeping) { return; }
	//if (Initiator->IsInCombat) { return; }
	//if (PlayerCharacter->IsPickingEnemyToFight) { return; }
	//if (PlayerCharacter->IsSelectedForCombat) { return; }
	if (Initiator) {
		auto SpawnedCombatManager = SpawnCombatManager(Cast<ALocationTemplate>(Initiator->CurrentLocation), false);
	}
	else {
		return;
	}

	//bIsCombatOn = true;
	if (pSelectedCharacter) { 
		pSelectedCharacter->SetSelected(false); 
		pSelectedCharacter = nullptr;
	}
}

void AWeirdThingsPlayerController::InitiateCombat(AEncounter* Initiator)
{
	//if (Initiator->IsSleeping) { return; }
	//if (Initiator->IsInCombat) { return; }
	//if (PlayerCharacter->IsPickingEnemyToFight) { return; }
	//if (PlayerCharacter->IsSelectedForCombat) { return; }
	if (Initiator) {	
		auto SpawnedCombatManager = SpawnCombatManager(Cast<ALocationTemplate>(Initiator->CurrentLocation), true);
	}
	else {
		return;
	}

	//bIsCombatOn = true;
	if (pSelectedCharacter) {
		pSelectedCharacter->SetSelected(false);
		pSelectedCharacter = nullptr;
	}
}

void AWeirdThingsPlayerController::EndCombat()
{
	
	bIsCombatOn = false;
	if (!PlayersChosenToFight.IsValidIndex(0)) { return; }
	UE_LOG(LogTemp, Warning, TEXT("PlayersChosenToFight is not empty"))
	
	for (int32 i = 0; i < PlayersChosenToFight.Num(); i++)
	{
		if (PlayersChosenToFight[i]) {
			PlayersChosenToFight[i]->SetSelectedForCombat(false, nullptr, nullptr);
			PlayersChosenToFight[i] = nullptr;
		}
	}

}

void AWeirdThingsPlayerController::Encounter_DeadLookForPlayerToAttack(AEncounter_Dead* Encounter_Dead)
{
	if (!PlayerCharacters.IsValidIndex(0)) { return; }
	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		if (!PlayerCharacters[i]) { return; }
		if (PlayerCharacters[i]->CurrentLocation == Encounter_Dead->CurrentLocation)
		{
			//AttackDefenseEvent(Encounter_Dead, PlayerCharacters[i]);
			InitiateCombat(Encounter_Dead);

			return;
		}

	}
}

void AWeirdThingsPlayerController::FleeFromCombat(AWTPlayerCharacter* FleeingCharacter)
{
	if (FleeingCharacter->IsSleeping) { return; }
	if (!FleeingCharacter->IsInCombat) { return; }

	if (FleeingCharacter->CurrentCombatManager->IsFightInProgress) { return; }

	if (FleeingCharacter->MovementPoints < 1)
	{ 
		FleeingCharacter->GetExhaustion(1);
		if (!FleeingCharacter) { return; }
		FleeingCharacter->MovementPoints++;
	}

	UE_LOG(LogTemp, Warning, TEXT("Fleeing from combat"))

	auto CurrenLocationHorizontalIndex = Cast<ALocationTemplate>(FleeingCharacter->CurrentLocation)->HorizontalIndex;
	auto CurrenLocationVerticalIndex = Cast<ALocationTemplate>(FleeingCharacter->CurrentLocation)->VerticalIndex;
	UE_LOG(LogTemp, Warning, TEXT("CurrentLocation Vertical Index: %i"), CurrenLocationVerticalIndex)
		UE_LOG(LogTemp, Warning, TEXT("CurrentLocation Horizontal Index: %i"), CurrenLocationHorizontalIndex)

	for (int32 i = 0; i < AllLocationsInPlay.Num(); i++)
	{
		auto PotentialLocationToMove = Cast<ALocationTemplate>(AllLocationsInPlay[i]);
		if (CurrenLocationVerticalIndex != PotentialLocationToMove->VerticalIndex) { continue;}
		

			UE_LOG(LogTemp, Warning, TEXT("Pre-Location vertical Index: %i"), PotentialLocationToMove->VerticalIndex)
				UE_LOG(LogTemp, Warning, TEXT("Pre-Location Horizontal Index: %i"), PotentialLocationToMove->HorizontalIndex)
				if ((CurrenLocationHorizontalIndex - PotentialLocationToMove->HorizontalIndex) == 1)
				{
					UE_LOG(LogTemp, Warning, TEXT("Pre-location to escape found"))
						MoveCharacter(FleeingCharacter, PotentialLocationToMove);
					DeselectCharacter(FleeingCharacter);
					FleeingCharacter->IsInCombat = false;
					if (!FleeingCharacter->CurrentCombatManager) { return; }
					FleeingCharacter->CurrentCombatManager->Refresh();
					return;
				}
		
	}
	


	for (int32 i = (AllLocationsInPlay.Num()-1); i >= 0; i--)
	{
		auto PotentialLocationToMove = Cast<ALocationTemplate>(AllLocationsInPlay[i]);
		if (CurrenLocationVerticalIndex != PotentialLocationToMove->VerticalIndex) { continue; }
		if ((PotentialLocationToMove->HorizontalIndex - CurrenLocationHorizontalIndex) == 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Post-location to escape found"))
			MoveCharacter(FleeingCharacter, PotentialLocationToMove);
			DeselectCharacter(FleeingCharacter);
			FleeingCharacter->IsInCombat = false;
			if (!FleeingCharacter->CurrentCombatManager) { return; }
			FleeingCharacter->CurrentCombatManager->Refresh();
			return;
		}
	}

	for (int32 i = 0; i < AllLocationsInPlay.Num(); i++)
	{
		auto PotentialLocationToMove = Cast<ALocationTemplate>(AllLocationsInPlay[i]);
		if (CurrenLocationHorizontalIndex != PotentialLocationToMove->HorizontalIndex) { continue; }


		UE_LOG(LogTemp, Warning, TEXT("Pre-Location Index: %i"), PotentialLocationToMove->HorizontalIndex)
			if ((CurrenLocationVerticalIndex - PotentialLocationToMove->VerticalIndex) == 1)
			{
				UE_LOG(LogTemp, Warning, TEXT("Pre-location to escape found"))
					MoveCharacter(FleeingCharacter, PotentialLocationToMove);
				DeselectCharacter(FleeingCharacter);
				FleeingCharacter->IsInCombat = false;
				if (!FleeingCharacter->CurrentCombatManager) { return; }
				FleeingCharacter->CurrentCombatManager->Refresh();
				return;
			}

	}

	for (int32 i = (AllLocationsInPlay.Num() - 1); i >= 0; i--)
	{
		auto PotentialLocationToMove = Cast<ALocationTemplate>(AllLocationsInPlay[i]);
		if (CurrenLocationHorizontalIndex != PotentialLocationToMove->HorizontalIndex) { continue; }

		if ((PotentialLocationToMove->VerticalIndex - CurrenLocationVerticalIndex) == 1)
		{
				MoveCharacter(FleeingCharacter, PotentialLocationToMove);
			DeselectCharacter(FleeingCharacter);
			FleeingCharacter->IsInCombat = false;
			if (!FleeingCharacter->CurrentCombatManager) { return; }
			FleeingCharacter->CurrentCombatManager->Refresh();
			return;
		}
	}

	Message[0] = "Nowhere to flee";

}

void AWeirdThingsPlayerController::RefreshCombatManager(ACombatManager* CombatManagerToRefresh)
{
	CombatManagerToRefresh->Refresh();
}




//------------------------------- Items -------------------------------------------

void AWeirdThingsPlayerController::ItemDurabilityCheck(AWTPlayerCharacter* ItemOwner, AItemTemplate* ItemToCheck, EItemType ItemTypeToCheck)
{
	//auto ActiveItem = ItemOwner->ActiveItem;
	//auto Backpack = ItemOwner->Backpack;
	//if (!ActiveItem) { return; }

	auto Rand = FMath::RandRange(1, 6);

	for (int32 i = 0; i < ItemToCheck->ItemType.Num(); i++)
	{
		if (ItemToCheck->ItemType[i] == ItemTypeToCheck)
		{
			if (ItemToCheck->ItemDurabilityByType[i] < Rand) {//FMath::RandRange(1, 6)) {

				ItemOwner->Backpack.Remove(ItemToCheck);
				ItemToCheck->Destroy();
				//Backpack[7] = nullptr;  // [7] - currently active item
				//ItemOwner->Backpack[7] = Backpack[7];  //// [7] - currently active item
			}
			UE_LOG(LogTemp, Error, TEXT("%i"), Rand)
		}

	}
}

AItemTemplate* AWeirdThingsPlayerController::ItemDurabilityCheck(AWTPlayerCharacter* ItemOwner, AItemTemplate* ItemToCheck)
{
	//auto ActiveItem = ItemOwner->ActiveItem;
	//auto Backpack = ItemOwner->Backpack;
	//if (!ActiveItem) { return; }
	if (!ItemToCheck) { return nullptr; }
	auto Rand = FMath::RandRange(1, 6);

	auto Durability = ItemToCheck->Durability;
	if (Rand > Durability)
	{
		auto IndexOfItem = ItemOwner->Backpack.Find(ItemToCheck);
		if (IndexOfItem == INDEX_NONE) {
			
			return nullptr; }
		if (ItemOwner->Backpack[IndexOfItem]) {
			ItemOwner->Backpack[IndexOfItem] = nullptr;
		}
		//ItemOwner->Backpack.Remove(ItemToCheck);
		Message[0] = "Item is broken";
		ItemToCheck->Destroy();
		ItemToCheck = nullptr;
	}

	RollVsDurabilityString = "Roll " + FString::FromInt(Rand) + " vs " + FString::FromInt(Durability) + " Durability";
	//Message[0] = "Roll " + FString::FromInt(Rand) + " vs " + FString::FromInt(Durability) + " Durability";
	//UE_LOG(LogTemp, Error, TEXT("%s"), *String)
	/*
	for (int32 i = 0; i < ItemToCheck->ItemType.Num(); i++)
	{
		if (ItemToCheck->ItemType[i] == ItemTypeToCheck)
		{
			if (ItemToCheck->ItemDurabilityByType[i] < Rand) {//FMath::RandRange(1, 6)) {

				ItemOwner->Backpack.Remove(ItemToCheck);
				ItemToCheck->Destroy();
				//Backpack[7] = nullptr;  // [7] - currently active item
				//ItemOwner->Backpack[7] = Backpack[7];  //// [7] - currently active item
			}
			UE_LOG(LogTemp, Error, TEXT("%i"), Rand)
		}

	}
	*/
	return ItemToCheck;
}

void AWeirdThingsPlayerController::UseItem(AItemTemplate* ItemToUse, AWTPlayerCharacter* ItemOwner)
{
	if (ItemOwner->CurrentActionPoints < ItemToUse->ActionPointsRequiredToUse)
	{
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Inside UseItem: checking if ItemToUse exists"))
	if (!ItemToUse) { return; }
	auto ItemToUseType = ItemToUse->ItemType;
	for (int32 i = 0; i < ItemToUseType.Num(); i++)
	{
		switch (ItemToUseType[i])

		{
		case EItemType::Medicine:

			
			if (ItemOwner->RemoveInjury(1))
			{
				ItemDurabilityCheck(ItemOwner, ItemToUse);
			}
			break;

		case EItemType::Tonic:

			ItemOwner->GetActionPoints(1);
			ItemDurabilityCheck(ItemOwner, ItemToUse);

			break;

		default:

			break;
			
		}
	}
	RemoveActionPointsFromCharacter(ItemOwner, ItemToUse->ActionPointsRequiredToUse);
	//ItemOwner->CurrentActionPoints -= ItemToUse->ActionPointsRequiredToUse;
}

bool AWeirdThingsPlayerController::PassItemToPlayer(TSubclassOf<AItemTemplate> ItemToPickClass)
{
	if (!ItemToPickClass) { return false; }

	auto RoomInBackpack = 5;
	auto Backpack = pSelectedCharacter->Backpack;
	for (int32 i = 3; i < Backpack.Num(); i++)
	{
		if (Backpack[i]) {
			RoomInBackpack--;
		}
	}

	if (RoomInBackpack < 1) { Message[0] = "Backpack is full"; return false; }

	AItemTemplate* ItemToPick = GetWorld()->SpawnActor<AItemTemplate>(ItemToPickClass);
	pSelectedCharacter->GetItem(ItemToPick);
	return true;
}

bool AWeirdThingsPlayerController::PassItemToPlayer(EItemValue ItemValue, int32 Modifier)
{
	

	auto RoomInBackpack = 5;
	auto Backpack = pSelectedCharacter->Backpack;
	for (int32 i = 3; i < Backpack.Num(); i++)
	{
		if (Backpack[i]) {
			RoomInBackpack--;
		}
	}

	if (RoomInBackpack < Modifier) { Message[0] = "Not enough room in backpack"; return false; }
	if (RoomInBackpack < 1) { Message[0] = "Backpack is full"; return false; }

	for (int32 i = 0; i < Modifier; i++)
	{
		auto ItemToPickClass = DeckManager->DrawItemFromDeck(ItemValue);

		if (!ItemToPickClass) { "No items in the world"; return false; }

		AItemTemplate* ItemToPick = GetWorld()->SpawnActor<AItemTemplate>(ItemToPickClass);
		pSelectedCharacter->GetItem(ItemToPick);
	}
	return true;
}

bool AWeirdThingsPlayerController::PassItemToPlayer(AItemTemplate* ItemsToPick)
{
	if (!ItemsToPick) { return false; }

	auto RoomInBackpack = 5;
	auto Backpack = pSelectedCharacter->Backpack;
	for (int32 i = 3; i < Backpack.Num(); i++)
	{
		if (Backpack[i]) {
			RoomInBackpack--;
		}
	}

	if (RoomInBackpack < 1) { Message[0] = "Backpack is full"; return false; }

	pSelectedCharacter->GetItem(ItemsToPick);
	return true;
}

bool AWeirdThingsPlayerController::DropItemOnLocation(AActor* LocationToDropItemOn, TSubclassOf<AAction> ActionItemToDropClass)
{
	if (!ActionItemToDropClass) { return false; }
	if (!LocationToDropItemOn) { return false; }
	UE_LOG(LogTemp, Warning, TEXT("Inside function of dropping items"))
	auto Location = Cast<ALocationTemplate>(LocationToDropItemOn);
	if (!Location->AvailableSocketDynamicPlayerAction[0]) { 
		UE_LOG(LogTemp, Warning, TEXT("No available sockets"))
		return false;
	}

	auto Transform = Location->AvailableSocketDynamicPlayerAction[0]->GetComponentTransform();
	Transform.SetScale3D(FVector(1.f, 1.f, 1.f));

	auto ItemToPick = GetWorld()->SpawnActor<AAction>(ActionItemToDropClass, Transform);
	if (!ItemToPick) { return false; }
	ItemToPick->IsOneTimeUse = true;
	ItemToPick->ActionPointsRequired = 0;
	ItemToPick->AttachToActor(LocationToDropItemOn, FAttachmentTransformRules::KeepWorldTransform);
	return true;
}




//------------------------------- Spawning -----------------------------------------

bool AWeirdThingsPlayerController::SpawnEnemy(AAction* ActionInstigator)
{
	auto ParentLocation = Cast<ALocationTemplate>(ActionInstigator->GetParentActor());
	if (ParentLocation->AvailableSocketEncounter.Num()==0) { return false; }
	auto SpawningSocketTransform = ParentLocation->AvailableSocketEncounter[0]->GetComponentTransform();

	SpawningSocketTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

	FActorSpawnParameters SpawnParameters;

	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	auto SpawningClass = DeckManager->DrawEnemyFromDeck();

	if (!ensure(SpawningClass)) {
		UE_LOG(LogTemp, Warning, TEXT("Enemy Not Spawned"))
			return false;
	}
	auto SpawnedEnemy = GetWorld()->SpawnActor<AEncounter_Bad>(SpawningClass, SpawningSocketTransform, SpawnParameters);
	if (SpawnedEnemy) {
		SpawnedEnemy->CurrentLocation = ParentLocation;
		InitiateCombat(SpawnedEnemy);
		return true;
	}
	return false;
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

			auto NewLocation = GetWorld()->SpawnActor<ALocationTemplate>(LocationClass, SpawnedLocationTransform, SpawningLocationParameters);
			Action->LocationArrowPointsTo = NewLocation;
			return NewLocation;

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

bool AWeirdThingsPlayerController::SpawnGoodEnc(AAction* ActionInstigator)
{
	auto ParentLocation = Cast<ALocationTemplate>(ActionInstigator->GetParentActor());
	if (ParentLocation->AvailableSocketEncounter_Good.Num()==0) { return false; }
	auto SpawningSocketTransform = ParentLocation->AvailableSocketEncounter_Good[0]->GetComponentTransform();

	SpawningSocketTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

	FActorSpawnParameters SpawnParameters;

	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	auto SpawningClass = DeckManager->DrawGoodEncFromDeck();

	if (!ensure(SpawningClass)) { return false; }
	auto SpawnedGoodEnc = GetWorld()->SpawnActor<AEncounter_Good>(SpawningClass, SpawningSocketTransform, SpawnParameters);
	if (SpawnedGoodEnc) {
		SpawnedGoodEnc->CurrentLocation = ParentLocation;
		SpawnedGoodEnc->pQuitManagementComponent->CheckQuitConditions();
		return true;
	}
	return false;
}

AItemTemplate* AWeirdThingsPlayerController::SpawnItem(TSubclassOf<AItemTemplate> ItemToSpawnClass)
{
	return GetWorld()->SpawnActor<AItemTemplate>(ItemToSpawnClass);
}

ACombatManager* AWeirdThingsPlayerController::SpawnCombatManager(ALocationTemplate* CurrentLocationOfCombat, bool IsInitiatedByEncounter)//, AActor* CombatInstigator)//, AActor* CombatInstigator)
{
	if (!CombatManagerClassToSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatManagerClassToSpawn wasn't set in Player Controller"))
			return nullptr;
	}

	for (int32 i = 0; i < CombatManagersInPlay.Num(); i++)
	{
		if (CombatManagersInPlay[i]->CurrentLocation == CurrentLocationOfCombat) { return nullptr; }
	}

	auto SpawnedCombatManager = GetWorld()->SpawnActor<ACombatManager>(CombatManagerClassToSpawn);
	if (SpawnedCombatManager) {
		SpawnedCombatManager->CurrentLocation = CurrentLocationOfCombat;
		//SpawnedCombatManager->CombatInstigator = CombatInstigator;
		 
			if (IsInitiatedByEncounter) {
				//SpawnedCombatManager->IsInitiatedByCharacter = false;
				SpawnedCombatManager->Refresh();
				if (SpawnedCombatManager) {
					SpawnedCombatManager->EncountersAttack();
				}
			}
			else {
				SpawnedCombatManager->IsInitiatedByCharacter = true;
				SpawnedCombatManager->Refresh();
			}
		
		return SpawnedCombatManager;
		
	}
	return nullptr;
	/*
	auto &CharactersInCombat = SpawnedCombatManager->CharactersInCombat;
	auto &EncountersInCombat = SpawnedCombatManager->EncountersInCombat;

	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		if (PlayerCharacters[i])
		{
			if (PlayerCharacters[i]->CurrentLocation == SpawnedCombatManager->CurrentLocation)
			{
				CharactersInCombat.Add(PlayerCharacters[i]);
				PlayerCharacters[i]->CurrentCombatManager = SpawnedCombatManager;
				PlayerCharacters[i]->IsInCombat = true;
			}
		}
	}

	if (Encounter_BadInPlay.IsValidIndex(0))
	{
		for (int32 i = 0; i < Encounter_BadInPlay.Num(); i++)
		{
			if (Encounter_BadInPlay[i])
			{
				if (Encounter_BadInPlay[i]->CurrentLocation == SpawnedCombatManager->CurrentLocation)
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
				if (Encounter_DeadsInPlay[i]->CurrentLocation == SpawnedCombatManager->CurrentLocation)
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
				if (Encounter_GoodInPlay[i]->CurrentLocation == SpawnedCombatManager->CurrentLocation)
				{
					EncountersInCombat.Add(Encounter_GoodInPlay[i]);
					Encounter_GoodInPlay[i]->IsInCombat = true;
				}
			}
		}
	}
	
	return SpawnedCombatManager;
	*/
}




// ------------------------------ Game goals --------------------------

void AWeirdThingsPlayerController::UpdateGameGoals()
{
	UpdateCharactersFoodInPlay();
	UpdateCharactersWoodInPlay();
}

void AWeirdThingsPlayerController::UpdateCharactersFoodInPlay()
{
	if (FoodGoalCompleted) { return; }
	if (FoodGoalFailed) { return; }
	CharactersFoodInPlay = 0;

	if (CurrentTimeOfDay == ETimeOfDay::Night)
	{
		Goals[0] = "(" + FString::FromInt(CharactersFoodInPlay) + "/" + FString::FromInt(FoodRequired) + ")" + " Find food before night" + "(Failed)";
		FoodGoalFailed = true;
		return;
	}

	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{

		if (PlayerCharacters[i])
		{
			CharactersFoodInPlay += PlayerCharacters[i]->Food;
		}
	}

	if (CharactersFoodInPlay >= FoodRequired)
	{
		Goals[0] = "(" + FString::FromInt(CharactersFoodInPlay) + "/" + FString::FromInt(FoodRequired) + ")" + " Find food before night" + "(Completed)";
		FoodGoalCompleted = true;
	}
	else {
		Goals[0] = "(" + FString::FromInt(CharactersFoodInPlay) + "/" + FString::FromInt(FoodRequired) + ")" + " Find food before night";
	}
}

void AWeirdThingsPlayerController::UpdateCharactersWoodInPlay()
{
	if (WoodGoalCompleted) { return; }
	if (WoodGoalFailed) { return; }

	if (CurrentTimeOfDay == ETimeOfDay::Night)
	{
		Goals[1] = "(" + FString::FromInt(CharactersWoodInPlay) + "/" + FString::FromInt(WoodRequired) + ")" + " Find wood before night" + "(Failed)";
		WoodGoalFailed = true;
		return;
	}

	CharactersWoodInPlay = 0;
	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{

		if (PlayerCharacters[i])
		{
			CharactersWoodInPlay += PlayerCharacters[i]->Wood;
		}
	}

	if (CharactersWoodInPlay >= WoodRequired)
	{
		Goals[1] = "(" + FString::FromInt(CharactersWoodInPlay) + "/" + FString::FromInt(WoodRequired) + ")" + " Find wood before night" + "(Completed)";
		WoodGoalCompleted = true;
	}
	else {
		Goals[1] = "(" + FString::FromInt(CharactersWoodInPlay) + "/" + FString::FromInt(WoodRequired) + ")" + " Find wood before night";
	}
}




//---------------------------------- Actions -------------------------

bool AWeirdThingsPlayerController::PerformAction(AAction* Action, int32 Modifier)
{

	if (!Action) { return false; }

	UE_LOG(LogTemp, Warning, TEXT("Performing action: %s"), *Action->GetName())
		auto ActionType = Action->ActionType;

	switch (ActionType)
	{
	case EActionType::Get_Item_C:
		
		if (PassItemToPlayer(EItemValue::C, Modifier)) {
			return true;
		}

		return false;
		break;

	case EActionType::Get_Item_S:

	
		if (PassItemToPlayer(EItemValue::S, Modifier)) {
			return true;
			}
	
		return false;
		break;

	case EActionType::Get_Item_G:

	
		if (PassItemToPlayer(EItemValue::G, Modifier)) {
			return true;
		}

		return false;
		break;

	case EActionType::Get_EncounterBad:

		if (SpawnEnemy(Action))
		{
			return true;
		}
		else
		{
			return false;
		}
		break;

	case EActionType::Get_EncounterGood:

		if (SpawnGoodEnc(Action))
		{
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Encounter_Good cannot be spawned"))
				return false;
		}
		break;

	case EActionType::Get_EncounterRandom:

		if (FMath::RandBool())
		{
			if (SpawnGoodEnc(Action))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else {
			if (SpawnEnemy(Action))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
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

	case EActionType::Heal:

		pSelectedCharacter->RemoveInjury(Modifier);
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
			ConsumeWood(1, pSelectedCharacter, 0);
			if (Action->EntangledDeadEncounter)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Has DEADENC"))
				auto EntangledDeadEncounter = Cast<AEncounter_Dead>(Action->EntangledDeadEncounter);

				FTransform EffectTransform;
				EffectTransform.SetLocation(EntangledDeadEncounter->GetActorLocation());
				Encounter_DeadsInPlay.RemoveSingle(EntangledDeadEncounter);
				Cast<AEncounter_Dead>(Action->EntangledDeadEncounter)->Deactivate();
				//if (!(EntangledDeadEncounter->IsOnPlot)) {
				//	EntangledDeadEncounter->CreatedAction->UnregisterComponent();
				//}
				if (BurningEffectClass) {

					GetWorld()->SpawnActor<AActor>(BurningEffectClass, EffectTransform);
				}
			}
			//else if (Cast<ALocationTemplate>(Action->GetParentActor())) {
			//}
			/*else if (Action->EntangledInteractiveLocationDecoration)
			{
				FTransform EffectTransform;
				EffectTransform.SetLocation(Action->EntangledInteractiveLocationDecoration->GetActorLocation() + FVector(0.f,-3.f,0.f));
				
				Action->EntangledInteractiveLocationDecoration->Destroy();
				if (BurningEffectClass) {
		
					GetWorld()->SpawnActor<AActor>(BurningEffectClass, EffectTransform);
				}
			}*/
			return true;
		}
		Message[0] = "No wood";
			return false;
		break;

	case EActionType::Get_Injury:

		pSelectedCharacter->GetInjury(Modifier);

		return true;
		break;


	case EActionType::ArrowRight_Good:

		if (pSelectedCharacter->MovementPoints < 1) { Message[0] = "No Movement Points"; return false; }
		if (Cast<ALocationTemplate>(Action->GetParentActor()))
		{
			if (Cast<ALocationTemplate>(Action->GetParentActor())->HorizontalIndex == 3)
			{
				Message[0] = "Dead End";
				Action->Deactivate();
				return false;
			}
		}
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

		//TArray<AActor*> AllLocationsInPlayActors;
		//for (int32 i = 0; i < AllLocationsInPlay.Num(); i++)
		//{
		//	if (AllLocationsInPlay[i])
		//	{
		//		AllLocationsInPlayActors.Add(Cast<AActor>(AllLocationsInPlay[i]));
		//	}
		//}

		Action->SetTeleport(AllLocationsInPlay);
		TeleportCharacter(pSelectedCharacter, Action->LocationArrowPointsTo);


		return true;
		break;

	case EActionType::Survive:

		//MoveCharacter(pSelectedCharacter, Action->LocationArrowPointsTo);
		pSelectedCharacter->Survive();
		DeselectCharacter(pSelectedCharacter);

		return true;
		break;

	case EActionType::PickUpItem:

		if (PassItemToPlayer(Action->ItemToGetClass))
		{
			return true;
		}

		return false;
		break;

	default:

		return false;
		break;
	}
	return false;
}

void AWeirdThingsPlayerController::TryToUnlock(AAction* CurrentAction)
{
	switch (CurrentAction->ActionLockType[CurrentAction->CurrentLockIndex])
	{
	case EActionLockType::Need_Food:
		if (ConsumeFood(1, pSelectedCharacter, 1)) {
			//return true;
			CurrentAction->Unlock();
		}
		break;

	case EActionLockType::Need_Wood:
		if (ConsumeWood(1, pSelectedCharacter, 1)) {
			//return true;
			CurrentAction->Unlock();
		}
		break;

	case EActionLockType::Need_Tool:

		CanLeftActiveItemUnlock(EItemType::Tool);
		CanRightActiveItemUnlock(EItemType::Tool);

		//return true;
		//if (FindAndUseItemToUnlock(EItemType::Tool)) {
		//	CurrentAction->Unlock();
		//}

		break;

	case EActionLockType::Need_Axe:

		CanLeftActiveItemUnlock(EItemType::Axe);
		CanRightActiveItemUnlock(EItemType::Axe);

		//return true;

		//if (FindAndUseItemToUnlock(EItemType::Axe)) {
			//CurrentAction->Unlock();
		//}

		break;

	case EActionLockType::Need_Exhaustion:

		pSelectedCharacter->GetExhaustion(1);

		//return true;

		CurrentAction->Unlock();

		break;

	case EActionLockType::Need_Insanity:

		pSelectedCharacter->GetInsanity(1);

		//return true;

		CurrentAction->Unlock();

		break;

	case EActionLockType::Need_Item_C:

		//if (FindAndUseItemToUnlock(EItemValue::C)) {

			//return true;
			CanLeftActiveItemUnlock(EItemValue::C);
			CanRightActiveItemUnlock(EItemValue::C);
			//CurrentAction->Unlock();
		//}

		break;

	case EActionLockType::Need_Item_S:

		//if (FindAndUseItemToUnlock(EItemValue::S)) {

			//return true;
			CanLeftActiveItemUnlock(EItemValue::S);
			CanRightActiveItemUnlock(EItemValue::S);
			//CurrentAction->Unlock();
		//}

		break;

	case EActionLockType::Need_Item_G:

		//if (FindAndUseItemToUnlock(EItemValue::G)) {

			CanLeftActiveItemUnlock(EItemValue::G);
			CanRightActiveItemUnlock(EItemValue::G);
			//return true;
			//CurrentAction->Unlock();
		//}

		break;

	case EActionLockType::Need_Shovel:

		CanLeftActiveItemUnlock(EItemType::Shovel);
		CanRightActiveItemUnlock(EItemType::Shovel);

		//return true;


		//if (FindAndUseItemToUnlock(EItemType::Shovel)) {
		//	CurrentAction->Unlock();
		//}

		break;

	default:

		CanLeftActiveItemUnlock(EItemType::No_Type);
		CanRightActiveItemUnlock(EItemType::No_Type);

		//return false;

		break;
	}
	//return false;
}

bool AWeirdThingsPlayerController::CanLeftActiveItemUnlock(EItemType BackpackItemType)
{
	if (pSelectedCharacter) {

		UE_LOG(LogTemp, Warning, TEXT("Checking if first item can ulock"))

			auto Backpack = pSelectedCharacter->Backpack;
		auto LeftActiveItemIndex = Backpack.Num() - 1;

		if (Backpack[LeftActiveItemIndex])
		{
			auto ItemType = Backpack[LeftActiveItemIndex]->ItemType;
			for (int32 i = 0; i < ItemType.Num(); i++)
			{
				if ((ItemType[i] == BackpackItemType) && (ItemType[i] != EItemType::No_Type))
				{
					pSelectedCharacter->LeftActiveItem = Backpack[LeftActiveItemIndex];
					UE_LOG(LogTemp, Warning, TEXT("%s is ready for use"), *Backpack[LeftActiveItemIndex]->GetName())
						return true;
				}

			}
			pSelectedCharacter->LeftActiveItem = nullptr;
			
			return false;
		}
		pSelectedCharacter->LeftActiveItem = nullptr;
		return false;
	}
	else if (CharacterPickingToFight)
	{
		UE_LOG(LogTemp, Warning, TEXT("Checking if first item can unlock"))

			auto Backpack = CharacterPickingToFight->Backpack;
		auto LeftActiveItemIndex = Backpack.Num() - 1;

		if (Backpack[LeftActiveItemIndex])
		{
			auto ItemType = Backpack[LeftActiveItemIndex]->ItemType;
			for (int32 i = 0; i < ItemType.Num(); i++)
			{
				if (ItemType[i] == BackpackItemType)
				{
					CharacterPickingToFight->LeftActiveItem = Backpack[LeftActiveItemIndex];
					UE_LOG(LogTemp, Warning, TEXT("%s is ready for use"), *Backpack[LeftActiveItemIndex]->GetName())
						return true;
				}

			}
			CharacterPickingToFight->LeftActiveItem = nullptr;
			return false;
		}
		CharacterPickingToFight->LeftActiveItem = nullptr;
		return false;
	}
	return false;
}

bool AWeirdThingsPlayerController::CanRightActiveItemUnlock(EItemType BackpackItemType)
{
	if (pSelectedCharacter) {

		UE_LOG(LogTemp, Warning, TEXT("Checking if second item can unlock"))

			auto Backpack = pSelectedCharacter->Backpack;
		auto RightActiveItemIndex = Backpack.Num() - 2;

		if (Backpack[RightActiveItemIndex])
		{
			auto ItemType = Backpack[RightActiveItemIndex]->ItemType;
			for (int32 i = 0; i < ItemType.Num(); i++)
			{
				if ((ItemType[i] == BackpackItemType) && (ItemType[i] != EItemType::No_Type))
				{
					pSelectedCharacter->RightActiveItem = Backpack[RightActiveItemIndex];
					UE_LOG(LogTemp, Warning, TEXT("%s is ready for use"), *Backpack[RightActiveItemIndex]->GetName())
						return true;
				}

			}
			pSelectedCharacter->RightActiveItem = nullptr;
			return false;
		}
		pSelectedCharacter->RightActiveItem = nullptr;
		return false;
	}
	else if (CharacterPickingToFight)
	{
		UE_LOG(LogTemp, Warning, TEXT("Checking if second item can ulock"))

			auto Backpack = CharacterPickingToFight->Backpack;
		auto RightActiveItemIndex = Backpack.Num() - 2;

		if (Backpack[RightActiveItemIndex])
		{
			auto ItemType = Backpack[RightActiveItemIndex]->ItemType;
			for (int32 i = 0; i < ItemType.Num(); i++)
			{
				if (ItemType[i] == BackpackItemType)
				{
					CharacterPickingToFight->RightActiveItem = Backpack[RightActiveItemIndex];
					UE_LOG(LogTemp, Warning, TEXT("%s is ready for use"), *Backpack[RightActiveItemIndex]->GetName())
						return true;
				}

			}
			CharacterPickingToFight->RightActiveItem = nullptr;
			return false;
		}
		CharacterPickingToFight->RightActiveItem = nullptr;
		return false;
	}
	return false;
}

bool AWeirdThingsPlayerController::CanLeftActiveItemUnlock(EItemValue BackpackItemValue)
{
	if (pSelectedCharacter) {

			auto Backpack = pSelectedCharacter->Backpack;
		auto LeftActiveItemIndex = Backpack.Num() - 1;

		if (Backpack[LeftActiveItemIndex])
		{
			auto ItemValue = Backpack[LeftActiveItemIndex]->ItemValue;
			
				if (ItemValue == BackpackItemValue)
				{
					pSelectedCharacter->LeftActiveItem = Backpack[LeftActiveItemIndex];
					UE_LOG(LogTemp, Warning, TEXT("%s is ready for use"), *Backpack[LeftActiveItemIndex]->GetName())
						return true;
				}

		
			pSelectedCharacter->LeftActiveItem = nullptr;
			return false;
		}
		pSelectedCharacter->LeftActiveItem = nullptr;
		return false;
	}
	/*
	else if (CharacterPickingToFight)
	{
		UE_LOG(LogTemp, Warning, TEXT("Checking if first item can unlock"))

			auto Backpack = CharacterPickingToFight->Backpack;
		auto LeftActiveItemIndex = Backpack.Num() - 1;

		if (Backpack[LeftActiveItemIndex])
		{
			auto ItemType = Backpack[LeftActiveItemIndex]->ItemType;
			for (int32 i = 0; i < ItemType.Num(); i++)
			{
				if (ItemType[i] == BackpackItemType)
				{
					CharacterPickingToFight->LeftActiveItem = Backpack[LeftActiveItemIndex];
					UE_LOG(LogTemp, Warning, TEXT("%s is ready for use"), *Backpack[LeftActiveItemIndex]->GetName())
						return true;
				}

			}
			CharacterPickingToFight->LeftActiveItem = nullptr;
			return false;
		}
		CharacterPickingToFight->LeftActiveItem = nullptr;
		return false;
	}*/
	return false;
}

bool AWeirdThingsPlayerController::CanRightActiveItemUnlock(EItemValue BackpackItemValue)
{
	if (pSelectedCharacter) {

		auto Backpack = pSelectedCharacter->Backpack;
		auto RightActiveItemIndex = Backpack.Num() - 2;

		if (Backpack[RightActiveItemIndex])
		{
			auto ItemValue = Backpack[RightActiveItemIndex]->ItemValue;

			if (ItemValue == BackpackItemValue)
			{
				pSelectedCharacter->RightActiveItem = Backpack[RightActiveItemIndex];
				UE_LOG(LogTemp, Warning, TEXT("%s is ready for use"), *Backpack[RightActiveItemIndex]->GetName())
					return true;
			}


			pSelectedCharacter->RightActiveItem = nullptr;
			return false;
		}
		pSelectedCharacter->RightActiveItem = nullptr;
		return false;
	}
	/*
	else if (CharacterPickingToFight)
	{
		UE_LOG(LogTemp, Warning, TEXT("Checking if first item can unlock"))

			auto Backpack = CharacterPickingToFight->Backpack;
		auto LeftActiveItemIndex = Backpack.Num() - 1;

		if (Backpack[LeftActiveItemIndex])
		{
			auto ItemType = Backpack[LeftActiveItemIndex]->ItemType;
			for (int32 i = 0; i < ItemType.Num(); i++)
			{
				if (ItemType[i] == BackpackItemType)
				{
					CharacterPickingToFight->LeftActiveItem = Backpack[LeftActiveItemIndex];
					UE_LOG(LogTemp, Warning, TEXT("%s is ready for use"), *Backpack[LeftActiveItemIndex]->GetName())
						return true;
				}

			}
			CharacterPickingToFight->LeftActiveItem = nullptr;
			return false;
		}
		CharacterPickingToFight->LeftActiveItem = nullptr;
		return false;
	}*/
	return false;
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

void AWeirdThingsPlayerController::ActivateEntangledILD(AAction* ActionEntangledWithILD)
{
	if (!ActionEntangledWithILD->EntangledInteractiveLocationDecoration) { return; }
	Cast<AInteractiveLocationDecoration>(ActionEntangledWithILD->EntangledInteractiveLocationDecoration)->Activate();
}

void AWeirdThingsPlayerController::DeactivateEntangledILD(AAction* ActionEntangledWithILD)
{
	if (!ActionEntangledWithILD->EntangledInteractiveLocationDecoration) { return; }
	Cast<AInteractiveLocationDecoration>(ActionEntangledWithILD->EntangledInteractiveLocationDecoration)->Deactivate();
}

void AWeirdThingsPlayerController::CreateDoor(AActor* LocationWithDoorCreatedActor, TSubclassOf<AInteractiveLocationDecoration> DoorToCreateClass, TSubclassOf<AAction> TeleportActionToCreateClass, AActor* LocationInstigatorActor)
{
	auto LocationWithDoorCreated = Cast<ALocationTemplate>(LocationWithDoorCreatedActor);
	auto LocationInstigator = Cast<ALocationTemplate>(LocationInstigatorActor);
	auto Door = LocationWithDoorCreated->Door;

	if (!ensure(LocationInstigator)) { return; }

	if (Door) {

		UE_LOG(LogTemp, Warning, TEXT("Door already exists"))
			return;
	}
	if (!DoorToCreateClass) {

		UE_LOG(LogTemp, Error, TEXT("Door cannot be created: no ILD class"))
			return;
	}
	if (!TeleportActionToCreateClass) {

		UE_LOG(LogTemp, Error, TEXT("Door cannot be created: no action class"))
			return;
	}

	if (!LocationWithDoorCreated) { UE_LOG(LogTemp, Error, TEXT("NoLocationWithDoorCreated")); return; }
	//Door = NewObject<UChildActorComponent>(this, ("Door"));

	//Door->RegisterComponent();
	LocationWithDoorCreated->CreateDoorComponent(LocationWithDoorCreated->SocketDoor->GetComponentLocation());
	Door = LocationWithDoorCreated->Door;
	Door->SetChildActorClass(DoorToCreateClass);

	
	//Door->SetWorldLocation(LocationWithDoorCreated->SocketDoor->GetComponentLocation());
	LocationWithDoorCreated->CreateDynamicAction(TeleportActionToCreateClass, LocationInstigator);// Door, LocationInstigator);

}

void AWeirdThingsPlayerController::SetForcedActionForLocation(AActor* LocationToSetForcedActionOn, AActor* ActionToSetAsForced)
{
	Cast<ALocationTemplate>(LocationToSetForcedActionOn)->ForcedAction = Cast<AAction>(ActionToSetAsForced);
}

void AWeirdThingsPlayerController::CreateDynamicAction(AActor* CurrentLocationActor, TSubclassOf<AAction> ActionClass, AEncounter_Dead* EntangledDead)
{
	

		auto CurrentLocation = Cast<ALocationTemplate>(CurrentLocationActor);
	auto DynamicAction = CurrentLocation->DynamicAction;

	for (int32 i = 0; i < DynamicAction.Num(); i++)
	{
		if (DynamicAction[i]) {
			continue;
		}
		if (!CurrentLocation->AvailableSocketDynamicPlayerAction.Last()) { return; }
		DynamicAction[i] = NewObject<UChildActorComponent>(CurrentLocation, ("Action_Dead" + i));

		DynamicAction[i]->RegisterComponent();
		DynamicAction[i]->SetChildActorClass(ActionClass);
		DynamicAction[i]->SetWorldLocation(CurrentLocation->AvailableSocketDynamicPlayerAction.Last()->GetComponentLocation());
		Cast<AAction>(DynamicAction[i]->GetChildActor())->EntangledDeadEncounter = EntangledDead;
		UE_LOG(LogTemp, Error, TEXT("Socket: %s"),*CurrentLocation->AvailableSocketDynamicPlayerAction.Last()->GetName())
		EntangledDead->CreatedAction = DynamicAction[i];

		return;
	}

}

void AWeirdThingsPlayerController::EntangleActionWithActor(UChildActorComponent* Action, UChildActorComponent* InteractiveLocationDecoration)
{
	auto ActionToEntangle = Cast<AAction>(Action->GetChildActor());
	auto InteractiveLocationDecorationToEntangle = Cast<AInteractiveLocationDecoration>(InteractiveLocationDecoration->GetChildActor());

	ActionToEntangle->EntangledInteractiveLocationDecoration = InteractiveLocationDecorationToEntangle;
	InteractiveLocationDecorationToEntangle->EntangledAction = ActionToEntangle;
}

// Get Service/Item from Encounter, pay for it
// *ResultOfTrading - type of Action Character gets as a Service/Item
// *ItemRequiredToTrade - Item used by Character as payment
// IsRight - was right Mouse Button clicked or not?
// Return true if trade happened
bool AWeirdThingsPlayerController::Trade(EActionType ResultOfTrading, EActionLockType ItemRequiredToTrade, bool IsRight)
{
	auto CanRequiredItemBeConsumed = false;
	if (!pSelectedCharacter) { return false; }

	switch (ItemRequiredToTrade)
	{
	case EActionLockType::Need_Food:
		if (pSelectedCharacter->Food>=1) {

			CanRequiredItemBeConsumed = true;
		}
		else {
			Message[0] = "No Food";
		}
		break;

	case EActionLockType::Need_Wood:
		if (pSelectedCharacter->Wood >= 1) {
			CanRequiredItemBeConsumed = true;
		}
		break;

	case EActionLockType::Need_Item_G:

		

		if (IsRight) {
			if(CanRightActiveItemUnlock(EItemValue::G)){ CanRequiredItemBeConsumed = true; }
			else { CanRequiredItemBeConsumed = false; }
		}
		else {
			if (CanLeftActiveItemUnlock(EItemValue::G)) { CanRequiredItemBeConsumed = true; }
			else { CanRequiredItemBeConsumed = false; }
		}

		//if (pSelectedCharacter->Wood >= 1) {
			
		//}
		break;

	default:

		break;
	}

	if (CanRequiredItemBeConsumed)
	{
		switch (ResultOfTrading)
		{

		case EActionType::Get_Item_C:
			if (CurrentlyHoveredByMouseEncounter_Good) {
				if (CurrentlyHoveredByMouseEncounter_Good->LBItemToSellClass)
				{
					pSelectedCharacter->GetItem(GetWorld()->SpawnActor<AItemTemplate>(CurrentlyHoveredByMouseEncounter_Good->LBItemToSellClass));
				}
			}
			break;

		case EActionType::Get_Item_S:

			if (CurrentlyHoveredByMouseEncounter_Good) {
				if (CurrentlyHoveredByMouseEncounter_Good->RBItemToSellClass)
				{
					pSelectedCharacter->GetItem(GetWorld()->SpawnActor<AItemTemplate>(CurrentlyHoveredByMouseEncounter_Good->RBItemToSellClass));
				}
			}
			break;

		case EActionType::Get_Item_G:

			if (CurrentlyHoveredByMouseEncounter_Good) {
				if (CurrentlyHoveredByMouseEncounter_Good->RBItemToSellClass)
				{
					pSelectedCharacter->GetItem(GetWorld()->SpawnActor<AItemTemplate>(CurrentlyHoveredByMouseEncounter_Good->RBItemToSellClass));
				}
			}
			break;

		case EActionType::Hire:

			if (CurrentlyHoveredByMouseEncounter_Good) {
				if (!pSelectedCharacter->SetHiredCompanion(CurrentlyHoveredByMouseEncounter_Good))
				{
					
					return false;
				}
				CurrentlyHoveredByMouseEncounter_Good->SetAsHired();
			}

			break;
		case EActionType::Get_Food:

			GetFood(1);

			break;

		case EActionType::Get_Wood:

			GetWood(1);

			break;

		case EActionType::Heal:

			if (pSelectedCharacter) {
				if (!pSelectedCharacter->RemoveInjury(1)) {
					return false;
				}
				
			}
			break;
	
		default:


			break;
		}

		if (ItemRequiredToTrade == EActionLockType::Need_Food){
			RemoveFood(1, pSelectedCharacter, 0);
		}
		else if (ItemRequiredToTrade == EActionLockType::Need_Wood) {
			ConsumeWood(1, pSelectedCharacter, 0);
			}
		else if (ItemRequiredToTrade == EActionLockType::Need_Item_G) {

			if (IsRight) {
				if (CanRightActiveItemUnlock(EItemValue::G)) {
					pSelectedCharacter->RightActiveItem->Destroy();
					pSelectedCharacter->RightActiveItem = nullptr;
					pSelectedCharacter->Backpack[pSelectedCharacter->Backpack.Num() - 2] = nullptr;
				}
				
			}
			else {
				if (CanLeftActiveItemUnlock(EItemValue::G)) {
					pSelectedCharacter->LeftActiveItem->Destroy();
					pSelectedCharacter->LeftActiveItem = nullptr;
					pSelectedCharacter->Backpack[pSelectedCharacter->Backpack.Num() - 1] = nullptr;
				}
			}
		}
		return true;
	}
	else {
		return false;
	}

}

FTransform AWeirdThingsPlayerController::GetAvailableSocketDynamicPlayerActionTransform(AActor* LocationWithSocketActor)
{
	auto LocationWithSocket = Cast<ALocationTemplate>(LocationWithSocketActor);

	return LocationWithSocket->AvailableSocketDynamicPlayerAction.Last()->GetComponentTransform();

}

void AWeirdThingsPlayerController::DeactivateAction(AActor* ActionToDeactivate)
{
	Cast<AAction>(ActionToDeactivate)->Deactivate();
}

void AWeirdThingsPlayerController::ActivateAction(AActor* ActionToActivate)
{
	Cast<AAction>(ActionToActivate)->Activate();
}




//--------------------------------- Day/Night cycle -------------------------------------

void AWeirdThingsPlayerController::SetTimeMorning()
{
	if (!ensure(SunlightMorning)) { return; }
	if (!ensure(SunlightNoon)) { return; }
	if (!ensure(SunlightEvening)) { return; }
	if (!ensure(SunlightNight)) { return; }

	SunlightMorning->SetActorHiddenInGame(false);
	SunlightNoon->SetActorHiddenInGame(true);
	SunlightEvening->SetActorHiddenInGame(true);
	SunlightNight->SetActorHiddenInGame(true);

	AddActionPointToEveryCharacter();

	UE_LOG(LogTemp, Warning, TEXT("Setting time: Morning"))

	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		if (!PlayerCharacters[i]) { continue; }
		//if (PlayerCharacters[i]->DoesNeedToConsumeFood) {
		//	
		//	PlayerCharacters[i]->GetHunger(1);
		//}
		if (!PlayerCharacters[i]) { continue; }
		if (PlayerCharacters[i]->DoesNeedToSleep) {
			PlayerCharacters[i]->GetExhaustion(1);
		}
		if (!PlayerCharacters[i]) { continue; }
		if (PlayerCharacters[i]->DoesNeedFire) {
			PlayerCharacters[i]->DoesNeedFire = false;
		}
		if (!PlayerCharacters[i]) { continue; }
		if (PlayerCharacters[i]->IsSleeping) {
			PlayerCharacters[i]->SetIsSleeping(false);
		}
	}

	if (Encounter_DeadsInPlay.IsValidIndex(0)) {
		for (int32 i = 0; i < Encounter_DeadsInPlay.Num(); i++)
		{
			Encounter_DeadsInPlay[i]->SetAwakened(false);
		}
	}
		if (CampfiresInPlay.IsValidIndex(0)) {
			//for (int32 i = (CampfiresInPlay.Num() - 1); i >= 0; i--)
			for (int32 i = 0; i<CampfiresInPlay.Num(); i++)
			{
				if (CampfiresInPlay[i])
				{
					UE_LOG(LogTemp, Warning, TEXT("Campfire spotted: %s"), *CampfiresInPlay[i]->GetName())
						CampfiresInPlay[i]->Destroy();
						//CampfiresInPlay[i]->DestroyConstructedComponents();
					CampfiresInPlay[i] = nullptr;
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("No campfire"))
				}
			}
		}

		if (Encounter_BadInPlay.IsValidIndex(0)) {
			for (int32 i = 0; i < Encounter_BadInPlay.Num(); i++)
			{
				if (!Encounter_BadInPlay[i]) {continue;}
				InitiateCombat(Encounter_BadInPlay[i]);

			}
		}
		
	CurrentTimeOfDay = ETimeOfDay::Morning;
	
}

void AWeirdThingsPlayerController::SetTimeNoon()
{
	if (!ensure(SunlightMorning)) { return; }
	if (!ensure(SunlightNoon)) { return; }
	if (!ensure(SunlightEvening)) { return; }
	if (!ensure(SunlightNight)) { return; }

	SunlightMorning->SetActorHiddenInGame(true);
	SunlightNoon->SetActorHiddenInGame(false);
	SunlightEvening->SetActorHiddenInGame(true);
	SunlightNight->SetActorHiddenInGame(true);

	AddActionPointToEveryCharacter();

	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		if (!PlayerCharacters[i]) { continue; }
		//if (PlayerCharacters[i]->DoesNeedToConsumeFood) {
		//	
		//	PlayerCharacters[i]->GetHunger(1);
		//}
		if (!PlayerCharacters[i]) { continue; }
		if (PlayerCharacters[i]->DoesNeedToSleep) {
			PlayerCharacters[i]->GetExhaustion(1);
		}
		if (!PlayerCharacters[i]) { continue; }
		if (PlayerCharacters[i]->DoesNeedFire) {
			PlayerCharacters[i]->DoesNeedFire = false;
		}
		if (!PlayerCharacters[i]) { continue; }
		if (PlayerCharacters[i]->IsSleeping) {
			PlayerCharacters[i]->SetIsSleeping(false);
		}
	}

	if (Encounter_BadInPlay.IsValidIndex(0)) {
		for (int32 i = 0; i < Encounter_BadInPlay.Num(); i++)
		{
			if (!Encounter_BadInPlay[i]) { continue; }
			InitiateCombat(Encounter_BadInPlay[i]);

		}
	}

	CurrentTimeOfDay = ETimeOfDay::Noon;
}

void AWeirdThingsPlayerController::SetTimeEvening()
{
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
	if (Encounter_DeadsInPlay.IsValidIndex(0)) {
		for (int32 i = 0; i < Encounter_DeadsInPlay.Num(); i++)
		{
			if (Encounter_DeadsInPlay[i]) {
				Encounter_DeadsInPlay[i]->SetAwakened(true);
			}
		}
	}
	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		if (!PlayerCharacters[i]) { continue; }
		//if (PlayerCharacters[i]->DoesNeedToConsumeFood) {
		//	
		//	PlayerCharacters[i]->GetHunger(1);
		//}
		if (!PlayerCharacters[i]) { continue; }
		if (PlayerCharacters[i]) {
			PlayerCharacters[i]->DoesNeedToConsumeFood = true;
		}
		if (!PlayerCharacters[i]) { continue; }
		if (PlayerCharacters[i]->DoesNeedToSleep) {
			PlayerCharacters[i]->GetExhaustion(1);
		}
		if (!PlayerCharacters[i]) { continue; }
		if (!(Cast<ALocationTemplate>(PlayerCharacters[i]->CurrentLocation)->AvailableSocketCampFire.Num() < 1))
		{
			if (PlayerCharacters[i]) {
				PlayerCharacters[i]->DoesNeedFire = true;
			}
		}
		if (!PlayerCharacters[i]) { continue; }
		if (PlayerCharacters[i]->IsSleeping) {
			PlayerCharacters[i]->SetIsSleeping(false);
		}
	}

	if (Encounter_DeadsInPlay.IsValidIndex(0)) {
		for (int32 i = 0; i < Encounter_DeadsInPlay.Num(); i++)
		{
			if (!Encounter_DeadsInPlay[i]) { continue; }
			//InitiateCombat(Encounter_BadInPlay[i]);
			//Move_Encounter_Dead(Encounter_DeadsInPlay[i]);
			Encounter_DeadLookForPlayerToAttack(Encounter_DeadsInPlay[i]);
			break;
		}
	}

	if (Encounter_BadInPlay.IsValidIndex(0)) {
		for (int32 i = 0; i < Encounter_BadInPlay.Num(); i++)
		{
			if (!Encounter_BadInPlay[i]) { continue; }
			InitiateCombat(Encounter_BadInPlay[i]);
			//Move_Encounter_Dead(Encounter_DeadsInPlay[i]);
			//Encounter_DeadLookForPlayerToAttack(Encounter_DeadsInPlay[i]);
			break;
		}
	}

	CurrentTimeOfDay = ETimeOfDay::Evening;
}

void AWeirdThingsPlayerController::SetTimeNight()
{
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
		if (!PlayerCharacters[i]) { continue; }
		if (PlayerCharacters[i]->DoesNeedToConsumeFood) {
			
			PlayerCharacters[i]->GetHunger(1);
			if (PlayerCharacters[i]) {
				PlayerCharacters[i]->DoesNeedToConsumeFood = false;
			}
		}
		if (!PlayerCharacters[i]) { continue; }
		if (PlayerCharacters[i]->DoesNeedToSleep) {
			PlayerCharacters[i]->GetExhaustion(1);
		}
		if (!PlayerCharacters[i]) { continue; }
		if (PlayerCharacters[i]) {
			PlayerCharacters[i]->DoesNeedToSleep = true;
		}
		if (!PlayerCharacters[i]) { continue; }
		if (PlayerCharacters[i]->DoesNeedFire){
		//if (!(Cast<ALocationTemplate>(PlayerCharacters[i]->CurrentLocation)->AvailableSocketCampFire.Num() < 1)) {
			PlayerCharacters[i]->GetInsanity(1);

			if (PlayerCharacters[i]) {
				PlayerCharacters[i]->DoesNeedFire = false;
			}
		}
		if (!PlayerCharacters[i]) { continue; }
		if (PlayerCharacters[i]->IsSleeping) {
			PlayerCharacters[i]->SetIsSleeping(false);
		}
	}

	//TODO collapse to function

	if (Encounter_DeadsInPlay.IsValidIndex(0)) {
		for (int32 i = 0; i < Encounter_DeadsInPlay.Num(); i++)
		{
			if (!Encounter_DeadsInPlay[i]) { continue; }
			//InitiateCombat(Encounter_BadInPlay[i]);
			Move_Encounter_Dead(Encounter_DeadsInPlay[i]);
			Encounter_DeadLookForPlayerToAttack(Encounter_DeadsInPlay[i]);
			break;
		}
	}

	if (Encounter_BadInPlay.IsValidIndex(0)) {
		for (int32 i = 0; i < Encounter_BadInPlay.Num(); i++)
		{
			if (!Encounter_BadInPlay[i]) { continue; }
			InitiateCombat(Encounter_BadInPlay[i]);
			//Move_Encounter_Dead(Encounter_DeadsInPlay[i]);
			//Encounter_DeadLookForPlayerToAttack(Encounter_DeadsInPlay[i]);
			break;
		}
	}

	CurrentTimeOfDay = ETimeOfDay::Night;

	UpdateCharactersFoodInPlay();
	UpdateCharactersWoodInPlay();
}




//--------------------------------- Character interaction -------------------------------

void AWeirdThingsPlayerController::RefreshCharacterMP()
{
	/* To DELETE----------------------------------------
	for (int32 i = 0; i < pSelectedCharacter->Backpack.Num(); i++)
	{
		if (!ensure(pSelectedCharacter->Backpack[i])) { return; }
		UE_LOG(LogTemp, Warning, TEXT("%s") ,*pSelectedCharacter->Backpack[i]->GetName())
	}
	*/

	if (CombatManagersInPlay.IsValidIndex(0)) { return; }

	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		if (!PlayerCharacters[i]) { continue; }
		PlayerCharacters[i]->MovementPoints = PlayerCharacters[i]->InitialMovementPoints;
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

void AWeirdThingsPlayerController::RemoveActionPointsFromCharacter(AWTPlayerCharacter* AffectedCharacter, int32 AmountToRemove)
{
	if (!AffectedCharacter) { return; }
	AffectedCharacter->RemoveActionPoints(AmountToRemove);
}

bool AWeirdThingsPlayerController::ConsumeFood(int32 FoodAmountToConsume, AWTPlayerCharacter* AffectedCharacter, int32 ActionPointsRequired) //TODO make applicable regardless selected character or not
{
	UE_LOG(LogTemp, Warning, TEXT("Consuming Food"))
	if (!AffectedCharacter) { return false; }

	if (AffectedCharacter->IsSleeping) { Message[0] = "Can't while sleeping"; return false; }
	if (AffectedCharacter->IsInCombat) { Message[0] = "Can't while in combat"; return false; }
	//if (PlayerCharacter->IsPickingEnemyToFight) { return; }
	//if (PlayerCharacter->IsSelectedForCombat) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Inside Consuming Food"))
	if (CurrentTimeOfDay == ETimeOfDay::Evening) {
		UE_LOG(LogTemp, Warning, TEXT("Evening"))
	}
	else if (AffectedCharacter->CurrentActionPoints < ActionPointsRequired)
	{
		Message[0] = "No Action Points";
		UE_LOG(LogTemp, Warning, TEXT("Not enough AP"))
			return false;
	}

	if ((AffectedCharacter->Food - FoodAmountToConsume) < 0)
	{
		Message[0] = "No Food";
		UE_LOG(LogTemp, Warning, TEXT("%s"),*Message[0])
		// TODO "Not enough food"
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("Substracting food"))
	AffectedCharacter->Food -= FoodAmountToConsume;
	AffectedCharacter->RemoveHunger(1);

	if (!(CurrentTimeOfDay == ETimeOfDay::Evening)) {
		RemoveActionPointsFromCharacter(AffectedCharacter, ActionPointsRequired);
		//AffectedCharacter->CurrentActionPoints -= ActionPointsRequired;
	}

	return true;
}

bool AWeirdThingsPlayerController::RemoveFood(int32 FoodAmountToConsume, AWTPlayerCharacter* AffectedCharacter, int32 ActionPointsRequired) //TODO make applicable regardless selected character or not
{
	if (!AffectedCharacter) { return false; }

	if (AffectedCharacter->CurrentActionPoints < ActionPointsRequired)
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
	//AffectedCharacter->RemoveHunger(1);

	RemoveActionPointsFromCharacter(AffectedCharacter, ActionPointsRequired);
	//AffectedCharacter->CurrentActionPoints -= ActionPointsRequired;

	return true;
}

bool AWeirdThingsPlayerController::ConsumeWood(int32 WoodAmountToConsume, AWTPlayerCharacter* AffectedCharacter, int32 ActionPointsRequired) //TODO make applicable regardless selected character or not
{
	if (!AffectedCharacter) { return false; }

	if (AffectedCharacter->CurrentActionPoints < ActionPointsRequired)
	{
			Message[0] = "No Action Points";
			return false;
	}

	if ((AffectedCharacter->Wood - WoodAmountToConsume) < 0)
	{
		// TODO "Not enough wood"
		Message[0] = "No Wood";
		return false;
	}
	AffectedCharacter->Wood -= WoodAmountToConsume;
	RemoveActionPointsFromCharacter(AffectedCharacter, ActionPointsRequired);
	//AffectedCharacter->CurrentActionPoints -= ActionPointsRequired;
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
	UpdateCharactersFoodInPlay();
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
	UpdateCharactersWoodInPlay();
	return true;
}

 void AWeirdThingsPlayerController::MakeCampFire(AWTPlayerCharacter* PlayerCharacter)
{

	//GetWorld()->SpawnActor<AActor>(CampFireClassToSpawn, PlayerCharacter->CurrentLocation->SocketCampFire->GetComponentLocation());
	//PlayerCharacter->CurrentLocation->SocketCampFire;

	 if (PlayerCharacter->IsSleeping) { Message[0] = "Can't while sleeping"; return; }
	 if (PlayerCharacter->IsInCombat) { Message[0] = "Can't while in combat"; return; }
	 //if (PlayerCharacter->IsPickingEnemyToFight) { return; }
	 //if (PlayerCharacter->IsSelectedForCombat) { return; }

	 int32 ActionPointsRequired = 1;

	 if ((CurrentTimeOfDay == ETimeOfDay::Evening)) {
		 ActionPointsRequired = 0;
	 }
	 else if (PlayerCharacter->CurrentActionPoints < ActionPointsRequired)
	 {
		 Message[0] = "No Action Points";
			 return;
	 }

	 
	auto AvailableCampFireSocket = Cast<ALocationTemplate>(PlayerCharacter->CurrentLocation)->AvailableSocketCampFire;
	if (!(AvailableCampFireSocket.IsValidIndex(0))) { 
		Message[0] = "Have a campfire already";
		return;
	}
	//if (!ensure(AvailableCampFireSocket)) { return; }

	auto SpawningSocketTransform = AvailableCampFireSocket[0]->GetComponentTransform();

	SpawningSocketTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

	FActorSpawnParameters SpawnParameters;

	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	auto SpawningClass = CampFireClassToSpawn;

	if (!ensure(CampFireClassToSpawn)) {
		UE_LOG(LogTemp, Warning, TEXT("Campfire Not Spawned"))
			return;
	}
	if (ConsumeWood(1, PlayerCharacter, ActionPointsRequired))
	{
		
		CampfiresInPlay.Add(GetWorld()->SpawnActor<AActor>(CampFireClassToSpawn, SpawningSocketTransform, SpawnParameters));

		for (int32 i = 0; i < PlayerCharacters.Num(); i++)
		{
			
			if (!PlayerCharacters[i]) { continue; }
			if (PlayerCharacters[i]->CurrentLocation != PlayerCharacter->CurrentLocation) { continue; }
			if (PlayerCharacters[i]->DoesNeedFire) {
				PlayerCharacters[i]->DoesNeedFire = false;
			}
		}
	}
	else {

		Message[0] = "No Wood";
	}

	//PlayerCharacter->CurrentActionPoints--;
}

void AWeirdThingsPlayerController::Sleep(AWTPlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter->IsSleeping) { Message[0] = "Already sleeping"; return; }
	if (PlayerCharacter->IsInCombat) { Message[0] = "Can't sleep in combat"; return; }
	//if (PlayerCharacter->IsPickingEnemyToFight) { return; }
	//if (PlayerCharacter->IsSelectedForCombat) { return; }
	if (PlayerCharacter->CurrentActionPoints < 1)
	{
		Message[0] = "No Action Points";
			return;
	}

	PlayerCharacter->RemoveExhaustion(1);

	PlayerCharacter->SetIsSleeping(true);
		PlayerCharacter->CurrentActionPoints = 0;
		PlayerCharacter->MovementPoints = 0;
		if (PlayerCharacter == pSelectedCharacter) { DeselectCharacter(pSelectedCharacter); }
}

void AWeirdThingsPlayerController::AddActionPointToEveryCharacter()
{
	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		if (PlayerCharacters[i]) {
			PlayerCharacters[i]->CurrentActionPoints = PlayerCharacters[i]->ActionPoints;
		}
	}
}




//----------------------------- Currently hovered by mouse actors -------------------------------------------------------------

void AWeirdThingsPlayerController::SetCurrentlyHoveredByMouseAction(bool IsHovered, AAction* ActionToSet)
{
	if (IsHovered)
	{
		CurrentlyHoveredByMouseAction = ActionToSet;
	}
	else
	{
		CurrentlyHoveredByMouseAction = nullptr;
	}

	if (CurrentlyHoveredByMouseAction)
	{
		if (!pSelectedCharacter) { return; }
		if (!pSelectedCharacter->IsInCombat) {
			//CanLeftActiveItemUnlock(EItemType::Weapon);
			//CanRightActiveItemUnlock(EItemType::Weapon);
			pSelectedCharacter->LeftActiveItem = nullptr;
			pSelectedCharacter->RightActiveItem = nullptr;

			switch (CurrentlyHoveredByMouseAction->ActionLockType[CurrentlyHoveredByMouseAction->CurrentLockTypeIndex])
			{
			case EActionLockType::Need_Axe:
				CanLeftActiveItemUnlock(EItemType::Axe);
				CanRightActiveItemUnlock(EItemType::Axe);

				break;

			case EActionLockType::Need_Shovel:
				CanLeftActiveItemUnlock(EItemType::Shovel);
				CanRightActiveItemUnlock(EItemType::Shovel);

				break;

			case EActionLockType::Need_Tool:
				CanLeftActiveItemUnlock(EItemType::Tool);
				CanRightActiveItemUnlock(EItemType::Tool);

				break;

			case EActionLockType::Need_Weapon:
				CanLeftActiveItemUnlock(EItemType::Weapon);
				CanRightActiveItemUnlock(EItemType::Weapon);

				break;

			case EActionLockType::Need_Item_C:
				CanLeftActiveItemUnlock(EItemValue::C);
				CanRightActiveItemUnlock(EItemValue::C);

				break;

			case EActionLockType::Need_Item_S:
				CanLeftActiveItemUnlock(EItemValue::S);
				CanRightActiveItemUnlock(EItemValue::S);

				break;

			case EActionLockType::Need_Item_G:
				CanLeftActiveItemUnlock(EItemValue::G);
				CanRightActiveItemUnlock(EItemValue::G);

				break;

			default:

				CanLeftActiveItemUnlock(EItemType::No_Type);
				CanRightActiveItemUnlock(EItemType::No_Type);

				break;
			}

		}
		//CurrentlyHoveredByMouseAction->ActionLockType[CurrentlyHoveredByMouseAction->CurrentLockTypeIndex]
		//TryToUnlock(CurrentlyHoveredByMouseAction);
	}

}

void AWeirdThingsPlayerController::SetCurrentlyHoveredByMouseEncounter_Good(bool IsHovered, AEncounter_Good* Encounter_GoodToSet)
{
	if (IsHovered)
	{
		CurrentlyHoveredByMouseEncounter_Good = Encounter_GoodToSet;
	}
	else
	{
		CurrentlyHoveredByMouseEncounter_Good = nullptr;
	}

	if (CurrentlyHoveredByMouseEncounter_Good) {
		if (!pSelectedCharacter) { return; }
		if (pSelectedCharacter->IsInCombat) {
			CanLeftActiveItemUnlock(EItemType::Weapon);
			CanRightActiveItemUnlock(EItemType::Weapon);
		}
		else {
			switch (CurrentlyHoveredByMouseEncounter_Good->LBTradingRequiredItem)
			{
			case EActionLockType::Need_Item_C:
				CanLeftActiveItemUnlock(EItemValue::C);

				break;

			case EActionLockType::Need_Item_S:
				CanLeftActiveItemUnlock(EItemValue::S);

				break;

			case EActionLockType::Need_Item_G:
				CanLeftActiveItemUnlock(EItemValue::G);

				break;

			default:

				CanLeftActiveItemUnlock(EItemType::No_Type);

				break;

			}

			switch (CurrentlyHoveredByMouseEncounter_Good->RBTradingRequiredItem)
			{
			case EActionLockType::Need_Item_C:
				CanRightActiveItemUnlock(EItemValue::C);

				break;

			case EActionLockType::Need_Item_S:
				CanRightActiveItemUnlock(EItemValue::S);

				break;

			case EActionLockType::Need_Item_G:
				CanRightActiveItemUnlock(EItemValue::G);

				break;

			default:

				
				CanRightActiveItemUnlock(EItemType::No_Type);

				break;

			}
		}
	}
}

void AWeirdThingsPlayerController::SetCurrentlyHoveredByMouseEncounter_Bad(bool IsHovered, AEncounter_Bad* Encounter_BadToSet)
{
	if (IsHovered)
	{
		CurrentlyHoveredByMouseEncounter_Bad = Encounter_BadToSet;
	}
	else
	{
		CurrentlyHoveredByMouseEncounter_Bad = nullptr;
	}

	if (CurrentlyHoveredByMouseEncounter_Bad) {
		if (!pSelectedCharacter) { return; }
		if (pSelectedCharacter->IsInCombat) {
			CanLeftActiveItemUnlock(EItemType::Weapon);
			CanRightActiveItemUnlock(EItemType::Weapon);
		}
	}

}

void AWeirdThingsPlayerController::SetCurrentlyHoveredByMouseEncounter_Dead(bool IsHovered, AEncounter_Dead* Encounter_DeadToSet)
{
	if (IsHovered)
	{
		CurrentlyHoveredByMouseEncounter_Dead = Encounter_DeadToSet;
	}
	else
	{
		CurrentlyHoveredByMouseEncounter_Dead = nullptr;
	}

	if (CurrentlyHoveredByMouseEncounter_Dead) {
		if (pSelectedCharacter) {

			if (pSelectedCharacter->IsInCombat && CurrentlyHoveredByMouseEncounter_Dead->IsAwake) {
				CanLeftActiveItemUnlock(EItemType::Weapon);
				CanRightActiveItemUnlock(EItemType::Weapon);
			}
		}
	}

}