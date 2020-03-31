// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackDefenseActor.h"
#include "WTPlayerCharacter.h"
#include "PaperFlipbook.h"
#include "AttackDefenseComponent.h"
#include "WeirdThingsPlayerController.h"
#include "Encounter.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
AAttackDefenseActor::AAttackDefenseActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//----------------------Creating Root Component--------------------------
	pRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(pRootComponent);

	//----------------------Creating Action PaperFlipbook component----------
	AttackDefenseFlipBookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	AttackDefenseFlipBookComponent->SetupAttachment(pRootComponent);

	//Setting up relative transform
	FTransform ActionFlipBookComponentRelativeTransform;
	ActionFlipBookComponentRelativeTransform.SetLocation(FVector(0.f, 0.f, 0.f));
	ActionFlipBookComponentRelativeTransform.SetRotation(FRotator(0.f, 90.f, 0.f).Quaternion());
	ActionFlipBookComponentRelativeTransform.SetScale3D(FVector(0.4f, 0.4f, 0.4f));

	AttackDefenseFlipBookComponent->SetRelativeTransform(ActionFlipBookComponentRelativeTransform);
	AttackDefenseFlipBookComponent->bGenerateOverlapEvents = false;
	AttackDefenseFlipBookComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//----------------------Creating Collision Box------------------------
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(pRootComponent);

	//Setting up relative transform
	FTransform CollisionBoxRelativeTransform;
	CollisionBoxRelativeTransform.SetLocation(FVector(0.f, 0.f, 0.f));
	CollisionBoxRelativeTransform.SetRotation(FRotator(0.f, 0.f, 0.f).Quaternion());
	CollisionBoxRelativeTransform.SetScale3D(FVector(0.2f, 0.6f, 0.6f));

	CollisionBox->SetRelativeTransform(CollisionBoxRelativeTransform);

	//Setting up collision
	FCollisionResponseContainer CollisionBoxResponseContainer;
	CollisionBoxResponseContainer.SetAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBoxResponseContainer.SetResponse(ECollisionChannel::ECC_Destructible, ECollisionResponse::ECR_Overlap);
	CollisionBoxResponseContainer.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	CollisionBox->SetCollisionResponseToChannels(CollisionBoxResponseContainer);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
}

// Called when the game starts or when spawned
void AAttackDefenseActor::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	CreationTime = FPlatformTime::Seconds();
}

// Called every frame
void AAttackDefenseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if ((FPlatformTime::Seconds() - CreationTime) > (LifeTime + HoldTime))
	{
		Destroy();
	}

	auto Speed = DeltaTime *30;

	if (IsAttacking) {
	
		if (!DidAttackStart) {
			TimeWhenAttackStarted = FPlatformTime::Seconds();
			DidAttackStart = true;
		}
		if ((FPlatformTime::Seconds() - TimeWhenAttackStarted /*CreationTime*/) > HoldTime) {
			if (GetActorLocation().Z < (StartLocation.Z + 280 + StartLocationOffset + (140*NumberOfActorsInSameRow)) && (GetActorLocation().Y == StartLocation.Y))
			{
				SetActorLocation((GetActorLocation() + FVector(0.f, 0.f, 40* Speed)));
			}
			else if (GetActorLocation().Y == StartLocation.Y)
			{
				SetActorLocation(EndLocation + FVector(0.f, 0.f, 1200.f));// +(100 * NumberOfActorsInSameRow)));
			}
			else
			{
				SetActorLocation((GetActorLocation() + FVector(0.f, 0.f, -40 * Speed)));
			}
		}
		if ((GetActorLocation().Z + 100) < EndLocation.Z) {
			Destroy();
		}
	}

}

void AAttackDefenseActor::Initialize()
{
	
	// Initialize as Attack
	switch (AttackState)
	{
	case EAttackType::No_Attack:

		//-If not Attack - initialize as Defence------------------------------------------------
		switch (DefenseState)
		{
		case EDefenseType::No_Defense:

			UE_LOG(LogTemp, Warning, TEXT("NO DEFENSE!!!"))
			break;

		case EDefenseType::Absorb:

			UE_LOG(LogTemp, Warning, TEXT("ABSORB!!!"))
			AttackDefenseFlipBookComponent->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/DefenseType_Absorb_Flipbook.DefenseType_Absorb_Flipbook'"))));

			break;

		case EDefenseType::Evade:

			UE_LOG(LogTemp, Warning, TEXT("EVADE!!!"))
			AttackDefenseFlipBookComponent->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/DefenseType_Evade_Flipbook.DefenseType_Evade_Flipbook'"))));

			break;

		default:
			break;
		}
		//---------------------------------------------------------------------------------------
		break;

	case EAttackType::Sharp:

		AttackDefenseFlipBookComponent->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/AttackType_Sharp_Flipbook.AttackType_Sharp_Flipbook'"))));
		IsAttacking = true;

		break;

	case EAttackType::Blunt:

		AttackDefenseFlipBookComponent->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/AttackType_Blunt_Flipbook.AttackType_Blunt_Flipbook'"))));
		IsAttacking = true;

		break;

	case EAttackType::Unavoidable:

		AttackDefenseFlipBookComponent->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/AttackType_Unavoidable_Flipbook.AttackType_Unavoidable_Flipbook'"))));
		IsAttacking = true;

		break;

	case EAttackType::Tricky:

		AttackDefenseFlipBookComponent->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, (TEXT("PaperFlipbook'/Game/Blueprints/AttackType_Tricky_Flipbook.AttackType_Tricky_Flipbook'"))));
		IsAttacking = true;

		break;

	default:
		break;
	}
	
}

void AAttackDefenseActor::CollisionResolve()
{
	
	GetOverlappingActors(OverlappingActors);

	if (!ensure(OverlappingActors[0])) { return; }
	if (Cast<AEncounter>(OverlappingActors[0])) {

		Cast<AEncounter>(OverlappingActors[0])->LooseHealth();
		UE_LOG(LogTemp, Warning, TEXT("Health is lost"))

			SetActorEnableCollision(false);

		Destroy();

		return;
	}
	else if (Cast<AWTPlayerCharacter>(OverlappingActors[0]))
	{

		Cast<AWTPlayerCharacter>(OverlappingActors[0])->GetInjury(1);
		UE_LOG(LogTemp, Warning, TEXT("Character is hit"))


			SetActorEnableCollision(false);

		Destroy();

		return;
	}


	if (!ensure(Cast<AAttackDefenseActor>(OverlappingActors[0]))) { return; }
	auto  OverlappingActor = Cast<AAttackDefenseActor>(OverlappingActors[0]);
	EAttackType OverlappingActorAttackState = OverlappingActor->AttackState;

	if (AttackState != EAttackType::No_Attack) { return; }
	if (DefenseState == EDefenseType::Absorb)
	{
		switch (OverlappingActorAttackState)
		{
		case EAttackType::Sharp:

			OverlappingActors[0]->Destroy();

			Destroy();
			break;

		case EAttackType::Blunt:

			OverlappingActor->AttackState = EAttackType::Sharp;
			OverlappingActor->Initialize();

			Destroy();

			break;

		case EAttackType::Unavoidable:

			OverlappingActors[0]->Destroy();

			Destroy();
			break;

		case EAttackType::Tricky:

			Destroy();

			break;

		default:
			break;
		}
	}
	if (DefenseState == EDefenseType::Evade)
	{
		switch (OverlappingActorAttackState)
		{

		case EAttackType::No_Attack:

			OverlappingActors[0]->SetActorEnableCollision(false);
			OverlappingActors[0]->Destroy();

			break;

		case EAttackType::Sharp:

			OverlappingActors[0]->SetActorEnableCollision(false);
			OverlappingActors[0]->Destroy();

			break;

		case EAttackType::Blunt:

			OverlappingActors[0]->Destroy();
			break;

		case EAttackType::Unavoidable:

			Destroy();

			break;

		case EAttackType::Tricky:


			Destroy();
			break;

		default:
			break;
		}
	}
	
}
