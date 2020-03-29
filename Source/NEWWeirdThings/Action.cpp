// Fill out your copyright notice in the Description page of Project Settings.
#include "Action.h"
#include "WeirdThingsPlayerController.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PaperSpriteComponent.h"
#include <math.h>
#include "Runtime/Core/Public/Containers/UnrealString.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Runtime/Core/Public/Math/UnrealMathVectorConstants.h"



#define PI 3.14159265

// Sets default values
AAction::AAction()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreateComponents();

	//-----------------------Initializing arrays-----------------------------
	ActionLock.Init(nullptr, 8);
	ActionLockType.Init(EActionLockType::No_Need, 8);

}

// Called when the game starts or when spawned
void AAction::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController());

	if (IsInfinite) {
		StartingActionLockType = ActionLockType;
	}

	ConstructActionLocks();
	ConstructModifierVisual();
	SetUpActionAsForced();

}

// Called every frame
void AAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAction::SetEntangledDeadEncounter(AActor* EntangledDeadEncounterToSet)
{
	EntangledDeadEncounter = EntangledDeadEncounterToSet;
}

void AAction::ConstructActionLocks()
{
	for (int32 i = 0; i < ActionLockType.Num(); i++)
	{
		if (ActionLockType[i] == EActionLockType::No_Need) { 
			
			if (ActionLock[0])
			{
				IsLocked = true;
			}

			return; }

		ActionLock[i] = NewObject<UPaperFlipbookComponent>(this, ("Lock    " + i));
		ActionLock[i]->RegisterComponent();
		switch (ActionLockType[i])
		{
		case EActionLockType::Need_Food:

			ActionLock[i]->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Food_Flipbook.Food_Flipbook'"))));
			break;

		case EActionLockType::Need_Wood:

			ActionLock[i]->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Wood_Flipbook.Wood_Flipbook'"))));
			break;

		case EActionLockType::Need_Tool:

			ActionLock[i]->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Tool_Flipbook.Tool_Flipbook'"))));
			break;

		case EActionLockType::Need_Axe:

			ActionLock[i]->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Axe_Flipbook.Axe_Flipbook'"))));
			break;

		case EActionLockType::Need_Exhaustion:

			ActionLock[i]->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Exhaustion_Flipbook.Exhaustion_Flipbook'"))));
			break;

		case EActionLockType::Need_Insanity:

			ActionLock[i]->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Insanity_Flipbook.Insanity_Flipbook'"))));
			break;

		case EActionLockType::Need_Item_C:

			ActionLock[i]->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Lock_Item_Copper_Flipbook.Lock_Item_Copper_Flipbook'"))));
			break;

		case EActionLockType::Need_Item_S:

			ActionLock[i]->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Lock_Item_Silver_Flipbook.Lock_Item_Silver_Flipbook'"))));
			break;

		case EActionLockType::Need_Item_G:

			ActionLock[i]->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Lock_Item_Gold_Flipbook.Lock_Item_Gold_Flipbook'"))));
			break;

		case EActionLockType::Need_Shovel:

			ActionLock[i]->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/ActionLock_Shovel_Flipbook.ActionLock_Shovel_Flipbook'"))));
			break;

		default:

			break;
		}


		FTransform TransformLock = ActionFlipBookComponent->GetComponentTransform();
		TransformLock.SetLocation(TransformLock.GetLocation() += (FVector(ActionLockOffset_X, ActionLocksOrbit*FMath::Sin(i*45.f*PI / 180.f), ActionLocksOrbit*FMath::Cos(i*45.f*PI / 180.f))));

		TransformLock.SetScale3D(FVector(2.5f, 2.5f, 2.5f));

		ActionLock[i]->SetRelativeTransform(TransformLock);
	}
	
	if (ActionLock[0])
	{
		IsLocked = true;
	}
}

void AAction::ConstructModifierVisual()
{
	// TODO Collapse component registration and transform setting into function
	if (Modifier == 1) { return; }
		ModifierVisual = NewObject<UPaperFlipbookComponent>(this, ("Modifier"));
		ModifierVisual->RegisterComponent();
		switch (Modifier)
		{
		case 2:

				ModifierVisual->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Flipbooks/Modifier_2_Flipbook.Modifier_2_Flipbook'"))));
			break;

		case 3:

			ModifierVisual->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Flipbooks/Modifier_3_Flipbook.Modifier_3_Flipbook'"))));
			break;

		case 4:

			ModifierVisual->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Flipbooks/Modifier_4_Flipbook.Modifier_4_Flipbook'"))));
			break;

		default:
			return;

			break;

		}

		FTransform TransformModifierVisual = ActionFlipBookComponent->GetComponentTransform();
		TransformModifierVisual.SetLocation(TransformModifierVisual.GetLocation()+ (FVector(-5.f, 80.f, -80.f))); // TODO get rid of magic number

		ModifierVisual->SetRelativeTransform(TransformModifierVisual);
	
}

void AAction::GetTypeOfLock()
{
	for (int32 i = 0; i < ActionLock.Num(); i++)
	{
		if (ActionLock[i])
		{
			CurrentLockIndex = i;
			CurrentLockTypeIndex = i;
			IsLocked = true;
			UE_LOG(LogTemp, Error, TEXT("Current lock index: %i"),i)
			return; 
		}

	}
	IsLocked = false;
	return;
}

void AAction::Unlock()
{
	UE_LOG(LogTemp, Error, TEXT("Inside Unlock in action"))
	if (!ActionLock[CurrentLockIndex]) { return; }
	ActionLockType[CurrentLockTypeIndex] = EActionLockType::No_Need;
	ActionLock[CurrentLockIndex]->UnregisterComponent();
	ActionLock[CurrentLockIndex] = nullptr;
	GetTypeOfLock();

	if (IsLocked) {
		UE_LOG(LogTemp, Error, TEXT("Action is still locked"))
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Action is unlocked"))
	}
}

void AAction::Deactivate()
{
	UE_LOG(LogTemp, Error, TEXT("Here"))
	// --- Deactivating arrow actions ---

	if (ActionType == EActionType::PickUpItem) {
		Destroy();
		return;
	}

	if ((IsWorkedOut)&&((ActionType == EActionType::ArrowRight_Bad) || (ActionType == EActionType::ArrowRight_Good) || (ActionType == EActionType::ArrowRight_Ugly) || (ActionType == EActionType::ArrowUp_Plot) || (ActionType == EActionType::Teleport)))
	{
		
		UpdateArrowActionVisual();
	}
	if (IsInfinite){}
	else if (!((ActionType == EActionType::Arrow_Move) && (!FirstActionInChain))){
		// --- Changing color on deactivated Action ---
		
		ActionFlipBookComponent->SetSpriteColor(FLinearColor(0.03f, 0.03f, 0.03f, 1));
		if (pActionForcedComponent) {
			pActionForcedComponent->SetSpriteColor(FLinearColor(0.03f, 0.03f, 0.03f, 1));
		}
		
		// --- Disabling Collision on deactivated Action ---
		ActionFlipBookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PlayerController->DeactivateEntangledILD(this);
		//CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//if (EntangledInteractiveLocationDecoration) {
		//	EntangledInteractiveLocationDecoration->InteractiveLocationDecoration_SpriteComponent_0->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//}
	}
	
	// --- Changing color of connector and activating ChildAction (if there are any) 
	if (Child && pConnector)
	{
		pConnector->SetSpriteColor(FLinearColor(0.03f, 0.03f, 0.03f, 1));
		Child->Activate();
	}

	

	if (FirstActionInChain)
	{
		FirstActionInChain->Activate();
	}
	else if (IsInfinite)
	{
		ActionLockType = StartingActionLockType;
		ConstructActionLocks();
	}

	if (ModifierVisual)
	{
		ModifierVisual->SetSpriteColor(FLinearColor(0.03f, 0.03f, 0.03f, 1));
	}

	IsDeactivated = true;

	if (Nexus) {
		Nexus->Activate();
		if (pConnector)
		{
			pConnector->SetSpriteColor(FLinearColor(0.03f, 0.03f, 0.03f, 1));
		}
	}
}

void AAction::Activate()
{
	if (ChildrenOfNexus.Num() > 0) {
		for (int32 i = 0; i < ChildrenOfNexus.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Checking if nexus should be activated"))
			if (ChildrenOfNexus[i]) {
				if(!(ChildrenOfNexus[i]->IsDeactivated)){ 
					UE_LOG(LogTemp, Warning, TEXT("Active child of Nexus: %s"),*ChildrenOfNexus[i]->GetName())
					return; 
				}
			}
		}
	}
	// --- Setting color of activated Action ---
	ActionFlipBookComponent->SetSpriteColor(FLinearColor(1.f, 1.f, 1.f, 1.f));

	// --- Setting collision of activated Action ---
	ActionFlipBookComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (EntangledInteractiveLocationDecoration) {
		//EntangledInteractiveLocationDecoration->InteractiveLocationDecoration_SpriteComponent_0->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PlayerController->ActivateEntangledILD(this);
	}

	if (Child && pConnector)
	{
		pConnector->SetSpriteColor(FLinearColor(1.f, 1.f, 1.f, 1));
	}
	IsDeactivated = false;
}

void AAction::SetUpActionAsForced()
{
	if (IsForced)
	{
		pActionForcedComponent = NewObject<UPaperFlipbookComponent>(this, "ForcedActionFlipbook");
		pActionForcedComponent->RegisterComponent();
		pActionForcedComponent->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Flipbooks/Action_Forced_Flipbook.Action_Forced_Flipbook'"))));

		FTransform ForcedActionTransform = ActionFlipBookComponent->GetComponentTransform();

		// Making sure ForcedActionFlipbook does not block ActionFlipbook
		FVector Offset = FVector(1.f, 0.f, 0.f);
		ForcedActionTransform.SetLocation(ForcedActionTransform.GetLocation() + Offset);


		pActionForcedComponent->SetRelativeTransform(ForcedActionTransform);

		// Letting the parent actor know that this action is forced
		PlayerController->SetForcedActionForLocation(GetParentActor(), this);
	}
}

//TODO Create derived class Action_Arrows and move this function there
void AAction::UpdateArrowActionVisual()
{
	UpdatedArrowVisual = NewObject<UPaperFlipbookComponent>(this, ("UpdatedArrowVisual"));
	UpdatedArrowVisual->RegisterComponent();
	UpdatedArrowVisual->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Flipbooks/Footprints_Flipbook.Footprints_Flipbook'"))));

	FTransform TransformUpdatedArrowVisual = ActionFlipBookComponent->GetComponentTransform();
	TransformUpdatedArrowVisual.SetLocation(TransformUpdatedArrowVisual.GetLocation() + (FVector(-5.f, 0.f, 0.f))); // TODO get rid of magic number
	UpdatedArrowVisual->SetRelativeTransform(TransformUpdatedArrowVisual);

	if (EntangledInteractiveLocationDecoration) {
		PlayerController->ActivateEntangledILD(this);
	}

	if (ActionType == EActionType::Arrow_Move) { return; }
	if (ActionType == EActionType::ArrowUp_Plot) {
					ActionType = EActionType::Arrow_Move;
				ActionPointsRequired = 0;
	}
	else if (ActionType == EActionType::Teleport) {
			ActionType = EActionType::Arrow_Move;
		ActionPointsRequired = 0;
	} else {
					ActionType = EActionType::Arrow_Move;
				ActionPointsRequired = 0;
	}
}

//TODO Create derived class Action_Arrows and move this function there
void AAction::SetTeleport(TArray<AActor*> LocationsInPlay)
{
	auto Rand = FMath::RandRange(0, (LocationsInPlay.Num() - 1));

	UE_LOG(LogTemp, Warning, TEXT("Random is %i"), Rand)

	while (!LocationArrowPointsTo) {
		if (LocationsInPlay[Rand] != GetParentActor())
		{
			LocationArrowPointsTo = LocationsInPlay[Rand];
			UE_LOG(LogTemp, Warning, TEXT("Location to teleport: %s"),*LocationsInPlay[Rand]->GetName())
		}
		else {
			Rand = FMath::RandRange(0, (LocationsInPlay.Num() - 1));
			UE_LOG(LogTemp, Warning, TEXT("Random is %i"), Rand)
		}
	}

	PlayerController->CreateDoor(LocationsInPlay[Rand], DoorToCreateClass, TeleportActionToCreateClass, GetParentActor());
}

void AAction::SetIsHovered(bool IsHovered)
{
	if (IsHovered)
	{
		PlayerController->SetCurrentlyHoveredByMouseAction(true, this);
	}
	else
	{
		PlayerController->SetCurrentlyHoveredByMouseAction(false, nullptr);
	}
}

void AAction::CreateComponents()
{
	//----------------------Creating Root Component--------------------------
	pRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(pRootComponent);

	//----------------------Creating Action PaperFlipbook component----------
	ActionFlipBookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Action"));
	ActionFlipBookComponent->SetupAttachment(pRootComponent);

	//Setting up relative transform
	FTransform ActionFlipBookComponentRelativeTransform;
	ActionFlipBookComponentRelativeTransform.SetLocation(FVector(-40.f, 0.f, 0.f));
	ActionFlipBookComponentRelativeTransform.SetRotation(FRotator(0.f, 90.f, 0.f).Quaternion());
	ActionFlipBookComponentRelativeTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

	ActionFlipBookComponent->SetRelativeTransform(ActionFlipBookComponentRelativeTransform);

	//Setting up collision
	FCollisionResponseContainer ActionFlipBookComponentResponseContainer;
	ActionFlipBookComponentResponseContainer.SetAllChannels(ECollisionResponse::ECR_Overlap);
	ActionFlipBookComponentResponseContainer.SetResponse(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	ActionFlipBookComponentResponseContainer.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	ActionFlipBookComponent->SetCollisionResponseToChannels(ActionFlipBookComponentResponseContainer);
	ActionFlipBookComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
}