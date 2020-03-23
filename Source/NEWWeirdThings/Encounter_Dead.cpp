// Fill out your copyright notice in the Description page of Project Settings.

#include "Encounter_Dead.h"
#include "PaperFlipbookComponent.h"
#include "Engine/World.h"
#include "QuitManagement.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/Core/Public/Math/Rotator.h"
#include "Runtime/Core/Public/Math/TransformNonVectorized.h"
#include "Runtime/Core/Public/Math/Quat.h"
#include "Components/SceneComponent.h"
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
	AwakenedDeadFlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
	//SleepingDeadFlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEncounter_Dead::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = (Cast<AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController()));
	PlayerController->Encounter_DeadsInPlay.Add(this);

	if (GetParentActor()){ 
		CurrentLocation = GetParentActor();
		UE_LOG(LogTemp, Error, TEXT("Current Location: %s"), *CurrentLocation->GetName())
	}

	// TODO move to LocationTemplate as a Function, that will be called here
	CreateDynamicAction();
	if ((PlayerController->CurrentTimeOfDay == ETimeOfDay::Evening) || (PlayerController->CurrentTimeOfDay == ETimeOfDay::Night))
	{
		SetAwakened(true);
	}
}

void AEncounter_Dead::SetAwakened(bool IsAwakened)
{
	if (IsAwakened) {
		IsAwake = true;
		AwakenedDeadFlipbookComponent->SetHiddenInGame(false);
		AwakenedDeadFlipbookComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SleepingDeadFlipbookComponent->SetHiddenInGame(true);
		PlayerController->Encounter_DeadLookForPlayerToAttack(this);
	}
	else {
		IsAwake = false;
		AwakenedDeadFlipbookComponent->SetHiddenInGame(true);
		AwakenedDeadFlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SleepingDeadFlipbookComponent->SetHiddenInGame(false);
	}
	return;
}

void AEncounter_Dead::CreateDynamicAction()
{
	if (IsOnPlot) { return; }
	if (CreatedAction) { return; }
	if (GetParentActor() && ActionClassToSpawn ) {
		UE_LOG(LogTemp, Warning, TEXT("DEAD IS CREATING ACTION"))
			PlayerController->CreateDynamicAction(CurrentLocation, ActionClassToSpawn, this);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Encounter Dead has no ActionClassToSpawn"))
	}
}

void AEncounter_Dead::SetAction(UChildActorComponent* ActionToSet)
{
	CreatedAction = ActionToSet;
}

void AEncounter_Dead::Deactivate()
{
	Super::Deactivate();

	if (PlayerController->Encounter_DeadsInPlay.Contains(this)) {
		PlayerController->Encounter_DeadsInPlay.Remove(Cast<AEncounter_Dead>(this));
	}
}

void AEncounter_Dead::SetIsHovered(bool IsHovered)
{
	if (IsHovered)
	{
		PlayerController->SetCurrentlyHoveredByMouseEncounter_Dead(true, this);
	}
	else
	{
		PlayerController->SetCurrentlyHoveredByMouseEncounter_Dead(false, nullptr);
	}
}