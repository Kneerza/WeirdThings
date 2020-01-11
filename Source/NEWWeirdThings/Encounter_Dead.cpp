// Fill out your copyright notice in the Description page of Project Settings.

#include "Encounter_Dead.h"
#include "PaperFlipbookComponent.h"
#include "Engine/World.h"
#include "LocationTemplate.h"
#include "WeirdThingsPlayerController.h"

AEncounter_Dead::AEncounter_Dead()
{
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
}

void AEncounter_Dead::BeginPlay()
{
	Super::BeginPlay();

	(Cast<AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController())->Encounter_DeadsInPlay).Add(this);

	// TODO move to LocationTemplate as a Function, that will be called here
	if (GetParentActor() && ActionClassToSpawn) {
		UE_LOG(LogTemp, Warning, TEXT("DEAD IS CREATING ACTION"))
			CurrentLocation = Cast<ALocationTemplate>(GetParentActor());
		auto NewComponent = NewObject<UChildActorComponent>(this, ("Action_Dead"));

		NewComponent->SetChildActorClass(ActionClassToSpawn);
	}
}

void AEncounter_Dead::SetAwakened(bool IsAwakened)
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

