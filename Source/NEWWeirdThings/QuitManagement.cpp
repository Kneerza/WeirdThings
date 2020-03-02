// Fill out your copyright notice in the Description page of Project Settings.

#include "QuitManagement.h"
#include "LocationTemplate.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "WeirdThingsPlayerController.h"
#include "WTEnemy.h"
#include "Encounter.h"
#include "WTPlayerCharacter.h"


// Sets default values for this component's properties
UQuitManagement::UQuitManagement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	QuitType.Init(EQuitType::No_QuitType, 3);
}


// Called when the game starts
void UQuitManagement::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Called every frame
void UQuitManagement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuitManagement::CheckQuitConditions()
{
	auto PlayerController = Cast<AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController());
	auto CurrentlySelectedCharacter = PlayerController->pSelectedCharacter;
	TArray<AItemTemplate*> Backpack = { nullptr };
	if (CurrentlySelectedCharacter) {
		 Backpack = CurrentlySelectedCharacter->Backpack;
	}
	switch (QuitType[0])
	{
	case EQuitType::TooManyCharacters:

		if ((5 - (Cast<AWTEnemy>(GetOwner())->CurrentLocation->AvailableSocketPlayer.Num())) >= Modifier)
		{
			OwnerQuits();
		}
		break;

	case EQuitType::Food:

		if (PlayerController->pSelectedCharacter->Food >= Modifier)
		{
			PlayerController->pSelectedCharacter->Food -= Modifier;
			OwnerQuits();
		}
		break;

	case EQuitType::Items:
		if (!Backpack[0]) { return; }
		for (int32 i = (Backpack.Num() - 1); i >= 0; i--)
		{
			if (Backpack[i])
			{
				(Cast<AWTEnemy>(GetOwner()))->Loot.Insert(Backpack[i], 2);
				Backpack.RemoveAt(i, 1, false);
				PlayerController->pSelectedCharacter->Backpack = Backpack;
				return;
			}
		}
		break;

	case EQuitType::TimeOfDay_Morning:

		if (PlayerController->CurrentTimeOfDay == ETimeOfDay::Morning)
		{
			OwnerQuits();
		}
		break;

	case EQuitType::TimeOfDay_Noon:

		if (PlayerController->CurrentTimeOfDay == ETimeOfDay::Noon)
		{
			OwnerQuits();
		}
		break;

	case EQuitType::TimeOfDay_Evening:

		if (PlayerController->CurrentTimeOfDay == ETimeOfDay::Evening)
		{
			OwnerQuits();
		}
		break;

	case EQuitType::TimeOfDay_Night:

		if (PlayerController->CurrentTimeOfDay == ETimeOfDay::Night)
		{
			OwnerQuits();
		}
		break;

	case EQuitType::Fire:

		if ((Cast<AEncounter>(GetOwner()))->CurrentLocation->AvailableSocketCampFire.Num()<1)
		{
			OwnerQuits();
		}
		break;

	default:

		break;
	}
}

void UQuitManagement::OwnerQuits()
{
	GetOwner()->Destroy();
}