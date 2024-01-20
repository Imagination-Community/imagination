// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Delegates/Delegate.h"

#include "ItemTrigger.generated.h"

UCLASS()
class IMAGINATIONPROJECT_API AItemTrigger : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AItemTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		bool IsGetterTrigger = false;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		bool IsItemAvailable = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTakeItemEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlaceItemEvent);

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnTakeItemEvent OnTakeItem;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnPlaceItemEvent OnPlaceItem;

	void TakeItem();
	void PlaceItem();
};
