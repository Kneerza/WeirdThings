// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveLocationDecoration.h"
#include "Action.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"


// Sets default values
AInteractiveLocationDecoration::AInteractiveLocationDecoration()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//----------------------Creating Root Component--------------------------
	pRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(pRootComponent);

	//----------------------Creating Sprite component_0----------
	InteractiveLocationDecoration_SpriteComponent_0 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite_0"));
	InteractiveLocationDecoration_SpriteComponent_0->SetupAttachment(pRootComponent);

	//Setting up relative transform
	FTransform InteractiveLocationDecoration_SpriteComponent_0_RelativeTransform;
	InteractiveLocationDecoration_SpriteComponent_0_RelativeTransform.SetLocation(FVector(0.f, 0.f, 0.f));
	InteractiveLocationDecoration_SpriteComponent_0_RelativeTransform.SetRotation(FRotator(0.f, 90.f, 0.f).Quaternion());
	InteractiveLocationDecoration_SpriteComponent_0_RelativeTransform.SetScale3D(FVector(0.4f, 0.4f, 0.4f));

	InteractiveLocationDecoration_SpriteComponent_0->SetRelativeTransform(InteractiveLocationDecoration_SpriteComponent_0_RelativeTransform);

	FCollisionResponseContainer CollisionArrowResponseContainer;
	CollisionArrowResponseContainer.SetAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionArrowResponseContainer.SetResponse(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	CollisionArrowResponseContainer.SetResponse(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	InteractiveLocationDecoration_SpriteComponent_0->SetCollisionResponseToChannels(CollisionArrowResponseContainer);
	InteractiveLocationDecoration_SpriteComponent_0->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);

	//----------------------Creating Sprite component_1----------
	InteractiveLocationDecoration_SpriteComponent_1 = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite_1"));
	InteractiveLocationDecoration_SpriteComponent_1->SetupAttachment(pRootComponent);

	//Setting up relative transform
	FTransform InteractiveLocationDecoration_SpriteComponent_1_RelativeTransform;
	InteractiveLocationDecoration_SpriteComponent_1_RelativeTransform.SetLocation(FVector(0.f, 0.f, 0.f));
	InteractiveLocationDecoration_SpriteComponent_1_RelativeTransform.SetRotation(FRotator(0.f, 90.f, 0.f).Quaternion());
	InteractiveLocationDecoration_SpriteComponent_1_RelativeTransform.SetScale3D(FVector(0.4f, 0.4f, 0.4f));

	InteractiveLocationDecoration_SpriteComponent_1->SetRelativeTransform(InteractiveLocationDecoration_SpriteComponent_1_RelativeTransform);
	InteractiveLocationDecoration_SpriteComponent_1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractiveLocationDecoration_SpriteComponent_1->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void AInteractiveLocationDecoration::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AInteractiveLocationDecoration::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveLocationDecoration::ChangeState_InteractiveLocationDecoration()
{
	if (InteractiveLocationDecoration_SpriteComponent_1->GetSprite())
	{
		InteractiveLocationDecoration_SpriteComponent_0->DestroyComponent();
		InteractiveLocationDecoration_SpriteComponent_1->SetHiddenInGame(false);
	}
	else {
		InteractiveLocationDecoration_SpriteComponent_0->SetSpriteColor(FLinearColor(0.03f, 0.03f, 0.03f, 1));
		InteractiveLocationDecoration_SpriteComponent_0->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (IsLastStateActive)
	{
		InteractiveLocationDecoration_SpriteComponent_1->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	return;
}

void AInteractiveLocationDecoration::Diactivate_InteractiveLocationDecoration()
{
	InteractiveLocationDecoration_SpriteComponent_0->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}