// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackDefenseComponent.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
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

	/*     //TODO make it pop-up info
	auto pAttackDefenseWidgetClass = LoadObject<UObject>(nullptr, (TEXT("WidgetBlueprint'/Game/Blueprints/AttackDefense.AttackDefense'")));

	UWidgetComponent* pAttackDefenseWidget = NewObject<UWidgetComponent>(this, "AttackDefenseWidget");
	pAttackDefenseWidget->SetupAttachment(GetOwner()->GetRootComponent());
	pAttackDefenseWidget->RegisterComponent();
	auto dsfsg = Cast<UBlueprint>(pAttackDefenseWidgetClass)->GeneratedClass;
	pAttackDefenseWidget->SetWidgetClass(*Cast<UBlueprint>(pAttackDefenseWidgetClass)->GeneratedClass);

	FTransform WidgetRelativeTransform;
	WidgetRelativeTransform.SetLocation(FVector(0.f, 0.f, 200.f));
	WidgetRelativeTransform.SetRotation(FRotator(0.0f, 180.0f, 0.0f).Quaternion());
	WidgetRelativeTransform.SetScale3D(FVector(0.4f, 0.05f, 0.4f));

	pAttackDefenseWidget->SetRelativeTransform(WidgetRelativeTransform);

	pAttackDefenseWidget->SetDrawSize(FVector2D(1280.f, 720.f));
	*/

}


// Called every frame
void UAttackDefenseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttackDefenseComponent::BuildAttackDefenseRow(TArray<EDefenseType> DefensePool)
{

}

void UAttackDefenseComponent::BuildAttackDefenseRow(TArray<EAttackType> AttackPool)
{

}