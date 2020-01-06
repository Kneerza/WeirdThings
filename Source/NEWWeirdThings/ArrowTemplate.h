// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArrowTemplate.generated.h"

UENUM()
enum class EArrowType : uint8
{
	Good,
	Plot,
	Bad,
	Ugly
};

class UStaticMeshComponent;
class UScenneComponent;
class AActor;
class ALocationTemplate;
class UPaperSpriteComponent;
class UPaperSprite;

UCLASS()
class NEWWEIRDTHINGS_API AArrowTemplate : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArrowTemplate();

	USceneComponent* pRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		EArrowType ArrowType;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	UPaperSpriteComponent* Arrow;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	UPaperSprite* DeactivatedArrow;

	//Location blueprint to spawn
	//UPROPERTY(EditAnywhere, Category = Setup)
	//	TSubclassOf<ALocationTemplate> LocationBlueprint;

	UPROPERTY(EditAnywhere, Category = Setup)
		FVector SpawnedLocationOffsetY = FVector(0.f, 3000.f, 0.f);

	UPROPERTY(EditAnywhere, Category = Setup)
		FVector SpawnedLocationOffsetZ = FVector(0.f, 0.f, 2000.f);

	UPROPERTY(EditAnywhere, Category = Setup)
		float LocationsRowLimit = 65000.f;

	void SpawnLocation(bool IsSpawningOnRight, TSubclassOf<UObject> LocationToSpawnClass);

	void DeactivateArrow();

	UPROPERTY(EditAnywhere, Category = Setup)
		bool IsArrowRight = true;

	void BuildArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
