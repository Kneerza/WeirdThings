// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveLocationDecoration.generated.h"

class AAction;
class UPaperSprite;
class UPaperSpriteComponent;
class USceneComponent;

UCLASS()
class NEWWEIRDTHINGS_API AInteractiveLocationDecoration : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractiveLocationDecoration();

	USceneComponent* pRootComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		UPaperSpriteComponent* InteractiveLocationDecoration_SpriteComponent_0;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		UPaperSpriteComponent* InteractiveLocationDecoration_SpriteComponent_1;

	AAction* EntangledAction = nullptr;

	void ChangeState();
	
	void Deactivate();
	void Activate();

	UFUNCTION(BlueprintCallable, Category = "Custom")
		void SetIsHovered(bool IsHovered);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		bool IsLastStateActive = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
