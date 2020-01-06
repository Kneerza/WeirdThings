// Fill out your copyright notice in the Description page of Project Settings.

#include "ArrowTemplate.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "LocationTemplate.h"
#include "Runtime/Core/Public/Math/TransformNonVectorized.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
//#include "Runtime/Engine/Classes/GameFramework/Actor.h"
//#include "Runtime/Core/Public/Math/TransformNonVectorized.h"


// Sets default values
AArrowTemplate::AArrowTemplate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//----------------------Creating Root Component--------------------------
	pRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(pRootComponent);

	//----------------------Creating Sprite component----------
	Arrow = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Arrow->SetupAttachment(pRootComponent);

	//Setting up relative transform
	FTransform ActionFlipBookComponentRelativeTransform;
	ActionFlipBookComponentRelativeTransform.SetLocation(FVector(0.f, 0.f, 0.f));
	ActionFlipBookComponentRelativeTransform.SetRotation(FRotator(0.f, 90.f, 0.f).Quaternion());
	ActionFlipBookComponentRelativeTransform.SetScale3D(FVector(0.4f, 0.4f, 0.4f));

	Arrow->SetRelativeTransform(ActionFlipBookComponentRelativeTransform);

	FCollisionResponseContainer CollisionArrowResponseContainer;
	CollisionArrowResponseContainer.SetAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionArrowResponseContainer.SetResponse(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	CollisionArrowResponseContainer.SetResponse(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	Arrow->SetCollisionResponseToChannels(CollisionArrowResponseContainer);
	Arrow->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
}

// Called when the game starts or when spawned
void AArrowTemplate::BeginPlay()
{
	Super::BeginPlay();

	BuildArrow();
	if (GetAttachParentActor())
	{
		if (IsArrowRight) {
			Cast<ALocationTemplate>(GetAttachParentActor())->ArrowRight = this;
		}
		else {
			Cast<ALocationTemplate>(GetAttachParentActor())->ArrowUp = this;
		}
	}
}

// Called every frame
void AArrowTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AArrowTemplate::SpawnLocation(bool IsSpawningOnRight, TSubclassOf<UObject> LocationToSpawnClass)
{
	FActorSpawnParameters SpawningLocationParameters;
	SpawningLocationParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	if (GetAttachParentActor()) {

		// Set Transform for spawned location
		FTransform ParentlLocationTransform = GetAttachParentActor()->GetActorTransform();
		FTransform SpawnedLocationTransform;
		SpawnedLocationTransform.SetScale3D(FVector(1.f, 1.f, 1.f));
		if (IsSpawningOnRight) {
			SpawnedLocationTransform.SetLocation(ParentlLocationTransform.GetLocation() + SpawnedLocationOffsetY);
		}
		else {
			SpawnedLocationTransform.SetLocation(ParentlLocationTransform.GetLocation() + SpawnedLocationOffsetZ);
		}
		SpawnedLocationTransform.SetRotation(ParentlLocationTransform.GetRotation());

		if ((SpawnedLocationTransform.GetLocation().Y > LocationsRowLimit)) { return; }
		GetWorld()->SpawnActor<ALocationTemplate>(LocationToSpawnClass, SpawnedLocationTransform, SpawningLocationParameters);
		
	}

}

void AArrowTemplate::DeactivateArrow()
{
	if (DeactivatedArrow) {
		Arrow->SetSprite(DeactivatedArrow);
	}
	else {
		Destroy();
		return;
	}
	Arrow->RelativeLocation.Z -= 80;
	Arrow->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
	Arrow->CastShadow = true;
	Arrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AArrowTemplate::BuildArrow()
{
	switch (ArrowType)
	{
	case EArrowType::Good:

		Arrow->SetSprite(LoadObject<UPaperSprite>(nullptr, (TEXT("PaperSprite'/Game/Blueprints/TEXTURES/Arrow_Good_Sprite.Arrow_Good_Sprite'"))));
		DeactivatedArrow = LoadObject<UPaperSprite>(nullptr, (TEXT("PaperSprite'/Game/Blueprints/TEXTURES/Arrow_Good_OnStamp_Sprite.Arrow_Good_OnStamp_Sprite'")));
		break;

	case EArrowType::Bad:

		Arrow->SetSprite(LoadObject<UPaperSprite>(nullptr, (TEXT("PaperSprite'/Game/Blueprints/TEXTURES/Arrow_Bad_Sprite.Arrow_Bad_Sprite'"))));
		DeactivatedArrow = LoadObject<UPaperSprite>(nullptr, (TEXT("PaperSprite'/Game/Blueprints/TEXTURES/Arrow_Bad_OnStamp_Sprite.Arrow_Bad_OnStamp_Sprite'")));
		break;

	case EArrowType::Ugly:

		Arrow->SetSprite(LoadObject<UPaperSprite>(nullptr, (TEXT("PaperSprite'/Game/Blueprints/TEXTURES/Arrow_Ugly_Sprite.Arrow_Ugly_Sprite'"))));
		DeactivatedArrow = LoadObject<UPaperSprite>(nullptr, (TEXT("PaperSprite'/Game/Blueprints/TEXTURES/Arrow_Ugly_OnStamp_Sprite.Arrow_Ugly_OnStamp_Sprite'")));
		break;

	case EArrowType::Plot:

		Arrow->SetSprite(LoadObject<UPaperSprite>(nullptr, (TEXT("PaperSprite'/Game/Blueprints/TEXTURES/Arrow_Plot_Sprite.Arrow_Plot_Sprite'"))));
		
		break;


	default:
		break;
	}
}