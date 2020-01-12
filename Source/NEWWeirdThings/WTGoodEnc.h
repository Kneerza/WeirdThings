// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WTGoodEnc.generated.h"

class UWidgetComponent;
class ALocationTemplate;
class AAttackDefenseActor;
class UAttackDefenseComponent;

// TODO Delete class

UCLASS()
class NEWWEIRDTHINGS_API AWTGoodEnc : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWTGoodEnc();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		UAttackDefenseComponent* pAttackDefenseComponent;

	USceneComponent* pRootComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		UWidgetComponent* pHealthWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		int32 HealthPoints = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		int32 MaxDamage = 3;

	AAttackDefenseActor* AttackDefenseActorToIgnore;

	float DamageWidgetAlphaChannel = 1;

	ALocationTemplate* CurrentLocation = nullptr;

	//UFUNCTION(BlueprintCallable, Category = "Custom")
	void LooseHealth(AAttackDefenseActor* AttackDefenseActorRef);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void LooseHealth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};
