// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackDefenseComponent.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "PaperFlipbookComponent.h"


// Sets default values for this component's properties
UAttackDefenseComponent::UAttackDefenseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	//-----------------------Initializing arrays-----------------------------
	//AttackPoolRow_1.Init(EAttackType::No_Attack, 5);
	//DefensePoolRow_1.Init(EDefenseType::No_Defense, 5);

	AttackPoolRow_2.Init(EAttackType::No_Attack, 5);
	DefensePoolRow_2.Init(EDefenseType::No_Defense, 5);

	AttackPoolRow_3.Init(EAttackType::No_Attack, 5);
	DefensePoolRow_3.Init(EDefenseType::No_Defense, 5);

	AttackPoolRow_4.Init(EAttackType::No_Attack, 5);
	DefensePoolRow_4.Init(EDefenseType::No_Defense, 5);

	AttackPoolRow_5.Init(EAttackType::No_Attack, 5);
	DefensePoolRow_5.Init(EDefenseType::No_Defense, 5);

	AttackPoolRow_6.Init(EAttackType::No_Attack, 5);
	DefensePoolRow_6.Init(EDefenseType::No_Defense, 5);
}


// Called when the game starts
void UAttackDefenseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...


	AttackPoolRow_Default = AttackPoolRow_1;
	DefensePoolRow_Default = DefensePoolRow_1;

}


// Called every frame
void UAttackDefenseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
