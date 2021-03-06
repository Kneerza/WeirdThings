// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackDefenseComponent.h" //TODO get rid of heavy library
#include "AttackDefenseActor.generated.h"



class UPaperFlipbookComponent;
class UBoxComponent;
class AWTEnemy;
class AWTPlayerCharacter;
class UPaperFlipbook;
class ATimer;

UCLASS()
class NEWWEIRDTHINGS_API AAttackDefenseActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAttackDefenseActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		EAttackType AttackState = EAttackType::No_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		EDefenseType DefenseState = EDefenseType::No_Defense;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		UPaperFlipbookComponent* AttackDefenseFlipBookComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		UBoxComponent* CollisionBox;

	UPaperFlipbook* FlipbookToSet;

	USceneComponent* pRootComponent;

	TArray<AActor*> OverlappingActors = { nullptr };

	FVector StartLocation;
	int32 StartLocationOffset = 0;

	FVector EndLocation;

	AWTEnemy* Enemy = nullptr;
	AWTPlayerCharacter* PlayerCharacter = nullptr;

	int32 NumberOfActorsInSameRow = 1;
	bool IsAttacking = false;
	double TimeWhenAttackStarted = 0;
	bool DidAttackStart = false;

	bool IsLast = false;

	float LifeTime = 2.5f;
	float HoldTime = 0.f;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void CollisionResolve();

	void Initialize();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	double CreationTime;

	bool IsHarmless = false;
};
