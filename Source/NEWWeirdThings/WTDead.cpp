// Fill out your copyright notice in the Description page of Project Settings.

#include "WTDead.h"
#include "LocationTemplate.h"
#include "Components/ChildActorComponent.h"
#include "Action.h"
#include "QuitManagement.h"
#include "AttackDefenseActor.h"
#include "AttackDefenseComponent.h"
#include "WeirdThingsPlayerController.h"
#include "ItemTemplate.h"
#include "PaperFlipbookComponent.h"
#include "Blueprint/UserWidget.h"
#include "Math/Vector2D.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"

// TODO Delete class

// Sets default values
AWTDead::AWTDead()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//----------------------Creating Root Component--------------------------
	pRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(pRootComponent);

	//----------------------Creating Health Widget Component-----------------
	pHealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetHealth"));
	pHealthWidget->AttachTo(pRootComponent);

	//Setting up relative transform
	FTransform HealthWidgetRelativeTransform;
	HealthWidgetRelativeTransform.SetLocation(FVector(0.f, -140.f, 320.f));
	HealthWidgetRelativeTransform.SetRotation(FRotator(0.0f, 180.0f, 0.0f).Quaternion());
	HealthWidgetRelativeTransform.SetScale3D(FVector(0.15f, 0.15f, 0.15f));

	pHealthWidget->SetRelativeTransform(HealthWidgetRelativeTransform);
	pHealthWidget->SetDrawSize(FVector2D(2400.f, 2000.f));

	//----------------------Creating Attack/Defense Component--------------------------
	pAttackDefenseComponent = CreateDefaultSubobject<UAttackDefenseComponent>(TEXT("AttackDefenseComponent"));

	//----------------------Creating Attack/Defense Component--------------------------
	pQuitManagementComponent = CreateDefaultSubobject<UQuitManagement>(TEXT("QuitManagementComponent"));

	//----------------------Creating AwakenedDeadFlipbookComponent----------
	AwakenedDeadFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("AwakenedDead"));
	AwakenedDeadFlipbookComponent->SetupAttachment(pRootComponent);

	//Setting up relative transform
	FTransform AwakenedDeadFlipbookComponentRelativeTransform;
	AwakenedDeadFlipbookComponentRelativeTransform.SetLocation(FVector(0.f, 5.f, 85.f));
	AwakenedDeadFlipbookComponentRelativeTransform.SetRotation(FRotator(0.f, 90.f, 0.f).Quaternion());
	AwakenedDeadFlipbookComponentRelativeTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

	AwakenedDeadFlipbookComponent->SetRelativeTransform(AwakenedDeadFlipbookComponentRelativeTransform);

	//Setting up collision
	FCollisionResponseContainer AwakenedDeadFlipbookComponentResponseContainer;
	AwakenedDeadFlipbookComponentResponseContainer.SetAllChannels(ECollisionResponse::ECR_Overlap);
	AwakenedDeadFlipbookComponentResponseContainer.SetResponse(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	AwakenedDeadFlipbookComponentResponseContainer.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	AwakenedDeadFlipbookComponent->SetCollisionResponseToChannels(AwakenedDeadFlipbookComponentResponseContainer);
	AwakenedDeadFlipbookComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

	//Setting up visibility
	AwakenedDeadFlipbookComponent->SetHiddenInGame(true);

	//----------------------Creating SleepingDeadFlipbookComponent----------
	SleepingDeadFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("SleepingDead"));
	SleepingDeadFlipbookComponent->SetupAttachment(pRootComponent);

	//Setting up relative transform
	FTransform SleepingDeadFlipbookComponentRelativeTransform;
	SleepingDeadFlipbookComponentRelativeTransform.SetLocation(FVector(0.f, 5.f, 85.f));
	SleepingDeadFlipbookComponentRelativeTransform.SetRotation(FRotator(0.f, 90.f, 0.f).Quaternion());
	SleepingDeadFlipbookComponentRelativeTransform.SetScale3D(FVector(1.f, 1.f, 1.f));

	SleepingDeadFlipbookComponent->SetRelativeTransform(SleepingDeadFlipbookComponentRelativeTransform);

	//Setting up collision
	FCollisionResponseContainer SleepingDeadFlipbookComponentResponseContainer;
	SleepingDeadFlipbookComponentResponseContainer.SetAllChannels(ECollisionResponse::ECR_Overlap);
	SleepingDeadFlipbookComponentResponseContainer.SetResponse(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	SleepingDeadFlipbookComponentResponseContainer.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SleepingDeadFlipbookComponent->SetCollisionResponseToChannels(SleepingDeadFlipbookComponentResponseContainer);
	SleepingDeadFlipbookComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

	//-----------------------Initializing arrays--------------------------------
	Loot.Init(nullptr, 3);
	LootClassesToSpawn.Init(nullptr, 3);
}

// Called when the game starts or when spawned
void AWTDead::BeginPlay()
{
	Super::BeginPlay();

	InitializeLoot();

	//(Cast<AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController())->AllDeadsInPlay).Add(this);

	// TODO move to LocationTemplate as a Function, that will be called here
	if (GetParentActor() && ActionClassToSpawn) {
		UE_LOG(LogTemp, Warning, TEXT("DEAD IS CREATING ACTION"))
			CurrentLocation = Cast<ALocationTemplate>(GetParentActor());
		auto NewComponent = NewObject<UChildActorComponent>(this, ("Action_Dead"));

		NewComponent->SetChildActorClass(ActionClassToSpawn);
	}
}

// Called every frame
void AWTDead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWTDead::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWTDead::LooseHealth()
{

	HealthPoints--;
	if (HealthPoints < 1)
	{

		Destroy();
	}
}

void AWTDead::LooseHealth(AAttackDefenseActor* AttackDefenseActorRef)
{
	if (AttackDefenseActorToIgnore == AttackDefenseActorRef) { return; }
	HealthPoints--;
	AttackDefenseActorToIgnore = AttackDefenseActorRef;
}

void AWTDead::InitializeLoot()
{
	for (int32 i = 0; i < LootClassesToSpawn.Num(); i++)
	{
		Loot[i] = GetWorld()->SpawnActor<AItemTemplate>(LootClassesToSpawn[i]);
	}
}

void AWTDead::CreateInfoWidget(UUserWidget* WidgetToCreate, FVector2D WidgetOffset)
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
}


void AWTDead::SetAwakened(bool IsAwakened)
{
	if (IsAwakened) {
		AwakenedDeadFlipbookComponent->SetHiddenInGame(false);
		SleepingDeadFlipbookComponent->SetHiddenInGame(true);
	}
	else {
		AwakenedDeadFlipbookComponent->SetHiddenInGame(true);
		SleepingDeadFlipbookComponent->SetHiddenInGame(false);
	}
	return;
}