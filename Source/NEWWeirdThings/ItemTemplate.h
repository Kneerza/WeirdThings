// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackDefenseComponent.h"
#include "GameFramework/Actor.h"
#include "ItemTemplate.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	No_Type,
	Weapon,
	Tool,
	Flamable,
	Armor,
	Axe,
	Shield
};

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	Head,
	Body,
	Legs,
	No_ArmorType
};

UENUM(BlueprintType)
enum class EItemValue : uint8
{
	C,
	S,
	G
};

UCLASS()
class NEWWEIRDTHINGS_API AItemTemplate : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemTemplate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EAttackType> AttackPoolRow_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EDefenseType> DefensePoolRow_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		EItemValue ItemValue = EItemValue::C;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		EArmorType ArmorType = EArmorType::Head;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		UTexture2D* ItemCard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		int32 Damage = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<EItemType> ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		TArray<int32> ItemDurabilityByType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		bool IsActive = false;

	UFUNCTION(BlueprintCallable, Category = "Custom")
		UTexture2D* GetItemIcon();



	void Deactivate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
