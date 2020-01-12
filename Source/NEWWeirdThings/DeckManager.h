// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemTemplate.h"
#include "Components/ActorComponent.h"
#include "DeckManager.generated.h"

class ALocationTemplate;
class AItemTemplate;
class AEncounter_Dead;
class AEncounter_Good;
class AEncounter_Bad;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NEWWEIRDTHINGS_API UDeckManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDeckManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<TSubclassOf<ALocationTemplate>> AvailableLocationsClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<TSubclassOf<ALocationTemplate>> AvailablePlotLocationsClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<TSubclassOf<AEncounter_Bad>> AvailableEnemyClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<TSubclassOf<AEncounter_Dead>> AvailableDeadClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<TSubclassOf<AItemTemplate>> AvailableItemsClasses;

	TArray<TSubclassOf<AItemTemplate>> AvailableItemsClasses_C;
	TArray<TSubclassOf<AItemTemplate>> AvailableItemsClasses_S;
	TArray<TSubclassOf<AItemTemplate>> AvailableItemsClasses_G;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<TSubclassOf<AEncounter_Good>> AvailableGoodEncClasses;

	TSubclassOf<ALocationTemplate> DrawLocationFromDeck();
	TSubclassOf<ALocationTemplate> DrawPlotLocationFromDeck();
	TSubclassOf<AEncounter_Bad> DrawEnemyFromDeck();
	TSubclassOf<AItemTemplate> DrawItemFromDeck(EItemValue ItemValue);
	TSubclassOf<AEncounter_Good> DrawGoodEncFromDeck();
	TSubclassOf<AEncounter_Dead> DrawDeadFromDeck();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	void DistributeItemClassesByValue();

};
