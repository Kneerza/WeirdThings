// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Math/Vector2D.h"
#include "WTDead.generated.h"

class UWidgetComponent;
class ALocationTemplate;
class AAttackDefenseActor;
class UAttackDefenseComponent;
class AItemTemplate;
class UUserWidget;
class UQuitManagement;
class UPaperFlipbook;
class UPaperFlipbookComponent;
class ALocationTemplate;
class AAction;

UCLASS()
class NEWWEIRDTHINGS_API AWTDead : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWTDead();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TSubclassOf<AAction> ActionClassToSpawn;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		UPaperFlipbookComponent* AwakenedDeadFlipbookComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		UPaperFlipbookComponent* SleepingDeadFlipbookComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<TSubclassOf<AItemTemplate>> LootClassesToSpawn;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		TArray<AItemTemplate*> Loot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		UAttackDefenseComponent* pAttackDefenseComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
		UQuitManagement* pQuitManagementComponent;

	USceneComponent* pRootComponent;

	///UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Custom")
	///	UWidgetComponent* pHealthWidget;

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

	///UFUNCTION(BlueprintCallable, Category = "Custom")
	///	void CreateInfoWidget(UUserWidget* WidgetToCreate, FVector2D WidgetOffset);

	void InitializeLoot();

	void SetAwakened(bool IsAwakened);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};
