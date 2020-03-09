// Fill out your copyright notice in the Description page of Project Settings.

#include "Encounter_Good.h"
#include "Action.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "WeirdThingsPlayerController.h"

void AEncounter_Good::SetIsHovered(bool IsHovered)
{
	auto PlayerController = Cast<AWeirdThingsPlayerController>(GetWorld()->GetFirstPlayerController());

	if (IsHovered)
	{
		PlayerController->SetCurrentlyHoveredByMouseEncounter_Good(true, this);
	}
	else
	{
		PlayerController->SetCurrentlyHoveredByMouseEncounter_Good(false, nullptr);
	}
}


