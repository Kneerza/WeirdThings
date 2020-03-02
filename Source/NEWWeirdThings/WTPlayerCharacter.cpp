// Fill out your copyright notice in the Description page of Project Settings.

#include "WTPlayerCharacter.h"
#include "LocationTemplate.h"
#include "WeirdThingsPlayerController.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
//#include "Runtime/Engine/Classes/Components/MeshComponent.h"
#include "Components/ArrowComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "AttackDefenseComponent.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "ItemTemplate.h"

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

	auto PlayerController = Cast<AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerController->PlayerCharacterRef = this;
	PlayerController->PlayerCharacters.Emplace(this);

	CurrentActionPoints = ActionPoints;

	//CreateAvatar();
}

// Called every frame
void AWTPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWTPlayerCharacter::SetSelected(bool IsSelected)
{
	SelectingArrow->SetSprite(SelectingArrowSprite);
	SelectingArrow->SetVisibility(IsSelected);
}

void AWTPlayerCharacter::SetSelectedForCombat(bool IsSelected)
{
	SelectingArrow->SetSprite(SelectingArrowForCombatSprite);
	SelectingArrow->SetVisibility(IsSelected);
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
		if (i == (Backpack.Num() - 1)) {
			Backpack[i]->IsActive = true;
			ActiveItem = Backpack[i];
			pAttackDefenseComponent->AttackPoolRow_1 = ActiveItem->AttackPoolRow_1;
			//pAttackDefenseComponent->DefensePoolRow_1 = ActiveItem->DefensePoolRow_1;
		}
		else if (i < 3) {
			pAttackDefenseComponent->DefensePoolRow_1 = Backpack[i]->DefensePoolRow_1;
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
	if (this->Injuries.Last() != EDurabilityState::Empty) { this->Destroy(); return; }
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
	if (this->Injuries.Last() != EDurabilityState::Empty) { this->Destroy(); return; }
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
	if (this->Injuries.Last() != EDurabilityState::Empty) { this->Destroy(); return; }
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
	if (this->Injuries.Last() != EDurabilityState::Empty) { this->Destroy(); return; }
}

void AWTPlayerCharacter::GetActionPoints(int32 ActionPointsAmountToGet)
{
	CurrentActionPoints += ActionPointsAmountToGet;
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
		}
	}
}

void AWTPlayerCharacter::RemoveHunger(int32 HungerAmountToRemove)
{
	if (HungerAmountToRemove == 0) { return; }
	for (int32 i = (Injuries.Num() - 1); i >= 0; i--)
	{
		if ((Injuries[i] == EDurabilityState::Hunger && (HungerAmountToRemove != 0)))
		{
			Injuries[i] = EDurabilityState::Empty;
			HungerAmountToRemove--;
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
			return true;
		}
	}
	return false;
}

void AWTPlayerCharacter::RemoveExhaustion(int32 ExhaustionAmountToRemove)
{
	if (ExhaustionAmountToRemove == 0) { return; }
	for (int32 i = (Injuries.Num() - 1); i >= 0; i--)
	{
		if ((Injuries[i] == EDurabilityState::Exhaustion && (ExhaustionAmountToRemove != 0)))
		{
			Injuries[i] = EDurabilityState::Empty;
			ExhaustionAmountToRemove--;
		}
	}
}

UTexture2D* AWTPlayerCharacter::GetCharacterPortrait()
{
	if (!ensure(CharacterPortrait)) { return nullptr; }
	return CharacterPortrait;
}

void AWTPlayerCharacter::CreateAvatar()
{
	if (!AvatarFlipbook) {

		UE_LOG(LogTemp, Error, TEXT("Avatar flipbook is not set"))
			return;
	}

	if (!CurrentLocation) {

		UE_LOG(LogTemp, Error, TEXT("Current Location does not exist"))
			return;
	}
	AvatarComponent = NewObject<UPaperFlipbookComponent>(this, ("Avatar"));

	AvatarComponent->RegisterComponent();

	FCollisionResponseContainer AvatarComponentResponseContainer;
	AvatarComponentResponseContainer.SetAllChannels(ECollisionResponse::ECR_Overlap);
	AvatarComponentResponseContainer.SetResponse(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	AvatarComponentResponseContainer.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	AvatarComponent->SetCollisionResponseToChannels(AvatarComponentResponseContainer);
	AvatarComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

	AvatarComponent->SetFlipbook(AvatarFlipbook);
	AvatarComponent->SetWorldTransform(CurrentLocation->AvailableSocketDynamicPlayerAction[0]->GetComponentTransform());
	//AvatarComponent->SetWorldLocation(CurrentLocation->AvailableSocketDynamicPlayerAction[0]->GetComponentLocation());
	//AvatarComponent->SetWorldRotation(CurrentLocation->AvailableSocketDynamicPlayerAction[0]->GetComponentRotation());

}

void AWTPlayerCharacter::UpdateAvatar()
{
	AvatarComponent->SetWorldTransform(CurrentLocation->AvailableSocketDynamicPlayerAction[0]->GetComponentTransform());
}