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
#include "Timer.h"
#include "Encounter.h"
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

	UpdateGameGoals();

}

//------------------- Click events responses --------------------------------------

void AWeirdThingsPlayerController::LeftClickEvents()
{
	if (AreClickEventsDisabled) { return; }
	GetComponentUnderCursor(pClickedActor, ClickedActorClassName);

	if (!pClickedActor) { return; }

	if (ClickedActorClassName == "WTPlayerCharacter") 
	{

		SelectCharacter(pClickedActor);
	}
	else if (ClickedActorClassName == "Encounter_Bad")
	{

		Encounter_BadLeftClickResponse();
	}
	else if (ClickedActorClassName == "Encounter_Good")
	{

		Encounter_GoodLeftClickResponse();
	}
	else if (ClickedActorClassName == "Encounter_Dead") {

		Encounter_DeadLeftClickResponse();
	}
	else if (ClickedActorClassName == "Action") {

		ActionLeftClickResponse();
	}
	
	else if (bIsCharacterPickingToFight)
	{
			CharacterPickingToFight->SetSelectedForPickingEnemy(false);
			CharacterPickingToFight = nullptr;
			bIsCharacterPickingToFight = false;

	}else if (bIsCombatOn) {

	}
}

void AWeirdThingsPlayerController::RightClickEvents()
{

	if (AreClickEventsDisabled) { return; }
	GetComponentUnderCursor(pClickedActor, ClickedActorClassName);

	if (!pClickedActor) { return; }

	if (ClickedActorClassName == "WTPlayerCharacter") {

		DeselectCharacter(pClickedActor);
	}
	else if (ClickedActorClassName == "Encounter_Bad") {

		Encounter_BadRightClickResponse();
	}
	else if (ClickedActorClassName == "Encounter_Good")
	{
		Encounter_GoodRightClickResponse();
	}
	else if (ClickedActorClassName == "Encounter_Dead") {

		Encounter_DeadRightClickResponse();
	}
	else if (ClickedActorClassName == "Action") {

		ActionRightClickResponse();
	}
	else if (ClickedActorClassName == "LocationTemplate")
	{

		LocationRightClickResponse();
	}
	else if (ClickedActorClassName == "InteractiveLocationDecoration")
	{

		InteractiveLocationDecorationRightClickResponse();
	}
	else if (bIsCharacterPickingToFight)
	{
			CharacterPickingToFight->SetSelectedForPickingEnemy(false);
			CharacterPickingToFight = nullptr;
			bIsCharacterPickingToFight = false;

	}if (bIsCombatOn) {

	}else if (CharacterIsSelected) {
	
	}

}

void AWeirdThingsPlayerController::Encounter_BadRightClickResponse()
{
	if (pSelectedCharacter)
	{
		if (pSelectedCharacter->IsPickingEnemyToFight)
		{
			pSelectedCharacter->SetSelectedForCombat(true, pSelectedCharacter->RightActiveItem, Cast<AEncounter>(pClickedActor));
			//pSelectedCharacter->CurrentEnemyToAttack = Cast<AEncounter>(pClickedActor);
			pSelectedCharacter = nullptr;
			//AddToPlayersChoosenForFight(pSelectedCharacter, Cast<AEncounter>(pClickedActor), pSelectedCharacter->RightActiveItem);
		}
		else if (!(pSelectedCharacter->IsInCombat))
		{
		//	InitiateCombat(Cast<AEncounter>(pClickedActor));
		}
	}
}

void AWeirdThingsPlayerController::Encounter_BadLeftClickResponse()
{
	if (pSelectedCharacter)
	{
		if (pSelectedCharacter->IsPickingEnemyToFight)
		{
			//AddToPlayersChoosenForFight(pSelectedCharacter, Cast<AEncounter>(pClickedActor), pSelectedCharacter->LeftActiveItem);
			pSelectedCharacter->SetSelectedForCombat(true, pSelectedCharacter->LeftActiveItem, Cast<AEncounter>(pClickedActor));
			//pSelectedCharacter->CurrentEnemyToAttack = Cast<AEncounter>(pClickedActor);
			pSelectedCharacter = nullptr;
			
		}
	}
}

void AWeirdThingsPlayerController::Encounter_GoodRightClickResponse()
{
	if (bIsCharacterPickingToFight)
	{
		if (!CharacterPickingToFight) { return; }

		//AddToPlayersChoosenForFight(CharacterPickingToFight, Cast<AEncounter>(pClickedActor), CharacterPickingToFight->RightActiveItem);
		bIsCharacterPickingToFight = false;
	}
	else if (bIsCombatOn) {

	}
	else if (CharacterIsSelected) {
		if (CurrentlyHoveredByMouseEncounter_Good)
		{
			if (Trade(CurrentlyHoveredByMouseEncounter_Good->RBTradingAction, CurrentlyHoveredByMouseEncounter_Good->RBTradingRequiredItem))
			{
				CurrentlyHoveredByMouseEncounter_Good->RBTradingAction = EActionType::No_Action;
				CurrentlyHoveredByMouseEncounter_Good->RBTradingRequiredItem = EActionLockType::No_Need;
			}
		}
	}

}

void AWeirdThingsPlayerController::Encounter_GoodLeftClickResponse()
{
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
			if (Trade(CurrentlyHoveredByMouseEncounter_Good->LBTradingAction, CurrentlyHoveredByMouseEncounter_Good->LBTradingRequiredItem))
			{
				CurrentlyHoveredByMouseEncounter_Good->LBTradingAction = EActionType::No_Action;
				CurrentlyHoveredByMouseEncounter_Good->LBTradingRequiredItem = EActionLockType::No_Need;
			}
		}
	}
}

void AWeirdThingsPlayerController::Encounter_DeadRightClickResponse()
{
	if (bIsCharacterPickingToFight)
	{
		if (!CharacterPickingToFight) { return; }

		//AddToPlayersChoosenForFight(CharacterPickingToFight, Cast<AEncounter>(pClickedActor), CharacterPickingToFight->RightActiveItem);
		bIsCharacterPickingToFight = false;
	}
	else if (bIsCombatOn) {

	}
	else if (CharacterIsSelected) {

		if (Cast<AEncounter_Dead>(pClickedActor)->IsAwake) {
			//InitiateCombat(Cast<AEncounter>(pClickedActor));
		}
	}
	else {

	}
}

void AWeirdThingsPlayerController::Encounter_DeadLeftClickResponse()
{
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
}

void AWeirdThingsPlayerController::ActionLeftClickResponse()
{
	if (bIsCharacterPickingToFight)
	{

	}
	else if (bIsCombatOn) {

	}
	else {
		if (pSelectedCharacter->LeftActiveItem)
		{
			Cast<AAction>(pClickedActor)->Unlock();
			pSelectedCharacter->LeftActiveItem->Destroy();
			pSelectedCharacter->LeftActiveItem = nullptr;
			pSelectedCharacter->RefreshItems();
			pSelectedCharacter->Backpack[pSelectedCharacter->Backpack.Num() - 1] = nullptr;
		}
	}
}

void AWeirdThingsPlayerController::ActionRightClickResponse()
{
	if (bIsCharacterPickingToFight)
	{

	}
	else if (bIsCombatOn) {

	}
	else if (pSelectedCharacter) {
		auto CurrentAction = Cast<AAction>(pClickedActor);
		if (CurrentAction->ActionLock[CurrentAction->CurrentLockIndex]) {
			if (pSelectedCharacter->RightActiveItem)
			{
				CurrentAction->Unlock();
				pSelectedCharacter->RightActiveItem->Destroy();
				pSelectedCharacter->RightActiveItem = nullptr;
				pSelectedCharacter->Backpack[pSelectedCharacter->Backpack.Num() - 2] = nullptr;
			}
		}
		else {
			ClickedActionHandle(CurrentAction);
		}
	}
}

void AWeirdThingsPlayerController::LocationRightClickResponse()
{
//	if (bIsCharacterPickingToFight)
//	{

//	}
	//else if (bIsCombatOn) {

//	}
	//else 
	if (pSelectedCharacter) {

		if (pSelectedCharacter->IsInCombat) { return; }

		auto ClickedLocation = Cast<ALocationTemplate>(pClickedActor);
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

void AWeirdThingsPlayerController::InteractiveLocationDecorationRightClickResponse()
{

	if (pSelectedCharacter) {
		if (!(pSelectedCharacter->IsInCombat)) {
			ClickedActionHandle(Cast<AInteractiveLocationDecoration>(pClickedActor)->EntangledAction);
		}
	}

}

void AWeirdThingsPlayerController::ClickedActionHandle(AAction* CurrentAction)
{
	if (!CurrentAction) { return; }

	if (pSelectedCharacter->CurrentLocation != CurrentAction->GetParentActor()) { return; }

	auto ActionPointsRequired = CurrentAction->ActionPointsRequired;
	if (pSelectedCharacter->CurrentActionPoints < ActionPointsRequired)
	{
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
			
			Cast<AInteractiveLocationDecoration>(CurrentAction->EntangledInteractiveLocationDecoration)->ChangeState();
				
		}
	}
	else {
		CurrentAction->GetTypeOfLock();
		TryToUnlock(CurrentAction);
	}
}

void AWeirdThingsPlayerController::SelectCharacter(AActor* CharacterToSelectActor)
{
	if (!ensure(CharacterToSelectActor)) { return; }

	auto CharacterToSelect = Cast<AWTPlayerCharacter>(CharacterToSelectActor);
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
	if (!ensure(CharacterToDeselectActor)) { return; }

	auto CharacterToDeselect = Cast<AWTPlayerCharacter>(CharacterToDeselectActor);
	CharacterToDeselect->SetSelected(false);
	CharacterToDeselect->SetSelectedForCombat(false, nullptr, nullptr);
	CharacterToDeselect->SetSelectedForPickingEnemy(false);
	if (PlayersChosenToFight.Contains(CharacterToDeselect))
	{
		int32 i = 0;
		PlayersChosenToFight.Find(CharacterToDeselect, i);
		PlayersChosenToFight[i] = nullptr;
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




//------------------------------- Movements -----------------------------------------------------------------------

void AWeirdThingsPlayerController::MoveCharacter(AWTPlayerCharacter* CharacterToMove, ALocationTemplate* LocationToMoveTo)
{
	if (!LocationToMoveTo) { return; }
	if (CharacterToMove->MovementPoints < 1) { return; }

	if (!ensure(LocationToMoveTo)) { return; }
	//auto LocationOfLocationTemplate = LocationToMoveTo->GetActorLocation();
	auto LocationOfCharacter = Cast<ALocationTemplate>(CharacterToMove->CurrentLocation);

	// Check if another location is too far 
	if (abs(LocationToMoveTo->HorizontalIndex - LocationOfCharacter->HorizontalIndex) > 1) { return; }
	if ((abs(LocationToMoveTo->VerticalIndex - LocationOfCharacter->VerticalIndex) == 1) && (LocationToMoveTo->HorizontalIndex != 0)) { return; }
	if (abs(LocationToMoveTo->VerticalIndex - LocationOfCharacter->VerticalIndex) > 1) { return; }
	if ((LocationToMoveTo->HorizontalIndex != LocationOfCharacter->HorizontalIndex) && (LocationToMoveTo->VerticalIndex != LocationOfCharacter->VerticalIndex)) { return; }

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

	for (int32 i = 0; i < CombatManagersInPlay.Num(); i++)
	{
		if (CombatManagersInPlay[i])
		{
			if (CombatManagersInPlay[i]->CurrentLocation == LocationToMoveTo)
			{
				CombatManagersInPlay[i]->CharactersInCombat.Add(CharacterToMove);

				DeselectCharacter(CharacterToMove);
				CharacterToMove->IsInCombat = true;
			}
		}
	}

	CharacterToMove->MovementPoints--;
	CharacterToMove->UpdateAvatar();
	
}

void AWeirdThingsPlayerController::TeleportCharacter(AWTPlayerCharacter* CharacterToMove, AActor* LocationToMoveToActor)
{

	auto LocationToMoveTo = Cast<ALocationTemplate>(LocationToMoveToActor);

	if (!LocationToMoveTo) { return; }
	if (CharacterToMove->MovementPoints < 1) { return; }

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
		Cast<AInteractiveLocationDecoration>(ForcedAction->EntangledInteractiveLocationDecoration)->ChangeState();
	}
	CharacterToMove->MovementPoints--;
	CharacterToMove->UpdateAvatar();
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

void AWeirdThingsPlayerController::Combat()
{
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
}

void AWeirdThingsPlayerController::InitiateCombat()
{
	if (pSelectedCharacter) {
	auto SpawnedCombatManager =	SpawnCombatManager(Cast<ALocationTemplate>(pSelectedCharacter->CurrentLocation), pSelectedCharacter);

	UE_LOG(LogTemp, Error, TEXT("Characters in combat:"))
	SpawnedCombatManager->ShowCharactersInCombat();
	}

	bIsCombatOn = true;
	if (pSelectedCharacter) { 
		pSelectedCharacter->SetSelected(false); 
		pSelectedCharacter = nullptr;
	}

	UE_LOG(LogTemp, Error, TEXT("Combat initiated"))
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
	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{

		if (PlayerCharacters[i]->CurrentLocation == Encounter_Dead->CurrentLocation)
		{
			//AttackDefenseEvent(Encounter_Dead, PlayerCharacters[i]);

			return;
		}

	}
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
				ItemDurabilityCheck(ItemOwner, ItemToUse, EItemType::Medicine);
			}
			break;

		case EItemType::Tonic:

			ItemOwner->GetActionPoints(1);
			ItemDurabilityCheck(ItemOwner, ItemToUse, EItemType::Tonic);

			break;

		default:

			break;
			
		}
	}
	ItemOwner->CurrentActionPoints -= ItemToUse->ActionPointsRequiredToUse;
}

void AWeirdThingsPlayerController::PassItemToPlayer(EItemValue ItemValue)
{
	auto ItemToPickClass = DeckManager->DrawItemFromDeck(ItemValue);

	if (!ItemToPickClass) { return; }

	AItemTemplate* ItemToPick = GetWorld()->SpawnActor<AItemTemplate>(ItemToPickClass);
	pSelectedCharacter->GetItem(ItemToPick);
}

void AWeirdThingsPlayerController::PassItemToPlayer(AItemTemplate* ItemsToPick)
{
	if (!ItemsToPick) { return; }

	pSelectedCharacter->GetItem(ItemsToPick);
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

ACombatManager* AWeirdThingsPlayerController::SpawnCombatManager(ALocationTemplate* CurrentLocationOfCombat, AActor* CombatInstigator)
{
	if (!CombatManagerClassToSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatManagerClassToSpawn wasn't set in Player Controller"))
			return nullptr;
	}
	auto SpawnedCombatManager = GetWorld()->SpawnActor<ACombatManager>(CombatManagerClassToSpawn);
	SpawnedCombatManager->CurrentLocation = CurrentLocationOfCombat;
	SpawnedCombatManager->CombatInstigator = CombatInstigator;

	
	auto &CharactersInCombat = SpawnedCombatManager->CharactersInCombat;
	auto &EncountersInCombat = SpawnedCombatManager->EncountersInCombat;

	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		if (PlayerCharacters[i])
		{
			if (PlayerCharacters[i]->CurrentLocation == SpawnedCombatManager->CurrentLocation)
			{
				CharactersInCombat.Add(PlayerCharacters[i]);
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

	UE_LOG(LogTemp, Warning, TEXT("Performing action: %s"), *Action->GetName())
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
			ConsumeWood(1, pSelectedCharacter, 1);
			if (Action->EntangledDeadEncounter)
			{
				auto EntangledDeadEncounter = Cast<AEncounter_Dead>(Action->EntangledDeadEncounter);

				FTransform EffectTransform;
				EffectTransform.SetLocation(EntangledDeadEncounter->GetActorLocation());
				Encounter_DeadsInPlay.RemoveSingle(EntangledDeadEncounter);
				Action->EntangledDeadEncounter->Destroy();
				if (!(EntangledDeadEncounter->IsOnPlot)) {
					EntangledDeadEncounter->CreatedAction->UnregisterComponent();
				}
				if (BurningEffectClass) {

					GetWorld()->SpawnActor<AActor>(BurningEffectClass, EffectTransform);
				}
			}
			else //if (Action->GetParentActor()->GetClass()->GetSuperClass()->GetName() == "Encounter_Dead") {
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
		if (ConsumeFood(1, pSelectedCharacter, 1)) {
			CurrentAction->Unlock();
		}
		break;

	case EActionLockType::Need_Wood:
		if (ConsumeWood(1, pSelectedCharacter, 1)) {
			CurrentAction->Unlock();
		}
		break;

	case EActionLockType::Need_Tool:

		CanLeftActiveItemUnlock(EItemType::Tool);
		CanRightActiveItemUnlock(EItemType::Tool);

		//if (FindAndUseItemToUnlock(EItemType::Tool)) {
		//	CurrentAction->Unlock();
		//}

		break;

	case EActionLockType::Need_Axe:

		CanLeftActiveItemUnlock(EItemType::Axe);
		CanRightActiveItemUnlock(EItemType::Axe);

		//if (FindAndUseItemToUnlock(EItemType::Axe)) {
		//	CurrentAction->Unlock();
		//}

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

	case EActionLockType::Need_Shovel:

		CanLeftActiveItemUnlock(EItemType::Shovel);
		CanRightActiveItemUnlock(EItemType::Shovel);

		//if (FindAndUseItemToUnlock(EItemType::Shovel)) {
		//	CurrentAction->Unlock();
		//}

		break;

	default:

		CanLeftActiveItemUnlock(EItemType::No_Type);
		CanRightActiveItemUnlock(EItemType::No_Type);

		break;
	}
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
				if (ItemType[i] == BackpackItemType)
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
				if (ItemType[i] == BackpackItemType)
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

	Door = NewObject<UChildActorComponent>(this, ("Door"));

	Door->RegisterComponent();
	Door->SetChildActorClass(DoorToCreateClass);
	Door->SetWorldLocation(LocationWithDoorCreated->SocketDoor->GetComponentLocation());
	LocationWithDoorCreated->CreateDynamicAction(TeleportActionToCreateClass, Door, LocationInstigator);

}

void AWeirdThingsPlayerController::SetForcedActionForLocation(AActor* LocationToSetForcedActionOn, AActor* ActionToSetAsForced)
{
	Cast<ALocationTemplate>(LocationToSetForcedActionOn)->ForcedAction = Cast<AAction>(ActionToSetAsForced);
}

void AWeirdThingsPlayerController::CreateDynamicAction(AActor* CurrentLocationActor, TSubclassOf<AAction> ActionClass, AEncounter_Dead* EntangledDead)
{
	UE_LOG(LogTemp, Warning, TEXT("Inside CreateDynemicAction function "))

		auto CurrentLocation = Cast<ALocationTemplate>(CurrentLocationActor);
	auto DynamicAction = CurrentLocation->DynamicAction;

	for (int32 i = 0; i < DynamicAction.Num(); i++)
	{
		if (DynamicAction[i]) {
			continue;
		}
		DynamicAction[i] = NewObject<UChildActorComponent>(CurrentLocation, ("Action_Dead" + i));

		DynamicAction[i]->RegisterComponent();
		DynamicAction[i]->SetChildActorClass(ActionClass);
		DynamicAction[i]->SetWorldLocation(CurrentLocation->AvailableSocketDynamicAction[0]->GetComponentLocation());
		Cast<AAction>(DynamicAction[i]->GetChildActor())->EntangledDeadEncounter = EntangledDead;

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

bool AWeirdThingsPlayerController::Trade(EActionType ResultOfTrading, EActionLockType ItemRequiredToTrade)
{
	auto WasRequiredItemConsumed = false;
	if (!pSelectedCharacter) { return false; }

	switch (ItemRequiredToTrade)
	{
	case EActionLockType::Need_Food:
		if (RemoveFood(1, pSelectedCharacter, 0)) {
			WasRequiredItemConsumed = true;
		}
		break;

	case EActionLockType::Need_Wood:
		if (ConsumeWood(1, pSelectedCharacter, 0)) {
			WasRequiredItemConsumed = true;
		}
		break;

	default:

		break;
	}

	if (WasRequiredItemConsumed)
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
				if (!(pSelectedCharacter->RemoveInjury(1))) {
					UE_LOG(LogTemp, Error, TEXT("No injuries can be removed"))
						return false;
				}
			}
			break;
	
		default:


			break;
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

	return LocationWithSocket->AvailableSocketDynamicPlayerAction[0]->GetComponentTransform();

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

	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		if (PlayerCharacters[i]->DoesNeedToConsumeFood) {
			PlayerCharacters[i]->GetHunger(1);
		}

		if (PlayerCharacters[i]->DoesNeedToSleep) {
			PlayerCharacters[i]->GetExhaustion(1);
		}

		if (PlayerCharacters[i]->DoesNeedFire) {
			PlayerCharacters[i]->DoesNeedFire = false;
		}
	}

	for (int32 i = 0; i < Encounter_DeadsInPlay.Num(); i++)
	{
		Encounter_DeadsInPlay[i]->SetAwakened(false);
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
		if (PlayerCharacters[i]->DoesNeedToConsumeFood) {
			PlayerCharacters[i]->GetHunger(1);
		}

		if (PlayerCharacters[i]->DoesNeedToSleep) {
			PlayerCharacters[i]->GetExhaustion(1);
		}

		if (PlayerCharacters[i]->DoesNeedFire) {
			PlayerCharacters[i]->DoesNeedFire = false;
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
	for (int32 i = 0; i < Encounter_DeadsInPlay.Num(); i++)
	{
		Encounter_DeadsInPlay[i]->SetAwakened(true);
	}

	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		if (PlayerCharacters[i]->DoesNeedToConsumeFood) {
			PlayerCharacters[i]->GetHunger(1);
		}
		PlayerCharacters[i]->DoesNeedToConsumeFood = true;

		if (PlayerCharacters[i]->DoesNeedToSleep) {
			PlayerCharacters[i]->GetExhaustion(1);
		}

		if (!(Cast<ALocationTemplate>(PlayerCharacters[i]->CurrentLocation)->AvailableSocketCampFire.Num() < 1))
		{
			PlayerCharacters[i]->DoesNeedFire = true;
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
		if (PlayerCharacters[i]->DoesNeedToConsumeFood) {
			PlayerCharacters[i]->GetHunger(1);
		}

		if (PlayerCharacters[i]->DoesNeedToSleep) {
			PlayerCharacters[i]->GetExhaustion(1);
		}

		PlayerCharacters[i]->DoesNeedToSleep = true;

		if (!(Cast<ALocationTemplate>(PlayerCharacters[i]->CurrentLocation)->AvailableSocketCampFire.Num() < 1)) {
			PlayerCharacters[i]->GetInsanity(1);

			PlayerCharacters[i]->DoesNeedFire = false;
		}
	}

	//TODO collapse to function
	for (int32 i = 0; i < Encounter_DeadsInPlay.Num(); i++)
	{
		if (Encounter_DeadsInPlay[i]) {
			Move_Encounter_Dead(Encounter_DeadsInPlay[i]);
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

bool AWeirdThingsPlayerController::ConsumeFood(int32 FoodAmountToConsume, AWTPlayerCharacter* AffectedCharacter, int32 ActionPointsRequired) //TODO make applicable regardless selected character or not
{
	if (!AffectedCharacter) { return false; }

	if (CurrentTimeOfDay == ETimeOfDay::Evening) {

	}
	else if (AffectedCharacter->CurrentActionPoints < ActionPointsRequired)
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

	if (!(CurrentTimeOfDay == ETimeOfDay::Evening)) {
		AffectedCharacter->CurrentActionPoints -= ActionPointsRequired;
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

	AffectedCharacter->CurrentActionPoints -= ActionPointsRequired;

	return true;
}

bool AWeirdThingsPlayerController::ConsumeWood(int32 WoodAmountToConsume, AWTPlayerCharacter* AffectedCharacter, int32 ActionPointsRequired) //TODO make applicable regardless selected character or not
{
	if (!AffectedCharacter) { return false; }

	if (AffectedCharacter->CurrentActionPoints < ActionPointsRequired)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough AP"))
			return false;
	}

	if ((AffectedCharacter->Wood - WoodAmountToConsume) < 0)
	{
		// TODO "Not enough wood"
		return false;
	}
	AffectedCharacter->Wood -= WoodAmountToConsume;
	AffectedCharacter->CurrentActionPoints -= ActionPointsRequired;
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

	 int32 ActionPointsRequired = 1;

	 if ((CurrentTimeOfDay == ETimeOfDay::Evening)) {
		 ActionPointsRequired = 0;
	 }
	 else if (PlayerCharacter->CurrentActionPoints < ActionPointsRequired)
	 {

		 UE_LOG(LogTemp, Warning, TEXT("Not enough AP"))
			 return;
	 }

	auto AvailableCampFireSocket = Cast<ALocationTemplate>(PlayerCharacter->CurrentLocation)->AvailableSocketCampFire[0];
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
	if (ConsumeWood(1, PlayerCharacter, ActionPointsRequired))
	{
		GetWorld()->SpawnActor<AActor>(CampFireClassToSpawn, SpawningSocketTransform, SpawnParameters);

		for (int32 i = 0; i < PlayerCharacters.Num(); i++)
		{
			if (PlayerCharacters[i]->DoesNeedFire) {
				PlayerCharacters[i]->DoesNeedFire = false;
			}
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No wood available"))
	}

	//PlayerCharacter->CurrentActionPoints--;
}

void AWeirdThingsPlayerController::Sleep(AWTPlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter->CurrentActionPoints < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough AP"))
			return;
	}

	PlayerCharacter->RemoveExhaustion(1);

		PlayerCharacter->CurrentActionPoints = 0;
		PlayerCharacter->MovementPoints = 0;
}

void AWeirdThingsPlayerController::AddActionPointToEveryCharacter()
{
	for (int32 i = 0; i < PlayerCharacters.Num(); i++)
	{
		PlayerCharacters[i]->CurrentActionPoints = PlayerCharacters[i]->ActionPoints;
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
		TryToUnlock(CurrentlyHoveredByMouseAction);
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
		if (bIsCombatOn) {
			CanLeftActiveItemUnlock(EItemType::Weapon);
			CanRightActiveItemUnlock(EItemType::Weapon);
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
		if (bIsCombatOn) {
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
		if (bIsCombatOn && CurrentlyHoveredByMouseEncounter_Dead->IsAwake) {
			CanLeftActiveItemUnlock(EItemType::Weapon);
			CanRightActiveItemUnlock(EItemType::Weapon);
		}
	}

}