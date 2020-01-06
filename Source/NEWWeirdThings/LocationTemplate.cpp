// Fill out your copyright notice in the Description page of Project Settings.

#include "LocationTemplate.h"
#include "Action.h"
#include "InteractiveLocationDecoration.h"
#include "WeirdThingsPlayerController.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/Core/Public/Math/Rotator.h"
#include "Runtime/Core/Public/Math/TransformNonVectorized.h"
#include "Runtime/Core/Public/Math/Quat.h"
#include "Components/SceneComponent.h"
#include "ArrowTemplate.h"

// Sets default values
ALocationTemplate::ALocationTemplate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//----------------------Creating Root Component--------------------------
	pRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(pRootComponent);

	//----------------------Attachinng Ground Mesh---------------------------
	pGround = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground"));
	pGround->SetupAttachment(pRootComponent);

	//Setting up relative transform
	FTransform GroundRelativeTransform;
	GroundRelativeTransform.SetLocation(FVector(0.f, 0.f, 0.f));
	GroundRelativeTransform.SetRotation(FRotator(15.0f, 0.0f, 0.0f).Quaternion());
	GroundRelativeTransform.SetScale3D(FVector(12.f, 20.f, 0.5f));

	pGround->SetRelativeTransform(GroundRelativeTransform);

	//----------------------Attaching BackGround Mesh---------------------------
	pBackGround = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackGround"));
	pBackGround->SetupAttachment(pRootComponent);

	//Setting up relative transform
	FTransform BackGroundRelativeTransform;
	BackGroundRelativeTransform.SetLocation(FVector(630.f, 0.f, 150.f));
	BackGroundRelativeTransform.SetRotation(FRotator(0.f, 0.f, 0.f).Quaternion());
	BackGroundRelativeTransform.SetScale3D(FVector(0.5f, 20.f, 8.f));

	pBackGround->SetRelativeTransform(BackGroundRelativeTransform);
	//--------------------------------------------------------------------------

	//-----------------------Creating sockets-----------------------------------

	//Setting up collision  
	FCollisionResponseContainer CollisionResponseContainer;
	CollisionResponseContainer.SetAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	FVector PlayerIncrement = FVector(13.f, 50.f, 3.56f);
	FVector EncounterIncrement = FVector(13.f, -50.f, 3.56f);
	FVector OriginalPosition = FVector(-471.f, -367.f, -74.5f);
	FVector CampFireOffset = FVector(0.f, 150.f, 0.f);
	FVector EncounterOffset = FVector(0.f, 600.f, 0.f);

	FVector Scale = FVector(0.01f, 0.3f, 1.f);

	SocketPlayer_0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayer_0"));
	SocketPlayer_0->SetupAttachment(pRootComponent);
	SocketPlayer_0->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketPlayer_0->SetRelativeScale3D(Scale);
	SocketPlayer_0->SetRelativeLocation((0 * PlayerIncrement) + OriginalPosition);
	SocketPlayer_0->SetHiddenInGame(true);
	SocketPlayer_0->SetMobility(EComponentMobility::Movable);
	SocketPlayer_0->CastShadow = false;
	AvailableSocketPlayer.Emplace(SocketPlayer_0);

	SocketPlayer_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayer_1"));
	SocketPlayer_1->SetupAttachment(pRootComponent);
	SocketPlayer_1->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketPlayer_1->SetRelativeScale3D(Scale);
	SocketPlayer_1->SetRelativeLocation((1 * PlayerIncrement) + OriginalPosition);
	SocketPlayer_1->SetHiddenInGame(true);
	SocketPlayer_1->SetMobility(EComponentMobility::Movable);
	SocketPlayer_1->CastShadow = false;
	AvailableSocketPlayer.Emplace(SocketPlayer_1);

	SocketPlayer_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayer_2"));
	SocketPlayer_2->SetupAttachment(pRootComponent);
	SocketPlayer_2->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketPlayer_2->SetRelativeScale3D(Scale);
	SocketPlayer_2->SetRelativeLocation((2 * PlayerIncrement) + OriginalPosition);
	SocketPlayer_2->SetHiddenInGame(true);
	SocketPlayer_2->SetMobility(EComponentMobility::Movable);
	SocketPlayer_2->CastShadow = false;
	AvailableSocketPlayer.Emplace(SocketPlayer_2);

	SocketPlayer_3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayer_3"));
	SocketPlayer_3->SetupAttachment(pRootComponent);
	SocketPlayer_3->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketPlayer_3->SetRelativeScale3D(Scale);
	SocketPlayer_3->SetRelativeLocation((3 * PlayerIncrement) + OriginalPosition);
	SocketPlayer_3->SetHiddenInGame(true);
	SocketPlayer_3->SetMobility(EComponentMobility::Movable);
	SocketPlayer_3->CastShadow = false;
	AvailableSocketPlayer.Emplace(SocketPlayer_3);

	SocketPlayer_4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayer_4"));
	SocketPlayer_4->SetupAttachment(pRootComponent);
	SocketPlayer_4->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketPlayer_4->SetRelativeScale3D(Scale);
	SocketPlayer_4->SetRelativeLocation((4 * PlayerIncrement) + OriginalPosition);
	SocketPlayer_4->SetHiddenInGame(true);
	SocketPlayer_4->SetMobility(EComponentMobility::Movable);
	SocketPlayer_4->CastShadow = false;
	AvailableSocketPlayer.Emplace(SocketPlayer_4);

	SocketCampFire = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketCampFire"));
	SocketCampFire->SetupAttachment(pRootComponent);
	SocketCampFire->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketCampFire->SetRelativeScale3D(Scale);
	SocketCampFire->SetRelativeLocation((2 * PlayerIncrement) + OriginalPosition + CampFireOffset);
	SocketCampFire->SetHiddenInGame(true);
	SocketCampFire->SetMobility(EComponentMobility::Movable);
	SocketCampFire->CastShadow = false;
	AvailableSocketCampFire.Emplace(SocketCampFire);

	SocketEncounter_0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_0"));
	SocketEncounter_0->SetupAttachment(pRootComponent);
	SocketEncounter_0->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_0->SetRelativeScale3D(Scale);
	SocketEncounter_0->SetRelativeLocation(OriginalPosition + EncounterOffset + (0 * EncounterIncrement));
	SocketEncounter_0->SetHiddenInGame(true);
	SocketEncounter_0->SetMobility(EComponentMobility::Movable);
	SocketEncounter_0->CastShadow = false;
	AvailableSocketEncounter.Emplace(SocketEncounter_0);

	SocketEncounter_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_1"));
	SocketEncounter_1->SetupAttachment(pRootComponent);
	SocketEncounter_1->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_1->SetRelativeScale3D(Scale);
	SocketEncounter_1->SetRelativeLocation(OriginalPosition + EncounterOffset + (1 * EncounterIncrement));
	SocketEncounter_1->SetHiddenInGame(true);
	SocketEncounter_1->SetMobility(EComponentMobility::Movable);
	SocketEncounter_1->CastShadow = false;
	AvailableSocketEncounter.Emplace(SocketEncounter_1);

	SocketEncounter_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_2"));
	SocketEncounter_2->SetupAttachment(pRootComponent);
	SocketEncounter_2->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_2->SetRelativeScale3D(Scale);
	SocketEncounter_2->SetRelativeLocation(OriginalPosition + EncounterOffset + (2 * EncounterIncrement));
	SocketEncounter_2->SetHiddenInGame(true);
	SocketEncounter_2->SetMobility(EComponentMobility::Movable);
	SocketEncounter_2->CastShadow = false;
	AvailableSocketEncounter.Emplace(SocketEncounter_2);

	SocketEncounter_3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_3"));
	SocketEncounter_3->SetupAttachment(pRootComponent);
	SocketEncounter_3->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_3->SetRelativeScale3D(Scale);
	SocketEncounter_3->SetRelativeLocation(OriginalPosition + EncounterOffset + (3 * EncounterIncrement));
	SocketEncounter_3->SetHiddenInGame(true);
	SocketEncounter_3->SetMobility(EComponentMobility::Movable);
	SocketEncounter_3->CastShadow = false;
	AvailableSocketEncounter.Emplace(SocketEncounter_3);

	SocketEncounter_4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_4"));
	SocketEncounter_4->SetupAttachment(pRootComponent);
	SocketEncounter_4->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_4->SetRelativeScale3D(Scale);
	SocketEncounter_4->SetRelativeLocation(OriginalPosition + EncounterOffset + (4 * EncounterIncrement));
	SocketEncounter_4->SetHiddenInGame(true);
	SocketEncounter_4->SetMobility(EComponentMobility::Movable);
	SocketEncounter_4->CastShadow = false;
	AvailableSocketEncounter.Emplace(SocketEncounter_4);

	//-----------------------Initializing arrays--------------------------------
	SocketsLockations.Init(FVector(0.f), 16);
	SocketPlayer_Transform.Init(FTransform(), 5);
	SocketEncounter_Transform.Init(FTransform(), 5);
}

// Called when the game starts or when spawned
void ALocationTemplate::BeginPlay()
{
	Super::BeginPlay();

	(Cast<AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController())->AllLocationsInPlay).Add(this);

}

// Called every frame
void ALocationTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ALocationTemplate::ExcludeFromAvailablePlayerSockets(UPrimitiveComponent* ComponentToExclude)
{
	AvailableSocketPlayer.RemoveSingle(Cast<UStaticMeshComponent>(ComponentToExclude));
}

void ALocationTemplate::IncludeInAvailablePlayerSockets(UPrimitiveComponent* ComponentToInclude)
{
	AvailableSocketPlayer.Emplace(Cast<UStaticMeshComponent>(ComponentToInclude));
}

void ALocationTemplate::ExcludeFromAvailableEncounterSockets(UPrimitiveComponent* ComponentToExclude)
{
	AvailableSocketEncounter.RemoveSingle(Cast<UStaticMeshComponent>(ComponentToExclude));
}

void ALocationTemplate::IncludeInAvailableEncounterSockets(UPrimitiveComponent* ComponentToInclude)
{
	AvailableSocketEncounter.Emplace(Cast<UStaticMeshComponent>(ComponentToInclude));
}

void ALocationTemplate::Connect(UChildActorComponent* Parent, UPaperFlipbookComponent* Connector_0, UChildActorComponent* Child_0, UPaperFlipbookComponent* Connector_1, UChildActorComponent* Child_1, UPaperFlipbookComponent* Connector_2, UChildActorComponent* Child_2, UPaperFlipbookComponent* Connector_3, UChildActorComponent* Child_3, UPaperFlipbookComponent* Connector_4, UChildActorComponent* Child_4)
{
	auto ParentAction = Cast<AAction>(Parent->GetChildActor());
	AAction* Action_Child_0 = nullptr;
	if (Child_0) { Action_Child_0 = Cast<AAction>(Child_0->GetChildActor()); }

	AAction* Action_Child_1 = nullptr;
	if (Child_1) { Action_Child_1 = Cast<AAction>(Child_1->GetChildActor()); }

	AAction* Action_Child_2 = nullptr;
	if (Child_2) { Action_Child_2 = Cast<AAction>(Child_2->GetChildActor()); }

	AAction* Action_Child_3 = nullptr;
	if (Child_3) { Action_Child_3 = Cast<AAction>(Child_3->GetChildActor()); }

	AAction* Action_Child_4 = nullptr;
	if (Child_4) { Action_Child_4 = Cast<AAction>(Child_4->GetChildActor()); }

	if (Action_Child_0) {
		ParentAction->Child = Action_Child_0;
		ParentAction->pConnector = Connector_0;
		Action_Child_0->Deactivate();
	}

	if (Action_Child_1) {
		Action_Child_0->Child = Action_Child_1;
		Action_Child_0->pConnector = Connector_1;
		Action_Child_1->Deactivate();
	}

	if (Action_Child_2) {
		Action_Child_1->Child = Action_Child_2;
		Action_Child_1->pConnector = Connector_2;
		Action_Child_2->Deactivate();
	}

	if (Action_Child_3) {
		Action_Child_2->Child = Action_Child_3;
		Action_Child_2->pConnector = Connector_3;
		Action_Child_3->Deactivate();
	}

	if (Action_Child_4) {
		Action_Child_3->Child = Action_Child_4;
		Action_Child_3->pConnector = Connector_4;
		Action_Child_4->Deactivate();
	}

}

void ALocationTemplate::ExcludeFromAvailableCampFireSocket(UPrimitiveComponent* ComponentToExclude)
{
	AvailableSocketCampFire.RemoveSingle(Cast<UStaticMeshComponent>(ComponentToExclude));
}

void ALocationTemplate::IncludeInAvailableCampFireSocket(UPrimitiveComponent* ComponentToInclude)
{
	AvailableSocketCampFire.Emplace(Cast<UStaticMeshComponent>(ComponentToInclude));
}

void ALocationTemplate::EntangleActionWithActor(UChildActorComponent* Action, UChildActorComponent* InteractiveLocationDecoration)
{
	auto ActionToEntangle = Cast<AAction>(Action->GetChildActor());
	auto InteractiveLocationDecorationToEntangle = Cast<AInteractiveLocationDecoration>(InteractiveLocationDecoration->GetChildActor());

	ActionToEntangle->EntangledInteractiveLocationDecoration = InteractiveLocationDecorationToEntangle;
	InteractiveLocationDecorationToEntangle->EntangledAction = ActionToEntangle;
}