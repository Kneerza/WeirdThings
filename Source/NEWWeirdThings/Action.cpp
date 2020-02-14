// Fill out your copyright notice in the Description page of Project Settings.
#include "Action.h"
#include "InteractiveLocationDecoration.h"
#include "WeirdThingsPlayerController.h"
#include "LocationTemplate.h"
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
#include "Runtime/Engine/Classes/Components/SphereComponent.h"


#define PI 3.14159265

// Sets default values
AAction::AAction()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//----------------------Initialize all flipbooks for Locks---------------
	// TODO load only needed files
	pLockFood = LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Food_Flipbook.Food_Flipbook'")));
	pLockWood = LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Wood_Flipbook.Wood_Flipbook'")));
	pLockTool = LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Tool_Flipbook.Tool_Flipbook'")));
	pLockAxe = LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Axe_Flipbook.Axe_Flipbook'")));
	pLockItem_C = LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Lock_Item_Copper_Flipbook.Lock_Item_Copper_Flipbook'")));
	pLockItem_S = LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Lock_Item_Silver_Flipbook.Lock_Item_Silver_Flipbook'")));
	pLockItem_G = LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Lock_Item_Gold_Flipbook.Lock_Item_Gold_Flipbook'")));
	pLockExhaustion = LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Exhaustion_Flipbook.Exhaustion_Flipbook'")));
	pLockInsanity = LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Action_Locks/Flipbooks/Insanity_Flipbook.Insanity_Flipbook'")));
	pActionForced = LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Flipbooks/Action_Forced_Flipbook.Action_Forced_Flipbook'")));

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
	ActionFlipBookComponentRelativeTransform.SetScale3D(FVector(0.7f, 0.7f, 0.7f));

	ActionFlipBookComponent->SetRelativeTransform(ActionFlipBookComponentRelativeTransform);

	//Setting up collision
	FCollisionResponseContainer ActionFlipBookComponentResponseContainer;
	ActionFlipBookComponentResponseContainer.SetAllChannels(ECollisionResponse::ECR_Overlap);
	ActionFlipBookComponentResponseContainer.SetResponse(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	ActionFlipBookComponentResponseContainer.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	ActionFlipBookComponent->SetCollisionResponseToChannels(ActionFlipBookComponentResponseContainer);
	ActionFlipBookComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

	//----------------------Creating Collision Sphere------------------------
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Ground"));
	CollisionSphere->SetupAttachment(pRootComponent);

	//Setting up relative transform
	FTransform CollisionSphereRelativeTransform;
	CollisionSphereRelativeTransform.SetLocation(FVector(0.f, 0.f, 0.f));
	CollisionSphereRelativeTransform.SetRotation(FRotator(0.f, 0.f, 0.f).Quaternion());
	CollisionSphereRelativeTransform.SetScale3D(FVector(2.f, 2.f, 2.f));

	CollisionSphere->SetRelativeTransform(CollisionSphereRelativeTransform);

	//-----------------------Initializing arrays-----------------------------
	ActionLock.Init(nullptr, 8);
	ActionLockType.Init(EActionLockType::No_Need, 8);

}

// Called when the game starts or when spawned
void AAction::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsInfinite) {
		StartingActionLockType = ActionLockType;
	}

	ConstructActionLocks();
	ConstructModifierVisual();
	ForcedActionHandling();

}

// Called every frame
void AAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAction::ConstructActionLocks()
{
	
	for (int32 i = 0; i < ActionLockType.Num(); i++)
	{
		if (ActionLockType[i] == EActionLockType::No_Need) { return; }
		ActionLock[i] = NewObject<UPaperFlipbookComponent>(this, ("Lock    " + i));
		ActionLock[i]->RegisterComponent();
		switch (ActionLockType[i])
		{
		case EActionLockType::Need_Food:

			ActionLock[i]->SetFlipbook(pLockFood);
			break;

		case EActionLockType::Need_Wood:

			ActionLock[i]->SetFlipbook(pLockWood);
			break;

		case EActionLockType::Need_Tool:

			ActionLock[i]->SetFlipbook(pLockTool);
			break;

		case EActionLockType::Need_Axe:

			ActionLock[i]->SetFlipbook(pLockAxe);
			break;

		case EActionLockType::Need_Exhaustion:

			ActionLock[i]->SetFlipbook(pLockExhaustion);
			break;

		case EActionLockType::Need_Insanity:

			ActionLock[i]->SetFlipbook(pLockInsanity);
			break;

		case EActionLockType::Need_Item_C:

			ActionLock[i]->SetFlipbook(pLockItem_C);
			break;

		case EActionLockType::Need_Item_S:

			ActionLock[i]->SetFlipbook(pLockItem_S);
			break;

		case EActionLockType::Need_Item_G:

			ActionLock[i]->SetFlipbook(pLockItem_G);
			break;

		default:

			break;
		}


		FTransform TransformLock = ActionFlipBookComponent->GetComponentTransform();
		TransformLock.SetLocation(TransformLock.GetLocation() += (FVector(ActionLockOffset_X, ActionLocksOrbit*FMath::Sin(i*45.f*PI / 180.f), ActionLocksOrbit*FMath::Cos(i*45.f*PI / 180.f))));

		TransformLock.SetScale3D(FVector(2.5f, 2.5f, 2.5f));

		ActionLock[i]->SetRelativeTransform(TransformLock);
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
			return; 
		}

	}
	return;
}

void AAction::Unlock()
{
	if (!ActionLock[CurrentLockIndex]) { return; }
	ActionLockType[CurrentLockTypeIndex] = EActionLockType::No_Need;
	ActionLock[CurrentLockIndex]->UnregisterComponent();
	ActionLock[CurrentLockIndex] = nullptr;
}

void AAction::Deactivate()
{
	// --- Deactivating arrow actions ---

	if ((IsWorkedOut)&&((ActionType == EActionType::ArrowRight_Bad) || (ActionType == EActionType::ArrowRight_Good) || (ActionType == EActionType::ArrowRight_Ugly) || (ActionType == EActionType::ArrowUp_Plot)))
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
		CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (EntangledInteractiveLocationDecoration) {
			EntangledInteractiveLocationDecoration->InteractiveLocationDecoration_SpriteComponent_0->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
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
}

void AAction::Activate()
{
	// --- Setting color of activated Action ---
	ActionFlipBookComponent->SetSpriteColor(FLinearColor(1.f, 1.f, 1.f, 1.f));

	// --- Setting collision of activated Action ---
	ActionFlipBookComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (EntangledInteractiveLocationDecoration) {
		EntangledInteractiveLocationDecoration->InteractiveLocationDecoration_SpriteComponent_0->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	if (Child && pConnector)
	{
		pConnector->SetSpriteColor(FLinearColor(1.f, 1.f, 1.f, 1));
	}
}

void AAction::ForcedActionHandling()
{
	if (IsForced)
	{
		pActionForcedComponent = NewObject<UPaperFlipbookComponent>(this, "ForcedActionFlipbook");
		pActionForcedComponent->RegisterComponent();
		pActionForcedComponent->SetFlipbook(pActionForced);

		FTransform ForcedActionTransform = ActionFlipBookComponent->GetComponentTransform();
		FVector Offset = FVector(1.f, 0.f, 0.f);
		ForcedActionTransform.SetLocation(ForcedActionTransform.GetLocation() + Offset);

		pActionForcedComponent->SetRelativeTransform(ForcedActionTransform);

		Cast<ALocationTemplate>(GetParentActor())->ForcedAction = this;
	}
}

void AAction::UpdateArrowActionVisual()
{
	UpdatedArrowVisual = NewObject<UPaperFlipbookComponent>(this, ("UpdatedArrowVisual"));
	UpdatedArrowVisual->RegisterComponent();
	UpdatedArrowVisual->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/Actions/Flipbooks/Footprints_Flipbook.Footprints_Flipbook'"))));

	FTransform TransformUpdatedArrowVisual = ActionFlipBookComponent->GetComponentTransform();
	TransformUpdatedArrowVisual.SetLocation(TransformUpdatedArrowVisual.GetLocation() + (FVector(-5.f, 0.f, 0.f))); // TODO get rid of magic number
	UpdatedArrowVisual->SetRelativeTransform(TransformUpdatedArrowVisual);

	if (EntangledInteractiveLocationDecoration) {
		EntangledInteractiveLocationDecoration->InteractiveLocationDecoration_SpriteComponent_0->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	auto ParentLocation = Cast<ALocationTemplate>(GetParentActor());
	auto ParentLocationHorizontalIndex = ParentLocation->HorizontalIndex;
	auto ParentLocationVerticalIndex = ParentLocation->VerticalIndex;

	auto PlayerController = Cast<AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController());
	auto LocationsInPlay = PlayerController->AllLocationsInPlay;

	if (ActionType == EActionType::ArrowUp_Plot) {
		for (int32 i = 0; i < LocationsInPlay.Num(); i++)
		{
			if (((LocationsInPlay[i]->VerticalIndex - ParentLocationVerticalIndex) == 1) && (ParentLocationHorizontalIndex == LocationsInPlay[i]->HorizontalIndex))
			{
				LocationArrowPointsTo = LocationsInPlay[i];
				UE_LOG(LogTemp, Warning, TEXT("Arrow points to: %s"), *LocationArrowPointsTo->GetName())
					ActionType = EActionType::Arrow_Move;
				ActionPointsRequired = 0;
				break;
			}
		}
	}
	else {
		for (int32 i = 0; i < LocationsInPlay.Num(); i++)
		{
			if (((LocationsInPlay[i]->HorizontalIndex - ParentLocationHorizontalIndex) == 1) && (ParentLocationVerticalIndex == LocationsInPlay[i]->VerticalIndex))
			{
				LocationArrowPointsTo = LocationsInPlay[i];
				UE_LOG(LogTemp, Warning, TEXT("Arrow points to: %s"), *LocationArrowPointsTo->GetName())
					ActionType = EActionType::Arrow_Move;
				ActionPointsRequired = 0;
				break;
			}
		}
	}
}