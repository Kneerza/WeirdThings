// Fill out your copyright notice in the Description page of Project Settings.

#include "Encounter_Dead.h"
#include "PaperFlipbookComponent.h"
#include "Engine/World.h"
#include "LocationTemplate.h"
#include "QuitManagement.h"
#include "WTPlayerCharacter.h"
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

	if (GetParentActor()){ CurrentLocation = Cast<ALocationTemplate>(GetParentActor()); }

	// TODO move to LocationTemplate as a Function, that will be called here
	CreateAction();
}

void AEncounter_Dead::SetAwakened(bool IsAwakened)
{
	if (IsAwakened) {
		IsAwake = true;
		AwakenedDeadFlipbookComponent->SetHiddenInGame(false);
		AwakenedDeadFlipbookComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SleepingDeadFlipbookComponent->SetHiddenInGame(true);
		FindPlayerToAttack();
	}
	else {
		IsAwake = false;
		AwakenedDeadFlipbookComponent->SetHiddenInGame(true);
		AwakenedDeadFlipbookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SleepingDeadFlipbookComponent->SetHiddenInGame(false);
	}
	return;
}

void AEncounter_Dead::FindPlayerToAttack()
{
	//UE_LOG(LogTemp, Error, TEXT("Dead Is Attacking"))
	auto PlayerCharactersInPlay = PlayerController->PlayerCharacters;
	UE_LOG(LogTemp, Error, TEXT("Current Location: %s"),*CurrentLocation->GetName())
	UE_LOG(LogTemp, Error, TEXT("Looking for player to attack"))
	for (int32 i = 0; i < PlayerCharactersInPlay.Num(); i++)
	{
		if (PlayerCharactersInPlay[i]->CurrentLocation == CurrentLocation)
		{
			PlayerController->AttackDefenseEvent(this, PlayerCharactersInPlay[i]);
			//UE_LOG(LogTemp, Error, TEXT("Attack-defense happened"))
			return;
		}
	}
}

void AEncounter_Dead::CreateAction()
{
	if (GetParentActor() && ActionClassToSpawn) {
		UE_LOG(LogTemp, Warning, TEXT("DEAD IS CREATING ACTION"))
			//	CurrentLocation = Cast<ALocationTemplate>(GetParentActor());
			CreatedAction = NewObject<UChildActorComponent>(this, ("Action_Dead"));
		CreatedAction->RegisterComponent();
		CreatedAction->SetChildActorClass(ActionClassToSpawn);
		CreatedAction->SetWorldLocation(CurrentLocation->AvailableSocketDynamicAction[0]->GetComponentLocation());
	}
}

void AEncounter_Dead::Move()
{
	if (CurrentLocation->HorizontalIndex > 0)
	{
		for (int32 i = 0; i < PlayerController->AllLocationsInPlay.Num(); i++)
		{
			if ((CurrentLocation->VerticalIndex == PlayerController->AllLocationsInPlay[i]->VerticalIndex) && ((CurrentLocation->HorizontalIndex - PlayerController->AllLocationsInPlay[i]->HorizontalIndex) == 1))
			{
				CurrentLocation = PlayerController->AllLocationsInPlay[i];
				CreatedAction->UnregisterComponent();
				DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				AttachToActor(CurrentLocation, FAttachmentTransformRules::KeepWorldTransform);
				SetActorLocation(CurrentLocation->AvailableSocketEncounter[0]->GetComponentLocation());
				CreateAction();
				return;
			}
		}
	}
	
	//CreatedAction->UnregisterComponent();
	//DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}