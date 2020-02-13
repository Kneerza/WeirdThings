// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LocationTemplate.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class UBoxComponent;
class USceneComponent;
class UChildActorComponent;
class AArrowTemplate;
class AActor;
class AAction;
class UPaperFlipbookComponent;
class AInteractiveLocationDecoration;

UCLASS()
class NEWWEIRDTHINGS_API ALocationTemplate : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALocationTemplate();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		USceneComponent* pRootComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		UStaticMeshComponent* pGround;

	AArrowTemplate* ArrowRight = nullptr;
	AArrowTemplate* ArrowUp = nullptr;

	AAction* ForcedAction = nullptr;

	TArray<FVector> SocketsLockations;

	// Actions formation
	float SocketLocationsStepBetweenColumns = 430.f;
	float SocketLocationsStepBetweenRow = -300.f;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void EntangleActionWithActor(UChildActorComponent* Action, UChildActorComponent* InteractiveLocationDecoration);

	FVector SocketLocationsOffset = FVector(-600.f, -650.f, -350.f);

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* pBackGround;

	//--------------------Sockets-----------------
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		TArray<UStaticMeshComponent*> AvailableSocketPlayer;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* SocketPlayer_0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* SocketPlayer_1;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketPlayer_2;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketPlayer_3;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketPlayer_4;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketCampFire;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		TArray<UStaticMeshComponent*> AvailableSocketEncounter;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketEncounter_0;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketEncounter_1;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketEncounter_2;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketEncounter_3;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketEncounter_4;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		TArray <UStaticMeshComponent*> AvailableSocketCampFire;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketDynamicAction_0;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketDynamicAction_1;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketDynamicAction_2;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		TArray <UStaticMeshComponent*> AvailableSocketDynamicAction;
	//---------------------------------------------------

	TArray<FTransform> SocketPlayer_Transform;

	FTransform SocketCampFire_Transform;

	TArray<FTransform> SocketEncounter_Transform;

	TArray<FTransform*> OccupiedSockets;

	int32 VerticalIndex;
	int32 HorizontalIndex;

	AActor* pArrow;

	AActor* pArrowUp;

	UPROPERTY(EditAnywhere, Category = Setup)
		TSubclassOf<AArrowTemplate> ArrowBlueprint;

	UPROPERTY(EditAnywhere, Category = "Arrow")
		bool HasArrowUp = true;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void ExcludeFromAvailablePlayerSockets(UPrimitiveComponent* ComponentToExclude);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void IncludeInAvailablePlayerSockets(UPrimitiveComponent* ComponentToInclude);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void ExcludeFromAvailableEncounterSockets(UPrimitiveComponent* ComponentToExclude);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void IncludeInAvailableEncounterSockets(UPrimitiveComponent* ComponentToInclude);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void Connect(bool IsInfinite, UChildActorComponent* Parent, UPaperFlipbookComponent* Connector_0, UChildActorComponent* Child_0, UPaperFlipbookComponent* Connector_1, UChildActorComponent* Child_1, UPaperFlipbookComponent* Connector_2, UChildActorComponent* Child_2, UPaperFlipbookComponent* Connector_3, UChildActorComponent* Child_3, UPaperFlipbookComponent* Connector_4, UChildActorComponent* Child_4);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void ExcludeFromAvailableCampFireSocket(UPrimitiveComponent* ComponentToExclude);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void IncludeInAvailableCampFireSocket(UPrimitiveComponent* ComponentToInclude);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void ExcludeFromAvailableDynamicActionSocket(UPrimitiveComponent* ComponentToExclude);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void IncludeInAvailableDynamicActionSocket(UPrimitiveComponent* ComponentToInclude);

	//UFUNCTION(BlueprintCallable, Category = "Custom")
	//	void TreatAsSameAction(TArray<AAction*> SameActions);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
