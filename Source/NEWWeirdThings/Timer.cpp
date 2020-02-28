// Fill out your copyright notice in the Description page of Project Settings.

#include "Timer.h"
#include "WeirdThingsPlayerController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "WeirdThingsPlayerController.h"


// Sets default values
ATimer::ATimer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimer::BeginPlay()
{
	Super::BeginPlay();

	TimeCreated = FPlatformTime::Seconds();
	Cast< AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController())->AreClickEventsDisabled = true;
}

// Called every frame
void ATimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((FPlatformTime::Seconds() - TimeCreated) >= LifeTime)
	{
		if (bIsFightingBack) {
			Cast<AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController())->FightBack(Enemy, PlayerCharacter);
			//UE_LOG(LogTemp, Warning, TEXT("%f"), (FPlatformTime::Seconds() - TimeCreated))
		}

		Cast< AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController())->AreClickEventsDisabled = false;
		Destroy();
	}
}

