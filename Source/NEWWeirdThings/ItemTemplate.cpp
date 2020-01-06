// Fill out your copyright notice in the Description page of Project Settings.

//#define GETENUMSTRING(etype, evalue) ( (FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true) != nullptr) ? FindObject<UEnum>(ANY_PACKAGE, TEXT(etype), true)->GetEnumName((int32)evalue) : FString("Invalid - are you sure enum uses UENUM() macro?") )

#include "ItemTemplate.h"
#include "AttackDefenseComponent.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"


// Sets default values
AItemTemplate::AItemTemplate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//-----------------------Initializing arrays-----------------------------
	ItemType.Init(EItemType::No_Type, 3);
	ItemDurabilityByType.Init(0, 3);
}

// Called when the game starts or when spawned
void AItemTemplate::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AItemTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UTexture2D* AItemTemplate::GetItemIcon()
{
	//if (!ItemIcon) { return; }
	return ItemIcon;
}

void AItemTemplate::Deactivate()
{
	//FindComponentByClass<UStaticMeshComponent>()->DestroyComponent();
}