// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "UpgradeZone.h"
#include "FirstDoor.generated.h"

/**
 * 
 */
UCLASS()
class IMAGINATIONPROJECT_API AFirstDoor : public ADoor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Method")
	void OnUpgradeEvent(AUpgradeZone* upgradeZone);
};
