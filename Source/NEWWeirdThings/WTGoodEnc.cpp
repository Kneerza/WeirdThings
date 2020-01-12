// Fill out your copyright notice in the Description page of Project Settings.

#include "WTGoodEnc.h"
#include "AttackDefenseActor.h"
#include "AttackDefenseComponent.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"

// TODO Delete class

// Sets default values
AWTGoodEnc::AWTGoodEnc()
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

}

// Called when the game starts or when spawned
void AWTGoodEnc::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWTGoodEnc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AWTGoodEnc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AWTGoodEnc::LooseHealth()
{
	HealthPoints--;
	if (HealthPoints < 1)
	{

		Destroy();
	}
}

void AWTGoodEnc::LooseHealth(AAttackDefenseActor* AttackDefenseActorRef)
{
	if (AttackDefenseActorToIgnore == AttackDefenseActorRef) { return; }
	HealthPoints--;
	AttackDefenseActorToIgnore = AttackDefenseActorRef;
}

