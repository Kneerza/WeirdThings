// Fill out your copyright notice in the Description page of Project Settings.

#include "Encounter_Bad.h"
#include "Engine/World.h"
#include "LocationTemplate.h"
#include "WeirdThingsPlayerController.h"




void AEncounter_Bad::BeginPlay()
{
	Super::BeginPlay();

	
	auto PlayerController = (Cast<AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController()));
	
	PlayerController->Encounter_BadInPlay.Add(this);
	
	if (GetParentActor()) {
		CurrentLocation = Cast<ALocationTemplate>(GetParentActor());
	}
	
	
}

void AEncounter_Bad::SetIsHovered(bool IsHovered)
{
	auto PlayerController = Cast<AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController());

	if (IsHovered)
	{
		PlayerController->SetCurrentlyHoveredByMouseEncounter_Bad(true, this);
	}
	else
	{
		PlayerController->SetCurrentlyHoveredByMouseEncounter_Bad(false, nullptr);
	}
}