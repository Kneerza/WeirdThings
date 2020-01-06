// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuitManagement.generated.h"

UENUM(BlueprintType)
enum class EQuitType : uint8
{
	No_QuitType,
	TooManyCharacters,
	Items,
	Fire,
	BadEncOnLocation,
	Food,
	TimeOfDay_Morning,
	TimeOfDay_Noon,
	TimeOfDay_Evening,
	TimeOfDay_Night,
	ToMuchHealth,
	NotEnoughHealth
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NEWWEIRDTHINGS_API UQuitManagement : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UQuitManagement();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EQuitType> QuitType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		int32 Modifier;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void OwnerQuits();

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void CheckQuitConditions();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



};
