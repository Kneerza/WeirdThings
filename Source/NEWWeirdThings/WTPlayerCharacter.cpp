// Fill out your copyright notice in the Description page of Project Settings.

#include "WTPlayerCharacter.h"
#include "WeirdThingsPlayerController.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "AttackDefenseComponent.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"

// Sets default values
AWTPlayerCharacter::AWTPlayerCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	//----------------------Making sure Pawn is not possessed----------------
	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	AutoPossessAI = EAutoPossessAI::Disabled;

	//----------------------Creating Root Component--------------------------
	pRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(pRootComponent);

	//----------------------Creating Died Character Sprite Component--------------------------
	DiedCharacterSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Died character sprite"));
	DiedCharacterSprite->SetupAttachment(pRootComponent);
	DiedCharacterSprite->SetHiddenInGame(true);
	DiedCharacterSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//----------------------Creating Selecting Arrow-------------------------
	SelectingArrow = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SelectingArrow"));
	SelectingArrow->AttachTo(pRootComponent);

	FTransform SelectingArrowRelativeTransform;
	SelectingArrowRelativeTransform.SetLocation(FVector(0.f, 0.f, 200.f));
	SelectingArrowRelativeTransform.SetRotation(FRotator(0.f, 90.f, 0.f).Quaternion());
	SelectingArrowRelativeTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

	SelectingArrow->SetRelativeTransform(SelectingArrowRelativeTransform);

	SelectingArrowSprite = LoadObject<UPaperSprite>(nullptr, (TEXT("PaperSprite'/Game/Blueprints/SelectingArrows/Sprite/SelectingArrow_Sprite.SelectingArrow_Sprite'")));
	SelectingArrowForCombatSprite = LoadObject<UPaperSprite>(nullptr, (TEXT("PaperSprite'/Game/Blueprints/SelectingArrows/Sprite/SelectingArrow_Combat_Sprite.SelectingArrow_Combat_Sprite'")));
	SelectingArrowForPickingEnemyToFight = LoadObject<UPaperSprite>(nullptr, (TEXT("PaperSprite'/Game/Blueprints/SelectingArrows/Sprite/SelectingArrow_PickingEnemyForFight_Sprite.SelectingArrow_PickingEnemyForFight_Sprite'")));
	//SelectingArrow->SetSprite(LoadObject<UPaperSprite>(nullptr, (TEXT("PaperSprite'/Game/Blueprints/SelectingArrow_Sprite.SelectingArrow_Sprite'"))));
	SelectingArrow->SetVisibility(false);

	SelectingArrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//----------------------Creating Attack/Defense Component--------------------------
	pAttackDefenseComponent = CreateDefaultSubobject<UAttackDefenseComponent>(TEXT("AttackDefenseComponent"));
	//pAttackDefenseComponent->AttachTo(pRootComponent);

	//-----------------------Initializing arrays--------------------------------
	Injuries.Init(EDurabilityState::Empty, 5);
	Backpack.Init(nullptr, 8);
}

// Called when the game starts or when spawned
void AWTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController());
	//PlayerController->PlayerCharacterRef = this;
	PlayerController->PlayerCharacters.Emplace(this);
	PlayerController->UpdateCharactersFoodInPlay();

	CurrentActionPoints = ActionPoints;

	//CreateAvatar();
}

// Called every frame
void AWTPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWTPlayerCharacter::SetIsSleeping(bool IsCharacterSleeping)
{
	IsSleeping = IsCharacterSleeping;
}

void AWTPlayerCharacter::SetSelected(bool IsSelected)
{
	if (IsSelected) {
		SetSelectedForCombat(false, nullptr, nullptr);
		SetSelectedForPickingEnemy(false);
	}

	if (SelectingArrow) {
		SelectingArrow->SetSprite(SelectingArrowSprite);
		SelectingArrow->SetVisibility(IsSelected);
	}
}

void AWTPlayerCharacter::SetSelectedForCombat(bool IsSelected, AItemTemplate* ItemPicked, AEncounter* CurrentEnemy)
{

	if (IsSelected) {
		SetSelected(false);
		SetSelectedForPickingEnemy(false);
	}

	SelectingArrow->SetSprite(SelectingArrowForCombatSprite);
	SelectingArrow->SetVisibility(IsSelected);

	IsSelectedForCombat = IsSelected;

	if (IsSelected) {
		if (ItemPicked)
		{
			ItemPickedForFight = ItemPicked;
		}
		else {
			ItemPickedForFight = nullptr;
		}
	}
	else {
		ItemPickedForFight = nullptr;
	}

	if (CurrentEnemy)
	{
		CurrentEnemyToAttack = CurrentEnemy;
	}
}

void AWTPlayerCharacter::SetSelectedForPickingEnemy(bool IsSelected)
{
	if (IsSelected) {
		PlayerController->Message[0] = "Choose enemy to attack";
		SetSelected(false);
		SetSelectedForCombat(false, nullptr, nullptr);
	}

	SelectingArrow->SetSprite(SelectingArrowForPickingEnemyToFight);
	SelectingArrow->SetVisibility(IsSelected);
	IsPickingEnemyToFight = IsSelected;
}

// Called to bind functionality to input
void AWTPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWTPlayerCharacter::RefreshItems()
{
	for (int32 i = 0; i < Backpack.Num(); i++)
	{
		if (!Backpack[i]) { continue; }
		if (i ==( (Backpack.Num() - 1)||(Backpack.Num() - 2))) {
			Backpack[i]->IsActive = true;
			//ActiveItem = Backpack[i];
			//pAttackDefenseComponent->AttackPoolRow_1 = ActiveItem->AttackPoolRow_1;
			//pAttackDefenseComponent->DefensePoolRow_1 = ActiveItem->DefensePoolRow_1;
		}
		else if (i < 3) {
			Backpack[i]->IsActive = false;
			pAttackDefenseComponent->DefensePoolRow_1.Append(Backpack[i]->DefensePoolRow_1);
		}
		else {
			Backpack[i]->IsActive = false;
			//pAttackDefenseComponent->AttackPoolRow_1 = pAttackDefenseComponent->AttackPoolRow_Default;
			//pAttackDefenseComponent->DefensePoolRow_1 = pAttackDefenseComponent->DefensePoolRow_Default;
		}
	}
	//if (!ensure(Backpack.Last(0))) { return; }
	//UE_LOG(LogTemp, Warning, TEXT("Active Item: %s"), *Backpack.Last(0)->GetName())

}

void AWTPlayerCharacter::GetItem(AItemTemplate* ItemToPick)
{
	UE_LOG(LogTemp, Warning, TEXT("Getting Item: %s"),*ItemToPick->GetName())

	
	if (!ItemToPick) { return; }
	for (int32 i = 3; i < Backpack.Num(); i++)
	{
		if (Backpack[i]) {}
		else {
			Backpack[i] = ItemToPick;
			RefreshItems();
			break;
		}
	}
}

void AWTPlayerCharacter::GetInjury(int32 InjuryAmountToGet)
{
	if (InjuryAmountToGet == 0) { return; }
	for (int32 i = 0; i < Injuries.Num(); i++)
	{
		if ((Injuries[i] != EDurabilityState::Empty) || (InjuryAmountToGet == 0)) {}
		else {
			Injuries[i] = EDurabilityState::Injury;
			InjuryAmountToGet--;
			//	ItemToPick->Deactivate();
		}
	}
	CheckIfDied();
	/*
	if (this->Injuries.Last() != EDurabilityState::Empty) { 

		//if (PlayerController->PlayerCharacters.Contains(this)) {
		//	PlayerController->PlayerCharacters.Remove(Cast<AWTPlayerCharacter>(this));
		//}

		Die(); 
		return; 
	}*/
}

void AWTPlayerCharacter::GetHunger(int32 HungerAmountToGet)
{
	if (HungerAmountToGet == 0) { return; }
	for (int32 i = 0; i < Injuries.Num(); i++)
	{
		if ((Injuries[i] != EDurabilityState::Empty) || (HungerAmountToGet == 0)) {}
		else {
			Injuries[i] = EDurabilityState::Hunger;
			HungerAmountToGet--;
			//	ItemToPick->Deactivate();
		}
	}

	CheckIfDied();
	//if (this->Injuries.Last() != EDurabilityState::Empty) { this->Destroy(); return; }
}

void AWTPlayerCharacter::GetInsanity(int32 InsanityAmountToGet)
{
	if (InsanityAmountToGet == 0) { return; }
	for (int32 i = 0; i < Injuries.Num(); i++)
	{
		if ((Injuries[i] != EDurabilityState::Empty) || (InsanityAmountToGet == 0)) {}
		else {
			Injuries[i] = EDurabilityState::Insanity;
			InsanityAmountToGet--;
			//	ItemToPick->Deactivate();
		}
	}
	CheckIfDied();
	//if (this->Injuries.Last() != EDurabilityState::Empty) { this->Destroy(); return; }
}

void AWTPlayerCharacter::GetExhaustion(int32 ExhaustionAmountToGet)
{
	if (ExhaustionAmountToGet == 0) { return; }
	for (int32 i = 0; i < Injuries.Num(); i++)
	{
		if ((Injuries[i] != EDurabilityState::Empty) || (ExhaustionAmountToGet == 0)) {}
		else {
			Injuries[i] = EDurabilityState::Exhaustion;
			ExhaustionAmountToGet--;
			//	ItemToPick->Deactivate();
		}
	}
	CheckIfDied();
	//if (this->Injuries.Last() != EDurabilityState::Empty) { this->Destroy(); return; }
}

void AWTPlayerCharacter::GetActionPoints(int32 ActionPointsAmountToGet)
{
	CurrentActionPoints += ActionPointsAmountToGet;
}

void AWTPlayerCharacter::RemoveActionPoints(int32 ActionPointsAmountToRemove)
{
	CurrentActionPoints -= ActionPointsAmountToRemove;
	if (CurrentActionPoints < 0)
	{
		CurrentActionPoints = 0;
	}
}

void AWTPlayerCharacter::RemoveInsanity(int32 InsanityAmountToRemove)
{
	if (InsanityAmountToRemove == 0) { return; }
	for (int32 i = (Injuries.Num() - 1); i >= 0; i--)
	{
		if ((Injuries[i] == EDurabilityState::Insanity && (InsanityAmountToRemove != 0)))
		{
			Injuries[i] = EDurabilityState::Empty;
			InsanityAmountToRemove--;

			SortInjuries();
		}
	}
}

void AWTPlayerCharacter::RemoveHunger(int32 HungerAmountToRemove)
{
	if (HungerAmountToRemove == 0) { return; }

	if (DoesNeedToConsumeFood)
	{
		DoesNeedToConsumeFood = false;
		HungerAmountToRemove--;
		if (HungerAmountToRemove == 0) {
			return;
		}
	}

	for (int32 i = (Injuries.Num() - 1); i >= 0; i--)
	{
		if ((Injuries[i] == EDurabilityState::Hunger && (HungerAmountToRemove != 0)))
		{
			Injuries[i] = EDurabilityState::Empty;
			HungerAmountToRemove--;

			SortInjuries();
		}
	}
}

bool AWTPlayerCharacter::RemoveInjury(int32 InjuryAmountToRemove)
{
	if (InjuryAmountToRemove == 0) { return false; }
	for (int32 i = (Injuries.Num() - 1); i >= 0; i--)
	{
		if ((Injuries[i] == EDurabilityState::Injury && (InjuryAmountToRemove != 0)))
		{
			Injuries[i] = EDurabilityState::Empty;
			InjuryAmountToRemove--;

			SortInjuries();

			return true;
		}
	}

	return false;
}

void AWTPlayerCharacter::RemoveExhaustion(int32 ExhaustionAmountToRemove)
{
	if (ExhaustionAmountToRemove == 0) { return; }

	if (DoesNeedToSleep)
	{
		DoesNeedToSleep = false;
		ExhaustionAmountToRemove--;
		if (ExhaustionAmountToRemove == 0) {
			return;
		}
	}

	for (int32 i = (Injuries.Num() - 1); i >= 0; i--)
	{
		if ((Injuries[i] == EDurabilityState::Exhaustion && (ExhaustionAmountToRemove != 0)))
		{
			Injuries[i] = EDurabilityState::Empty;
			ExhaustionAmountToRemove--;

			SortInjuries();
		}
	}
}

void AWTPlayerCharacter::SortInjuries()
{
	for (int32 i = 0; i < (Injuries.Num() - 1); i++)
	{
		if (Injuries[i] == EDurabilityState::Empty)
		{
			Injuries.Swap(i, (i + 1));
		}
	}

}

void AWTPlayerCharacter::CheckIfDied()
{
	if (this->Injuries.Last() != EDurabilityState::Empty) { this->Die(); return; }
}

UTexture2D* AWTPlayerCharacter::GetCharacterPortrait()
{
	if (!ensure(CharacterPortrait)) { return nullptr; }
	return CharacterPortrait;
}

bool AWTPlayerCharacter::SetHiredCompanion(AActor* CompanionToHire)
{
	if (HiredCompanion) {

		PlayerController->Message[0] = "Alredy have companion";
			return false;
	}
	HiredCompanion = CompanionToHire;
	HiredCompanion->SetActorLocation(GetActorLocation() + FVector(1.f, 30.f, 0.f));
	HiredCompanion->SetActorEnableCollision(ECollisionEnabled::NoCollision);
	return true;
}

void AWTPlayerCharacter::Survive()
{
	if (PlayerController->PlayerCharacters.Contains(this))
	{
		auto CharacterIndex = PlayerController->PlayerCharacters.Find(this);
		PlayerController->PlayerCharacters[CharacterIndex] = nullptr;
		PlayerController->SurvivedCharacters.Add(this);
	}
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	PlayerController->Message[0] = "Character survived";
	IsSurvived = true;

	auto CharactersInPlay = PlayerController->PlayerCharacters;
	int32 NumberOfCharactersInPlay = 0;
	for (int32 i = 0; i < CharactersInPlay.Num(); i++)
	{
		if (CharactersInPlay[i])
		{
			NumberOfCharactersInPlay++;
			//	PlayerController->PlayerCharacters.RemoveSingle(CharactersInPlay[i]);
		}
	}

	if (NumberOfCharactersInPlay < 1)
	{
		if (PlayerController->SurvivedCharacters.IsValidIndex(0))
		{
			PlayerController->IsGameFinished = true;
		}
		else
		{
			PlayerController->IsGameLost = true;
		}
		PlayerController->DisableInput(PlayerController);
	}
}

void AWTPlayerCharacter::Die()
{

	// -------------------------- Change sprite for died character -------------------
	if (DiedCharacterSprite->GetSprite())
	{
		TArray<UPaperSpriteComponent*> SpriteComponents;
		GetComponents<UPaperSpriteComponent>(SpriteComponents);
		for (int32 i = 0; i < SpriteComponents.Num(); i++)
		{
			if (SpriteComponents[i] == DiedCharacterSprite)
			{
				SpriteComponents[i]->SetHiddenInGame(false);
			}
			else
			{
				SpriteComponents[i]->SetHiddenInGame(true);
			}
		}
	}

	if (PlayerController->PlayerCharacters.Contains(this))
	{
		auto CharacterIndex = PlayerController->PlayerCharacters.Find(this);
		PlayerController->PlayerCharacters[CharacterIndex] = nullptr;
		PlayerController->DiedCharacters.Add(this);
	}
	SetActorEnableCollision(false);
	PlayerController->Message[0] = "Character died";
	IsDied = true;
	SelectingArrow->DestroyComponent();


	if (CurrentCombatManager) {
		PlayerController->RefreshCombatManager(CurrentCombatManager);
	}
	/*
	auto CombatManagers = PlayerController->CombatManagersInPlay;
	if (CombatManagers.IsValidIndex(0)) {
		for (int32 i = 0; i < CombatManagers.Num(); i++)
		{
			if (CombatManagers[i])
			{
				CombatManagers[i]->Refresh();
				//	PlayerController->PlayerCharacters.RemoveSingle(CharactersInPlay[i]);
			}
		}
	}*/

	auto CharactersInPlay = PlayerController->PlayerCharacters;
	int32 NumberOfCharactersInPlay = 0;
	for (int32 i = 0; i< CharactersInPlay.Num(); i++)
	{
		if (CharactersInPlay[i])
		{
			NumberOfCharactersInPlay++;
		//	PlayerController->PlayerCharacters.RemoveSingle(CharactersInPlay[i]);
		}
	}

	if (NumberOfCharactersInPlay < 1)
	{
		if (PlayerController->SurvivedCharacters.IsValidIndex(0))
		{
			PlayerController->IsGameFinished = true;
		}
		else
		{
			PlayerController->IsGameLost = true;
		}
		PlayerController->DisableInput(PlayerController);
	}
}
