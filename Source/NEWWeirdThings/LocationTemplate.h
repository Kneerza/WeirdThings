// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LocationTemplate.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class AWeirdThingsPlayerController;
class UBoxComponent;
class USceneComponent;
class UChildActorComponent;
class AActor;
class AAction;
class UPaperFlipbookComponent;

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
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		UChildActorComponent* Door;

	AWeirdThingsPlayerController* PlayerController;

	AAction* ForcedAction = nullptr;

	TArray<FVector> SocketsLockations;



	// Actions formation
	float SocketLocationsStepBetweenColumns = 430.f;
	float SocketLocationsStepBetweenRow = -300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		bool IsRestricting = false;

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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* SocketPlayer_2;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
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
		TArray<UStaticMeshComponent*> AvailableSocketEncounter_Dead;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketEncounter_Dead_0;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketEncounter_Dead_1;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketEncounter_Dead_2;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketEncounter_Dead_3;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketEncounter_Dead_4;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		TArray<UStaticMeshComponent*> AvailableSocketEncounter_Good;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketEncounter_Good_0;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketEncounter_Good_1;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketEncounter_Good_2;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		TArray <UStaticMeshComponent*> AvailableSocketCampFire;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketDynamicAction_0;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketDynamicAction_1;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketDynamicAction_2;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketDynamicPlayerAction_0;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketDynamicPlayerAction_1;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketDynamicPlayerAction_2;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketDynamicPlayerAction_3;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketDynamicPlayerAction_4;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketDynamicPlayerAction_5;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* SocketDoor;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		TArray <UStaticMeshComponent*> AvailableSocketDynamicAction;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		TArray <UStaticMeshComponent*> AvailableSocketDynamicPlayerAction;
	//---------------------------------------------------

	TArray<FTransform> SocketPlayer_Transform;

	FTransform SocketCampFire_Transform;

	TArray<FTransform> SocketEncounter_Transform;

	TArray<FTransform*> OccupiedSockets;

	int32 VerticalIndex;
	int32 HorizontalIndex;

	AActor* pArrow;

	AActor* pArrowUp;

	TArray<UChildActorComponent*> DynamicAction = { nullptr, nullptr, nullptr };

	void CreateDynamicAction(TSubclassOf<AAction> ActionClass, UChildActorComponent* ActorToEntangleWith, ALocationTemplate* LocationInstigator);

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
		void ExcludeFromAvailableEncounter_GoodSockets(UPrimitiveComponent* ComponentToExclude);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void IncludeInAvailableEncounter_GoodSockets(UPrimitiveComponent* ComponentToInclude);

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

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void ExcludeFromAvailableDynamicPlayerActionSocket(UPrimitiveComponent* ComponentToExclude);

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void IncludeInAvailableDynamicPlayerActionSocket(UPrimitiveComponent* ComponentToInclude);

	//UFUNCTION(BlueprintCallable, Category = "Custom")
	//	void TreatAsSameAction(TArray<AAction*> SameActions);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
