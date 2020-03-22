// Fill out your copyright notice in the Description page of Project Settings.

#include "QuitManagement.h"
#include "LocationTemplate.h"
#include "Encounter_Dead.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "WeirdThingsPlayerController.h"
#include "WTEnemy.h"
#include "Encounter.h"
#include "Encounter_Bad.h"
#include "WTPlayerCharacter.h"


// Sets default values for this component's properties
UQuitManagement::UQuitManagement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	QuitType.Init(EQuitType::No_QuitType, 3);
	QuitTypeModifier.Init(1,3);
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
	for (int32 i = 0; i < QuitType.Num(); i++)
	{
		switch (QuitType[i])
		{
		case EQuitType::TooManyCharacters:

			if ((5 - (Cast<AWTEnemy>(GetOwner())->CurrentLocation->AvailableSocketPlayer.Num())) >= QuitTypeModifier[i])
			{
				OwnerQuits();
			}
			break;

		case EQuitType::Food:
			/*
			if (PlayerController->pSelectedCharacter->Food >= QuitTypeModifier[i])
			{
				PlayerController->pSelectedCharacter->Food -= QuitTypeModifier[i];
				OwnerQuits();
			}
			*/
			break;

		case EQuitType::Items:
			/*
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
			*/
			break;

		case EQuitType::TimeOfDay_Morning:

			if (PlayerController->CurrentTimeOfDay == ETimeOfDay::Morning)
			{
				OwnerQuits();
				return;
			}
			break;

		case EQuitType::TimeOfDay_Noon:

			if (PlayerController->CurrentTimeOfDay == ETimeOfDay::Noon)
			{
				OwnerQuits();
				return;
			}
			break;

		case EQuitType::TimeOfDay_Evening:

			if (PlayerController->CurrentTimeOfDay == ETimeOfDay::Evening)
			{
				OwnerQuits();
				return;
			}
			break;

		case EQuitType::TimeOfDay_Night:

			if (PlayerController->CurrentTimeOfDay == ETimeOfDay::Night)
			{
				OwnerQuits();
				return;
			}
			break;

		case EQuitType::Fire:

			if (Cast<ALocationTemplate>((Cast<AEncounter>(GetOwner()))->CurrentLocation)->AvailableSocketCampFire.Num() < 1)
			{
				OwnerQuits();
				return;
			}
			break;

		case EQuitType::DeadOnLocation:

			UE_LOG(LogTemp, Error, TEXT("Checking if dead is on location"))

				for (int32 i = 0; i < PlayerController->Encounter_DeadsInPlay.Num(); i++)
				{
					if (PlayerController->Encounter_DeadsInPlay[i]) {
						UE_LOG(LogTemp, Error, TEXT("%s"), *Cast<AEncounter>(PlayerController->Encounter_DeadsInPlay[i])->CurrentLocation->GetName())
					}

					if (Cast<AEncounter>(GetOwner())->CurrentLocation) {
						UE_LOG(LogTemp, Error, TEXT("%s"), *Cast<AEncounter>(GetOwner())->CurrentLocation->GetName())
					}

					if (Cast<AEncounter>(PlayerController->Encounter_DeadsInPlay[i])->CurrentLocation == Cast<AEncounter>(GetOwner())->CurrentLocation)
					{
						UE_LOG(LogTemp, Error, TEXT("Dead is spotted"))
							OwnerQuits();
						return;
					}
				}
			break;

		case EQuitType::BadEncOnLocation:

			UE_LOG(LogTemp, Error, TEXT("Checking if Enc_Bad is on location"))

				for (int32 i = 0; i < PlayerController->Encounter_BadInPlay.Num(); i++)
				{

					if (Cast<AEncounter>(PlayerController->Encounter_BadInPlay[i])->CurrentLocation == Cast<AEncounter>(GetOwner())->CurrentLocation)
					{
						UE_LOG(LogTemp, Error, TEXT("Enc_Bad is spotted"))
							OwnerQuits();
						return;
					}
				}
			break;

		default:

			break;
		}
	}
}

void UQuitManagement::OwnerQuits()
{
	GetOwner()->Destroy();
}