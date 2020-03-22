// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackDefenseComponent.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	No_Attack,
	Sharp,
	Blunt,
	Unavoidable,
	Tricky
};

UENUM(BlueprintType)
enum class EDefenseType : uint8
{
	No_Defense,
	Absorb,
	Evade
};

class UWidgetComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NEWWEIRDTHINGS_API UAttackDefenseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttackDefenseComponent();

	UWidgetComponent* pAttackDefenseWidget;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
	//	TSubclassOf<UUserWidget> AttackDefenseClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EAttackType> AttackPoolRow_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EDefenseType> DefensePoolRow_1;

	TArray<EAttackType> AttackPoolRow_Default;
	TArray<EDefenseType> DefensePoolRow_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EAttackType> AttackPoolRow_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EDefenseType> DefensePoolRow_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EAttackType> AttackPoolRow_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EDefenseType> DefensePoolRow_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EAttackType> AttackPoolRow_4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EDefenseType> DefensePoolRow_4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EAttackType> AttackPoolRow_5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EDefenseType> DefensePoolRow_5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EAttackType> AttackPoolRow_6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EDefenseType> DefensePoolRow_6;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

};
