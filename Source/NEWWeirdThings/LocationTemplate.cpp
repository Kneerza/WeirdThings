// Fill out your copyright notice in the Description page of Project Settings.

#include "LocationTemplate.h"
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

// Sets default values
ALocationTemplate::ALocationTemplate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//----------------------Creating Root Component--------------------------
	pRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(pRootComponent);

	//----------------------Attachinng Ground Mesh---------------------------
	auto CubeShape = LoadObject<UStaticMesh>(nullptr, (TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'")));

	pGround = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground"));
	pGround->SetupAttachment(pRootComponent);

	//Setting up relative transform
	FTransform GroundRelativeTransform;
	GroundRelativeTransform.SetLocation(FVector(0.f, 0.f, 0.f));
	GroundRelativeTransform.SetRotation(FRotator(15.0f, 0.0f, 0.0f).Quaternion());
	GroundRelativeTransform.SetScale3D(FVector(12.f, 20.f, 0.5f));

	pGround->SetRelativeTransform(GroundRelativeTransform);
	pGround->SetCastShadow(false);
	pGround->SetStaticMesh(CubeShape);

	//----------------------Attaching BackGround Mesh---------------------------
	pBackGround = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackGround"));
	pBackGround->SetupAttachment(pRootComponent);

	//Setting up relative transform
	FTransform BackGroundRelativeTransform;
	BackGroundRelativeTransform.SetLocation(FVector(630.f, 0.f, 150.f));
	BackGroundRelativeTransform.SetRotation(FRotator(0.f, 0.f, 0.f).Quaternion());
	BackGroundRelativeTransform.SetScale3D(FVector(0.5f, 20.f, 8.f));

	pBackGround->SetRelativeTransform(BackGroundRelativeTransform);
	pBackGround->SetStaticMesh(CubeShape);
	//--------------------------------------------------------------------------

	//-----------------------Creating sockets-----------------------------------

	//Setting up collision  
	FCollisionResponseContainer CollisionResponseContainer;
	CollisionResponseContainer.SetAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	FVector PlayerIncrement = FVector(13.f, 120.f, 3.56f);
	FVector EncounterIncrement = FVector(13.f, -120.f, 3.56f);
	FVector DynamicActionIncrement = FVector(0.f, -220.f, 0.f);
	FVector DynamicPlayerActionIncrement = FVector(0.f, 220.f, 0.f);
	FVector OriginalPosition = FVector(-471.f, -900.f, -74.5f);
	FVector CampFireOffset = FVector(0.f, 500.f, 0.f);
	FVector EncounterOffset = FVector(0.f, 1500.f, 0.f);
	FVector Encounter_GoodOffset = FVector(182.f, 880.f, 49.84f);
	FVector Encounter_DeadOffset = FVector(182.f, 1700.f, 49.84f);

	FVector Scale = FVector(0.01f, 0.3f, 1.f);

	auto ConeShape = LoadObject<UStaticMesh>(nullptr, (TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone'")));

	SocketDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketDoor"));
	SocketDoor->SetupAttachment(pRootComponent);
	SocketDoor->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketDoor->SetRelativeScale3D(Scale);
	SocketDoor->SetRelativeLocation(FVector(-216.f, -364.f, 8.5f));
	SocketDoor->SetHiddenInGame(true);
	SocketDoor->SetMobility(EComponentMobility::Movable);
	SocketDoor->SetStaticMesh(ConeShape);
	SocketDoor->CastShadow = false;

	SocketPlayer_0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayer_0"));
	SocketPlayer_0->SetupAttachment(pRootComponent);
	SocketPlayer_0->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketPlayer_0->SetRelativeScale3D(Scale);
	SocketPlayer_0->SetRelativeLocation((0 * PlayerIncrement) + OriginalPosition);
	SocketPlayer_0->SetHiddenInGame(true);
	SocketPlayer_0->SetMobility(EComponentMobility::Movable);
	SocketPlayer_0->SetStaticMesh(ConeShape);
	SocketPlayer_0->CastShadow = false;
	AvailableSocketPlayer.Emplace(SocketPlayer_0);

	SocketPlayer_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayer_1"));
	SocketPlayer_1->SetupAttachment(pRootComponent);
	SocketPlayer_1->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketPlayer_1->SetRelativeScale3D(Scale);
	SocketPlayer_1->SetRelativeLocation((1 * PlayerIncrement) + OriginalPosition);
	SocketPlayer_1->SetHiddenInGame(true);
	SocketPlayer_1->SetMobility(EComponentMobility::Movable);
	SocketPlayer_1->SetStaticMesh(ConeShape);
	SocketPlayer_1->CastShadow = false;
	AvailableSocketPlayer.Emplace(SocketPlayer_1);

	SocketPlayer_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayer_2"));
	SocketPlayer_2->SetupAttachment(pRootComponent);
	SocketPlayer_2->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketPlayer_2->SetRelativeScale3D(Scale);
	SocketPlayer_2->SetRelativeLocation((2 * PlayerIncrement) + OriginalPosition);
	SocketPlayer_2->SetHiddenInGame(true);
	SocketPlayer_2->SetMobility(EComponentMobility::Movable);
	SocketPlayer_2->SetStaticMesh(ConeShape);
	SocketPlayer_2->CastShadow = false;
	AvailableSocketPlayer.Emplace(SocketPlayer_2);

	SocketPlayer_3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayer_3"));
	SocketPlayer_3->SetupAttachment(pRootComponent);
	SocketPlayer_3->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketPlayer_3->SetRelativeScale3D(Scale);
	SocketPlayer_3->SetRelativeLocation((3 * PlayerIncrement) + OriginalPosition);
	SocketPlayer_3->SetHiddenInGame(true);
	SocketPlayer_3->SetMobility(EComponentMobility::Movable);
	SocketPlayer_3->SetStaticMesh(ConeShape);
	SocketPlayer_3->CastShadow = false;
	AvailableSocketPlayer.Emplace(SocketPlayer_3);

	SocketPlayer_4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayer_4"));
	SocketPlayer_4->SetupAttachment(pRootComponent);
	SocketPlayer_4->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketPlayer_4->SetRelativeScale3D(Scale);
	SocketPlayer_4->SetRelativeLocation((4 * PlayerIncrement) + OriginalPosition);
	SocketPlayer_4->SetHiddenInGame(true);
	SocketPlayer_4->SetMobility(EComponentMobility::Movable);
	SocketPlayer_4->SetStaticMesh(ConeShape);
	SocketPlayer_4->CastShadow = false;
	AvailableSocketPlayer.Emplace(SocketPlayer_4);

	SocketCampFire = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketCampFire"));
	SocketCampFire->SetupAttachment(pRootComponent);
	SocketCampFire->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketCampFire->SetRelativeScale3D(Scale);
	SocketCampFire->SetRelativeLocation((2 * PlayerIncrement) + OriginalPosition + CampFireOffset);
	SocketCampFire->SetHiddenInGame(true);
	SocketCampFire->SetMobility(EComponentMobility::Movable);
	SocketCampFire->SetStaticMesh(ConeShape);
	SocketCampFire->CastShadow = false;
	AvailableSocketCampFire.Emplace(SocketCampFire);

	SocketEncounter_0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_0"));
	SocketEncounter_0->SetupAttachment(pRootComponent);
	SocketEncounter_0->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_0->SetRelativeScale3D(Scale);
	SocketEncounter_0->SetRelativeLocation(OriginalPosition + EncounterOffset + (0 * EncounterIncrement));
	SocketEncounter_0->SetHiddenInGame(true);
	SocketEncounter_0->SetMobility(EComponentMobility::Movable);
	SocketEncounter_0->SetStaticMesh(ConeShape);
	SocketEncounter_0->CastShadow = false;
	AvailableSocketEncounter.Emplace(SocketEncounter_0);

	SocketEncounter_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_1"));
	SocketEncounter_1->SetupAttachment(pRootComponent);
	SocketEncounter_1->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_1->SetRelativeScale3D(Scale);
	SocketEncounter_1->SetRelativeLocation(OriginalPosition + EncounterOffset + (1 * EncounterIncrement));
	SocketEncounter_1->SetHiddenInGame(true);
	SocketEncounter_1->SetMobility(EComponentMobility::Movable);
	SocketEncounter_1->SetStaticMesh(ConeShape);
	SocketEncounter_1->CastShadow = false;
	AvailableSocketEncounter.Emplace(SocketEncounter_1);

	SocketEncounter_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_2"));
	SocketEncounter_2->SetupAttachment(pRootComponent);
	SocketEncounter_2->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_2->SetRelativeScale3D(Scale);
	SocketEncounter_2->SetRelativeLocation(OriginalPosition + EncounterOffset + (2 * EncounterIncrement));
	SocketEncounter_2->SetHiddenInGame(true);
	SocketEncounter_2->SetMobility(EComponentMobility::Movable);
	SocketEncounter_2->SetStaticMesh(ConeShape);
	SocketEncounter_2->CastShadow = false;
	AvailableSocketEncounter.Emplace(SocketEncounter_2);

	SocketEncounter_3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_3"));
	SocketEncounter_3->SetupAttachment(pRootComponent);
	SocketEncounter_3->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_3->SetRelativeScale3D(Scale);
	SocketEncounter_3->SetRelativeLocation(OriginalPosition + EncounterOffset + (3 * EncounterIncrement));
	SocketEncounter_3->SetHiddenInGame(true);
	SocketEncounter_3->SetMobility(EComponentMobility::Movable);
	SocketEncounter_3->SetStaticMesh(ConeShape);
	SocketEncounter_3->CastShadow = false;
	AvailableSocketEncounter.Emplace(SocketEncounter_3);

	SocketEncounter_4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_4"));
	SocketEncounter_4->SetupAttachment(pRootComponent);
	SocketEncounter_4->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_4->SetRelativeScale3D(Scale);
	SocketEncounter_4->SetRelativeLocation(OriginalPosition + EncounterOffset + (4 * EncounterIncrement));
	SocketEncounter_4->SetHiddenInGame(true);
	SocketEncounter_4->SetMobility(EComponentMobility::Movable);
	SocketEncounter_4->SetStaticMesh(ConeShape);
	SocketEncounter_4->CastShadow = false;
	AvailableSocketEncounter.Emplace(SocketEncounter_4);

	SocketEncounter_Dead_0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_Dead_0"));
	SocketEncounter_Dead_0->SetupAttachment(pRootComponent);
	SocketEncounter_Dead_0->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_Dead_0->SetRelativeScale3D(Scale);
	SocketEncounter_Dead_0->SetRelativeLocation(OriginalPosition + Encounter_DeadOffset + (0 * EncounterIncrement));
	SocketEncounter_Dead_0->SetHiddenInGame(true);
	SocketEncounter_Dead_0->SetMobility(EComponentMobility::Movable);
	SocketEncounter_Dead_0->SetStaticMesh(ConeShape);
	SocketEncounter_Dead_0->CastShadow = false;
	AvailableSocketEncounter_Dead.Emplace(SocketEncounter_0);

	SocketEncounter_Dead_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_Dead_1"));
	SocketEncounter_Dead_1->SetupAttachment(pRootComponent);
	SocketEncounter_Dead_1->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_Dead_1->SetRelativeScale3D(Scale);
	SocketEncounter_Dead_1->SetRelativeLocation(OriginalPosition + Encounter_DeadOffset + (1 * EncounterIncrement));
	SocketEncounter_Dead_1->SetHiddenInGame(true);
	SocketEncounter_Dead_1->SetMobility(EComponentMobility::Movable);
	SocketEncounter_Dead_1->SetStaticMesh(ConeShape);
	SocketEncounter_Dead_1->CastShadow = false;
	AvailableSocketEncounter_Dead.Emplace(SocketEncounter_1);

	SocketEncounter_Dead_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_Dead_2"));
	SocketEncounter_Dead_2->SetupAttachment(pRootComponent);
	SocketEncounter_Dead_2->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_Dead_2->SetRelativeScale3D(Scale);
	SocketEncounter_Dead_2->SetRelativeLocation(OriginalPosition + Encounter_DeadOffset + (2 * EncounterIncrement));
	SocketEncounter_Dead_2->SetHiddenInGame(true);
	SocketEncounter_Dead_2->SetMobility(EComponentMobility::Movable);
	SocketEncounter_Dead_2->SetStaticMesh(ConeShape);
	SocketEncounter_Dead_2->CastShadow = false;
	AvailableSocketEncounter_Dead.Emplace(SocketEncounter_2);

	SocketEncounter_Dead_3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_Dead_3"));
	SocketEncounter_Dead_3->SetupAttachment(pRootComponent);
	SocketEncounter_Dead_3->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_Dead_3->SetRelativeScale3D(Scale);
	SocketEncounter_Dead_3->SetRelativeLocation(OriginalPosition + Encounter_DeadOffset + (3 * EncounterIncrement));
	SocketEncounter_Dead_3->SetHiddenInGame(true);
	SocketEncounter_Dead_3->SetMobility(EComponentMobility::Movable);
	SocketEncounter_Dead_3->SetStaticMesh(ConeShape);
	SocketEncounter_Dead_3->CastShadow = false;
	AvailableSocketEncounter_Dead.Emplace(SocketEncounter_3);

	SocketEncounter_Dead_4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_Dead_4"));
	SocketEncounter_Dead_4->SetupAttachment(pRootComponent);
	SocketEncounter_Dead_4->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_Dead_4->SetRelativeScale3D(Scale);
	SocketEncounter_Dead_4->SetRelativeLocation(OriginalPosition + Encounter_DeadOffset + (4 * EncounterIncrement));
	SocketEncounter_Dead_4->SetHiddenInGame(true);
	SocketEncounter_Dead_4->SetMobility(EComponentMobility::Movable);
	SocketEncounter_Dead_4->SetStaticMesh(ConeShape);
	SocketEncounter_Dead_4->CastShadow = false;
	AvailableSocketEncounter_Dead.Emplace(SocketEncounter_4);

	SocketEncounter_Good_0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_Good_0"));
	SocketEncounter_Good_0->SetupAttachment(pRootComponent);
	SocketEncounter_Good_0->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_Good_0->SetRelativeScale3D(Scale);
	SocketEncounter_Good_0->SetRelativeLocation(OriginalPosition + Encounter_GoodOffset + (0 * EncounterIncrement));
	SocketEncounter_Good_0->SetHiddenInGame(true);
	SocketEncounter_Good_0->SetMobility(EComponentMobility::Movable);
	SocketEncounter_Good_0->SetStaticMesh(ConeShape);
	SocketEncounter_Good_0->CastShadow = false;
	AvailableSocketEncounter_Good.Emplace(SocketEncounter_Good_0);

	SocketEncounter_Good_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_Good_1"));
	SocketEncounter_Good_1->SetupAttachment(pRootComponent);
	SocketEncounter_Good_1->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_Good_1->SetRelativeScale3D(Scale);
	SocketEncounter_Good_1->SetRelativeLocation(OriginalPosition + Encounter_GoodOffset + (1 * EncounterIncrement));
	SocketEncounter_Good_1->SetHiddenInGame(true);
	SocketEncounter_Good_1->SetMobility(EComponentMobility::Movable);
	SocketEncounter_Good_1->SetStaticMesh(ConeShape);
	SocketEncounter_Good_1->CastShadow = false;
	AvailableSocketEncounter_Good.Emplace(SocketEncounter_Good_1);

	SocketEncounter_Good_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketEncounter_Good_2"));
	SocketEncounter_Good_2->SetupAttachment(pRootComponent);
	SocketEncounter_Good_2->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketEncounter_Good_2->SetRelativeScale3D(Scale);
	SocketEncounter_Good_2->SetRelativeLocation(OriginalPosition + Encounter_GoodOffset + (2 * EncounterIncrement));
	SocketEncounter_Good_2->SetHiddenInGame(true);
	SocketEncounter_Good_2->SetMobility(EComponentMobility::Movable);
	SocketEncounter_Good_2->SetStaticMesh(ConeShape);
	SocketEncounter_Good_2->CastShadow = false;
	AvailableSocketEncounter_Good.Emplace(SocketEncounter_Good_2);

	OriginalPosition = FVector(-526.f, 1008.f, -807.f);
	Scale = FVector(1.f, 1.f, 1.f);
	/*
	SocketDynamicAction_0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketDynamicAction_0"));
	SocketDynamicAction_0->SetupAttachment(pRootComponent);
	SocketDynamicAction_0->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketDynamicAction_0->SetRelativeScale3D(Scale);
	SocketDynamicAction_0->SetRelativeLocation(OriginalPosition + (0 * DynamicActionIncrement));
	SocketDynamicAction_0->SetHiddenInGame(true);
	SocketDynamicAction_0->SetMobility(EComponentMobility::Movable);
	SocketDynamicAction_0->CastShadow = false;
	AvailableSocketDynamicAction.Emplace(SocketDynamicAction_0);
	
	SocketDynamicAction_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketDynamicAction_1"));
	SocketDynamicAction_1->SetupAttachment(pRootComponent);
	SocketDynamicAction_1->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketDynamicAction_1->SetRelativeScale3D(Scale);
	SocketDynamicAction_1->SetRelativeLocation(OriginalPosition + (1 * DynamicActionIncrement));
	SocketDynamicAction_1->SetHiddenInGame(true);
	SocketDynamicAction_1->SetMobility(EComponentMobility::Movable);
	SocketDynamicAction_1->CastShadow = false;
	AvailableSocketDynamicAction.Emplace(SocketDynamicAction_1);
	
	SocketDynamicAction_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketDynamicAction_2"));
	SocketDynamicAction_2->SetupAttachment(pRootComponent);
	SocketDynamicAction_2->SetCollisionResponseToChannels(CollisionResponseContainer);
	SocketDynamicAction_2->SetRelativeScale3D(Scale);
	SocketDynamicAction_2->SetRelativeLocation(OriginalPosition + (2 * DynamicActionIncrement));
	SocketDynamicAction_2->SetHiddenInGame(true);
	SocketDynamicAction_2->SetMobility(EComponentMobility::Movable);
	SocketDynamicAction_2->CastShadow = false;
	AvailableSocketDynamicAction.Emplace(SocketDynamicAction_2);
	*/

	AvailableSocketDynamicPlayerAction.Init(nullptr, 27);

	SocketDynamicPlayerAction_0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_0"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_0, 0);
	AvailableSocketDynamicPlayerAction[0]=SocketDynamicPlayerAction_0;

	SocketDynamicPlayerAction_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_1"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_1, 1);
	AvailableSocketDynamicPlayerAction[1] = (SocketDynamicPlayerAction_1);

	SocketDynamicPlayerAction_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_2"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_2, 2);
	AvailableSocketDynamicPlayerAction[2] = (SocketDynamicPlayerAction_2);

	SocketDynamicPlayerAction_3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_3"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_3, 3);
	AvailableSocketDynamicPlayerAction[3] = (SocketDynamicPlayerAction_3);

	SocketDynamicPlayerAction_4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_4"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_4, 4);
	AvailableSocketDynamicPlayerAction[4] = (SocketDynamicPlayerAction_4);

	SocketDynamicPlayerAction_5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_5"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_5, 5);
	AvailableSocketDynamicPlayerAction[5] = (SocketDynamicPlayerAction_5);

	SocketDynamicPlayerAction_6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_6"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_6, 6);
	AvailableSocketDynamicPlayerAction[6] = (SocketDynamicPlayerAction_6);

	SocketDynamicPlayerAction_7 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_7"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_7, 7);
	AvailableSocketDynamicPlayerAction[7] = (SocketDynamicPlayerAction_7);

	SocketDynamicPlayerAction_8 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_8"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_8, 8);
	AvailableSocketDynamicPlayerAction[8] = (SocketDynamicPlayerAction_8);

	SocketDynamicPlayerAction_9 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_9"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_9, 9);
	AvailableSocketDynamicPlayerAction[9] = (SocketDynamicPlayerAction_9);

	SocketDynamicPlayerAction_10 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_10"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_10, 10);
	AvailableSocketDynamicPlayerAction[10] = (SocketDynamicPlayerAction_10);

	SocketDynamicPlayerAction_11 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_11"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_11, 11);
	AvailableSocketDynamicPlayerAction[11] = (SocketDynamicPlayerAction_11);

	SocketDynamicPlayerAction_12 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_12"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_12, 12);
	AvailableSocketDynamicPlayerAction[12] = (SocketDynamicPlayerAction_12);

	SocketDynamicPlayerAction_13 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_13"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_13, 13);
	AvailableSocketDynamicPlayerAction[13] = (SocketDynamicPlayerAction_13);

	SocketDynamicPlayerAction_14 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_14"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_14, 14);
	AvailableSocketDynamicPlayerAction[14] = (SocketDynamicPlayerAction_14);

	SocketDynamicPlayerAction_15 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_15"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_15, 15);
	AvailableSocketDynamicPlayerAction[15] = (SocketDynamicPlayerAction_15);

	SocketDynamicPlayerAction_16 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_16"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_16, 16);
	AvailableSocketDynamicPlayerAction[16] = (SocketDynamicPlayerAction_16);

	SocketDynamicPlayerAction_17 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_17"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_17, 17);
	AvailableSocketDynamicPlayerAction[17] = (SocketDynamicPlayerAction_17);

	SocketDynamicPlayerAction_18 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_18"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_18, 18);
	AvailableSocketDynamicPlayerAction[18] = (SocketDynamicPlayerAction_18);

	SocketDynamicPlayerAction_19 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_19"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_19, 19);
	AvailableSocketDynamicPlayerAction[19] = (SocketDynamicPlayerAction_19);

	SocketDynamicPlayerAction_20 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_20"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_20, 20);
	AvailableSocketDynamicPlayerAction[20] = (SocketDynamicPlayerAction_20);

	SocketDynamicPlayerAction_21 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_21"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_21, 21);
	AvailableSocketDynamicPlayerAction[21] = (SocketDynamicPlayerAction_21);

	SocketDynamicPlayerAction_22 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_22"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_22, 22);
	AvailableSocketDynamicPlayerAction[22] = (SocketDynamicPlayerAction_22);

	SocketDynamicPlayerAction_23 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_23"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_23, 23);
	AvailableSocketDynamicPlayerAction[23] = (SocketDynamicPlayerAction_23);

	SocketDynamicPlayerAction_24 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_24"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_24, 24);
	AvailableSocketDynamicPlayerAction[24] = (SocketDynamicPlayerAction_24);

	SocketDynamicPlayerAction_25 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_25"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_25, 25);
	AvailableSocketDynamicPlayerAction[25] = (SocketDynamicPlayerAction_25);

	SocketDynamicPlayerAction_26 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SocketPlayerDynamicAction_26"));
	SetDynamicPlayerAction(SocketDynamicPlayerAction_26, 26);
	AvailableSocketDynamicPlayerAction[26] = (SocketDynamicPlayerAction_26);

	for (int32 i = 0; i < AvailableSocketDynamicPlayerAction.Num(); i++)
	{
		UE_LOG(LogTemp, Error, TEXT("%s"),*AvailableSocketDynamicPlayerAction[i]->GetName());
	}

	//ConstructDynamicPlayerAction();

	OriginalPosition = FVector(-526.f, -852.f, -807.f);

	//-----------------------Initializing arrays--------------------------------
	SocketsLockations.Init(FVector(0.f), 16);
	SocketPlayer_Transform.Init(FTransform(), 5);
	SocketEncounter_Transform.Init(FTransform(), 5);
}

// Called when the game starts or when spawned
void ALocationTemplate::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Error, TEXT("WakeUP!"));

	PlayerController = Cast<AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerController->AllLocationsInPlay.Add(this);

	DynamicPlayerActionsSetCollisionOn();

	
	for (int32 i = 0; i < AvailableSocketDynamicPlayerAction.Num(); i++)
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *AvailableSocketDynamicPlayerAction[i]->GetName());
	}

	HorizontalIndex = (GetActorLocation().Y) / (PlayerController->SpawnedLocationOffsetY.Y);
	VerticalIndex = (GetActorLocation().Z) / (PlayerController->SpawnedLocationOffsetZ.Z);

	UE_LOG(LogTemp, Warning, TEXT("%s : Vertical Index = %i , Horizontal Index = %i"), *GetName(), VerticalIndex, HorizontalIndex)
}

// Called every frame
void ALocationTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ALocationTemplate::CreateDoorComponent(FVector Location)
{
	Door = NewObject<UChildActorComponent>(this, ("Door"));
	Door->RegisterComponent();
	Door->SetWorldLocation(Location);
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

void ALocationTemplate::ExcludeFromAvailableEncounter_GoodSockets(UPrimitiveComponent* ComponentToExclude)
{
	AvailableSocketEncounter_Good.RemoveSingle(Cast<UStaticMeshComponent>(ComponentToExclude));
}

void ALocationTemplate::IncludeInAvailableEncounter_GoodSockets(UPrimitiveComponent* ComponentToInclude)
{
	AvailableSocketEncounter_Good.Emplace(Cast<UStaticMeshComponent>(ComponentToInclude));
}

void ALocationTemplate::Connect(bool IsInfinite, UChildActorComponent* Parent, UPaperFlipbookComponent* Connector_0, UChildActorComponent* Child_0, UPaperFlipbookComponent* Connector_1, UChildActorComponent* Child_1, UPaperFlipbookComponent* Connector_2, UChildActorComponent* Child_2, UPaperFlipbookComponent* Connector_3, UChildActorComponent* Child_3, UPaperFlipbookComponent* Connector_4, UChildActorComponent* Child_4)
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
		//Action_Child_0->Deactivate();
	}

	if (Action_Child_1) {
		Action_Child_0->Child = Action_Child_1;
		Action_Child_0->pConnector = Connector_1;
		Action_Child_0->Deactivate();
		//Action_Child_1->Deactivate();
	}
	else if(IsInfinite){
		Action_Child_0->FirstActionInChain = ParentAction;
		Action_Child_0->Deactivate();
		return;
	}
	else {
		Action_Child_0->Deactivate();
		return;
	}

	if (Action_Child_2) {
		Action_Child_1->Child = Action_Child_2;
		Action_Child_1->pConnector = Connector_2;
		Action_Child_1->Deactivate();
		//Action_Child_2->Deactivate(); 
	}
	else if (IsInfinite) {
		Action_Child_1->FirstActionInChain = ParentAction;
		Action_Child_1->Deactivate();
		return;
	}
	else {
		Action_Child_1->Deactivate();
		return;
	}

	if (Action_Child_3) {
		Action_Child_2->Child = Action_Child_3;
		Action_Child_2->pConnector = Connector_3;
		Action_Child_2->Deactivate();
		//Action_Child_3->Deactivate();
	}
	else if (IsInfinite) {
		Action_Child_2->FirstActionInChain = ParentAction;
		Action_Child_2->Deactivate();
		return;
	}
	else {
		Action_Child_2->Deactivate();
		return;
	}
	

	if (Action_Child_4) {
		Action_Child_3->Child = Action_Child_4;
		Action_Child_3->pConnector = Connector_4;
		Action_Child_3->Deactivate();
		if (IsInfinite) {
			Action_Child_4->FirstActionInChain = ParentAction;
		}
		Action_Child_4->Deactivate();
	}
	else if (IsInfinite) {
		Action_Child_3->FirstActionInChain = ParentAction;
		Action_Child_3->Deactivate();
		return;
	}
	else {
		Action_Child_3->Deactivate();
		return;
	}

}

void ALocationTemplate::MakeActionNexus(UChildActorComponent* Parent, UPaperFlipbookComponent* Connector_0, UChildActorComponent* Child_0, UPaperFlipbookComponent* Connector_1, UChildActorComponent* Child_1, UPaperFlipbookComponent* Connector_2, UChildActorComponent* Child_2)
{
	auto NexusAction = Cast<AAction>(Parent->GetChildActor());

	AAction* Action_Child_0 = nullptr;
	if (Child_0) { Action_Child_0 = Cast<AAction>(Child_0->GetChildActor()); }

	AAction* Action_Child_1 = nullptr;
	if (Child_1) { Action_Child_1 = Cast<AAction>(Child_1->GetChildActor()); }

	AAction* Action_Child_2 = nullptr;
	if (Child_2) { Action_Child_2 = Cast<AAction>(Child_2->GetChildActor()); }

	if (Action_Child_0) { Action_Child_0->pConnector = Connector_0; NexusAction->ChildrenOfNexus.Add(Action_Child_0); Action_Child_0->Nexus = NexusAction;}
	if (Action_Child_1) { Action_Child_1->pConnector = Connector_1; NexusAction->ChildrenOfNexus.Add(Action_Child_1); Action_Child_1->Nexus = NexusAction;}
	if (Action_Child_2) { Action_Child_2->pConnector = Connector_2; NexusAction->ChildrenOfNexus.Add(Action_Child_2); Action_Child_2->Nexus = NexusAction;}

	NexusAction->Deactivate();
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
	PlayerController->EntangleActionWithActor(Action, InteractiveLocationDecoration);
}
/*
void ALocationTemplate::ExcludeFromAvailableDynamicActionSocket(UPrimitiveComponent* ComponentToExclude)
{
	AvailableSocketDynamicAction.RemoveSingle(Cast<UStaticMeshComponent>(ComponentToExclude));
}

void ALocationTemplate::IncludeInAvailableDynamicActionSocket(UPrimitiveComponent* ComponentToInclude)
{
	AvailableSocketDynamicAction.Emplace(Cast<UStaticMeshComponent>(ComponentToInclude));
}
*/
void ALocationTemplate::ExcludeFromAvailableDynamicPlayerActionSocket(UPrimitiveComponent* ComponentToExclude)
{
	//UE_LOG(LogTemp, Error, TEXT("LOCATION WHILE EXCLUDING:"));
	//for (int32 i = 0; i < AvailableSocketDynamicPlayerAction.Num(); i++)
	//{
//		UE_LOG(LogTemp, Error, TEXT("%s"), *AvailableSocketDynamicPlayerAction[i]->GetName());
	//}

	AvailableSocketDynamicPlayerAction.RemoveSingle(Cast<UStaticMeshComponent>(ComponentToExclude));
}

void ALocationTemplate::IncludeInAvailableDynamicPlayerActionSocket(UPrimitiveComponent* ComponentToInclude)
{
	AvailableSocketDynamicPlayerAction.Add(Cast<UStaticMeshComponent>(ComponentToInclude));
}

void ALocationTemplate::CreateDynamicAction(TSubclassOf<AAction> ActionClass, ALocationTemplate* LocationInstigator)// UChildActorComponent* ActorToEntangleWith, ALocationTemplate* LocationInstigator)
{
	
	for (int32 i = 0; i < DynamicAction.Num(); i++)
	{
		if (!DynamicAction[i]) {
			DynamicAction[i] = NewObject<UChildActorComponent>(this, ("Action_Dynamic" + i));

			DynamicAction[i]->RegisterComponent();
			DynamicAction[i]->SetChildActorClass(ActionClass);
			DynamicAction[i]->SetWorldLocation(AvailableSocketDynamicPlayerAction.Last()->GetComponentLocation());
			//Cast<AAction>(DynamicAction[i]->GetChildActor())->EntangledDeadEncounter = EntangledDead;

			//EntangledDead->CreatedAction = DynamicAction[i];
			AAction* CreatedAction = nullptr;

			EntangleActionWithActor(DynamicAction[i], Door);
			CreatedAction = Cast<AAction>(DynamicAction[i]->GetChildActor());

			CreatedAction->ActionPointsRequired = 0;
			if (LocationInstigator) {
				CreatedAction->LocationArrowPointsTo = LocationInstigator;
			}
			CreatedAction->ActionType = EActionType::Arrow_Move;
			CreatedAction->UpdateArrowActionVisual();
			return;
		}
	}
}

void ALocationTemplate::SetDynamicPlayerAction(UStaticMeshComponent* ComponentToSet, int32 Index)
{
	int32 NumberOfActions = 27;

	//SocketDynamicPlayerAction.Init(nullptr, NumberOfActions);

	FCollisionResponseContainer CollisionResponseContainer;
	CollisionResponseContainer.SetAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionResponseContainer.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	int32 NumberOfSocketsPerRow = 9;

	FVector Scale = FVector(0.4f, 0.4f, 0.4f);
	float RowInterval = 200;

	FVector OriginalPosition = FVector(-600.f, -852.f, -250.f);
	FVector DynamicPlayerActionIncrement = FVector(0.f, 220.f, 0.f);

		int32 RowNumber = Index / int32(NumberOfSocketsPerRow);

		DynamicPlayerActionIncrement = FVector(0.f, 220.f, 0.f);
		OriginalPosition = FVector(-600.f, -852.f, (-250.f - (RowInterval * RowNumber)));

		ComponentToSet->SetupAttachment(pRootComponent);
		ComponentToSet->SetCollisionResponseToChannels(CollisionResponseContainer);
		ComponentToSet->SetRelativeScale3D(Scale);
		ComponentToSet->SetRelativeLocation(OriginalPosition + ((Index - (RowNumber*NumberOfSocketsPerRow)) * DynamicPlayerActionIncrement));
		ComponentToSet->SetRelativeRotation(FRotator(0.f, 0.f, 0.f).Quaternion());
		ComponentToSet->SetHiddenInGame(true);
		ComponentToSet->SetMobility(EComponentMobility::Movable);
		ComponentToSet->CastShadow = false;
		ComponentToSet->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, (TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"))));
		ComponentToSet->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		AvailableSocketDynamicPlayerAction.Add(ComponentToSet);
}

void ALocationTemplate::DynamicPlayerActionsSetCollisionOn() {

	SocketDynamicPlayerAction_0->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_1->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_2->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_3->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_4->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_5->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_6->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_7->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_8->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_9->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_10->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_11->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_12->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_13->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_14->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_15->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_16->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_17->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_18->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_19->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_20->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_21->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_22->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_23->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_24->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_25->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SocketDynamicPlayerAction_26->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}