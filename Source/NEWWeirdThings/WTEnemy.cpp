// Fill out your copyright notice in the Description page of Project Settings.

#include "WTEnemy.h"
#include "QuitManagement.h"
#include "AttackDefenseActor.h"
#include "AttackDefenseComponent.h"
#include "ItemTemplate.h"
///#include "Blueprint/UserWidget.h"
#include "Math/Vector2D.h"
///#include "Runtime/UMG/Public/Components/WidgetComponent.h"


// Sets default values
AWTEnemy::AWTEnemy()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//----------------------Creating Root Component--------------------------
	pRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(pRootComponent);

	//----------------------Creating Health Widget Component-----------------
	///pHealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetHealth"));
	///pHealthWidget->AttachTo(pRootComponent);

	//Setting up relative transform
	FTransform HealthWidgetRelativeTransform;
	HealthWidgetRelativeTransform.SetLocation(FVector(0.f, -140.f, 320.f));
	HealthWidgetRelativeTransform.SetRotation(FRotator(0.0f, 180.0f, 0.0f).Quaternion());
	HealthWidgetRelativeTransform.SetScale3D(FVector(0.15f, 0.15f, 0.15f));

	///pHealthWidget->SetRelativeTransform(HealthWidgetRelativeTransform);
	///pHealthWidget->SetDrawSize(FVector2D(2400.f, 2000.f));

	//----------------------Creating Attack/Defense Component--------------------------
	pAttackDefenseComponent = CreateDefaultSubobject<UAttackDefenseComponent>(TEXT("AttackDefenseComponent"));

	//----------------------Creating Attack/Defense Component--------------------------
	pQuitManagementComponent = CreateDefaultSubobject<UQuitManagement>(TEXT("QuitManagementComponent"));

	//-----------------------Initializing arrays--------------------------------
	Loot.Init(nullptr, 3);
	LootClassesToSpawn.Init(nullptr, 3);
}

// Called when the game starts or when spawned
void AWTEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitializeLoot();

}

// Called every frame
void AWTEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWTEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWTEnemy::LooseHealth()
{

	HealthPoints--;
	if (HealthPoints < 1)
	{

		Destroy();
	}
}

void AWTEnemy::LooseHealth(AAttackDefenseActor* AttackDefenseActorRef)
{
	if (AttackDefenseActorToIgnore == AttackDefenseActorRef) { return; }
	HealthPoints--;
	AttackDefenseActorToIgnore = AttackDefenseActorRef;
}

void AWTEnemy::InitializeLoot()
{
	for (int32 i = 0; i < LootClassesToSpawn.Num(); i++)
	{
		Loot[i] = GetWorld()->SpawnActor<AItemTemplate>(LootClassesToSpawn[i]);
	}
}
/*
void AWTEnemy::CreateInfoWidget(UUserWidget* WidgetToCreate, FVector2D WidgetOffset)
{

	WidgetToCreate->AddToViewport();

	FVector2D ProjectedActorLocation;
	GetWorld()->GetFirstPlayerController()->ProjectWorldLocationToScreen(GetActorLocation(), ProjectedActorLocation);
	WidgetToCreate->SetPositionInViewport(ProjectedActorLocation + WidgetOffset);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *ProjectedActorLocation.ToString())

		WidgetToCreate->SetDesiredSizeInViewport(FVector2D(100.f, 100.f));

	FWidgetTransform WidgetTransform;
	WidgetTransform.Scale.X = 0.2f;
	WidgetTransform.Scale.Y = 0.2f;
	WidgetToCreate->SetRenderTransform(WidgetTransform);
}*/