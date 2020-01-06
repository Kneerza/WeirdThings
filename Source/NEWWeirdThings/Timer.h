// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Timer.generated.h"

class AWTEnemy;
class AWTPlayerCharacter;

UCLASS()
class NEWWEIRDTHINGS_API ATimer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimer();

	AWTEnemy* Enemy = nullptr;
	AWTPlayerCharacter* PlayerCharacter = nullptr;


	// TODO make time frames dependable
	float TimeCreated;
	float LifeTime = 2.7f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
