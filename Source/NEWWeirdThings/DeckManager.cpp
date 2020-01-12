// Fill out your copyright notice in the Description page of Project Settings.

#include "DeckManager.h"
#include "Encounter_Good.h"
#include "Encounter_Bad.h"
#include "Encounter_Dead.h"
#include "ItemTemplate.h"
#include "LocationTemplate.h"
#include "Runtime/Engine/Classes/Engine/World.h"

// Sets default values for this component's properties
UDeckManager::UDeckManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

}

// Called when the game starts
void UDeckManager::BeginPlay()
{
	Super::BeginPlay();

	// ...

	DistributeItemClassesByValue();
}

// Called every frame
void UDeckManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TSubclassOf<ALocationTemplate> UDeckManager::DrawLocationFromDeck()
{
	TSubclassOf<ALocationTemplate> LocationToDraw = nullptr;

	auto IndexToDraw = FMath::RandRange(0, AvailableLocationsClasses.Num() - 1);

	if (AvailableLocationsClasses.IsValidIndex(IndexToDraw))
	{
		LocationToDraw = AvailableLocationsClasses[IndexToDraw];
		AvailableLocationsClasses.RemoveSingle(LocationToDraw);
	}

	return LocationToDraw;
}

TSubclassOf<ALocationTemplate> UDeckManager::DrawPlotLocationFromDeck()
{
	TSubclassOf<ALocationTemplate> LocationToDraw = nullptr;

	if (AvailablePlotLocationsClasses.IsValidIndex(0))
	{
		LocationToDraw = AvailablePlotLocationsClasses[0];
		AvailablePlotLocationsClasses.RemoveSingle(LocationToDraw);
	}

	return LocationToDraw;
}

TSubclassOf<AEncounter_Bad> UDeckManager::DrawEnemyFromDeck()
{
	TSubclassOf<AEncounter_Bad> EnemyToDraw = nullptr;

	auto IndexToDraw = FMath::RandRange(0, AvailableEnemyClasses.Num() - 1);

	if (AvailableEnemyClasses.IsValidIndex(IndexToDraw))
	{
		EnemyToDraw = AvailableEnemyClasses[IndexToDraw];
		AvailableEnemyClasses.RemoveSingle(EnemyToDraw);
	}

	return EnemyToDraw;
}

TSubclassOf<AItemTemplate> UDeckManager::DrawItemFromDeck(EItemValue ItemValue)
{
	TSubclassOf<AItemTemplate> ItemToDraw = nullptr;
	int32 IndexToDraw;

	switch (ItemValue)
	{
	case EItemValue::C:

		IndexToDraw = FMath::RandRange(0, AvailableItemsClasses_C.Num() - 1);

		if (AvailableItemsClasses_C.IsValidIndex(IndexToDraw))
		{
			ItemToDraw = AvailableItemsClasses_C[IndexToDraw];
			AvailableItemsClasses_C.RemoveSingle(ItemToDraw);
		}

		break;

	case EItemValue::S:

		IndexToDraw = FMath::RandRange(0, AvailableItemsClasses_S.Num() - 1);

		if (AvailableItemsClasses_S.IsValidIndex(IndexToDraw))
		{
			ItemToDraw = AvailableItemsClasses_S[IndexToDraw];
			AvailableItemsClasses_S.RemoveSingle(ItemToDraw);
		}

		break;

	case EItemValue::G:

		IndexToDraw = FMath::RandRange(0, AvailableItemsClasses_G.Num() - 1);

		if (AvailableItemsClasses_G.IsValidIndex(IndexToDraw))
		{
			ItemToDraw = AvailableItemsClasses_G[IndexToDraw];
			AvailableItemsClasses_G.RemoveSingle(ItemToDraw);
		}

		break;

	default:
		break;
	}

	return ItemToDraw;
}

TSubclassOf<AEncounter_Good> UDeckManager::DrawGoodEncFromDeck()
{
	TSubclassOf<AEncounter_Good> GoodEncToDraw = nullptr;

	auto IndexToDraw = FMath::RandRange(0, AvailableGoodEncClasses.Num() - 1);

	if (AvailableGoodEncClasses.IsValidIndex(IndexToDraw))
	{
		GoodEncToDraw = AvailableGoodEncClasses[IndexToDraw];
		AvailableGoodEncClasses.RemoveSingle(GoodEncToDraw);
	}

	return GoodEncToDraw;
}

TSubclassOf<AEncounter_Dead> UDeckManager::DrawDeadFromDeck()
{
	TSubclassOf<AEncounter_Dead> DeadToDraw = nullptr;

	auto IndexToDraw = FMath::RandRange(0, AvailableDeadClasses.Num() - 1);

	if (AvailableDeadClasses.IsValidIndex(IndexToDraw))
	{
		DeadToDraw = AvailableDeadClasses[IndexToDraw];
		AvailableDeadClasses.RemoveSingle(DeadToDraw);
	}

	return DeadToDraw;
}

void UDeckManager::DistributeItemClassesByValue()
{
	for (int32 i = 0; i < AvailableItemsClasses.Num(); i++)
	{
		switch (auto c = AvailableItemsClasses[i].GetDefaultObject()->ItemValue)
		{
		case EItemValue::C:

			AvailableItemsClasses_C.Emplace(AvailableItemsClasses[i]);
			UE_LOG(LogTemp, Warning, TEXT("COPPER: %s"), *AvailableItemsClasses[i]->GetName())

				break;

		case EItemValue::S:

			AvailableItemsClasses_S.Emplace(AvailableItemsClasses[i]);
			UE_LOG(LogTemp, Warning, TEXT("SILVER: %s"), *AvailableItemsClasses[i]->GetName())

				break;

		case EItemValue::G:

			AvailableItemsClasses_G.Emplace(AvailableItemsClasses[i]);
			UE_LOG(LogTemp, Warning, TEXT("GOLD: %s"), *AvailableItemsClasses[i]->GetName())

				break;

		default:
			break;
		}
	}
	
}