// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class IMAGINATIONPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	AMyPlayerController();

	void TurnCamera();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void Move(FVector2D value);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void Rotate(float value);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RotateCamera(FVector2D value);

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void Jump();

	UFUNCTION(BlueprintCallable, Category = "Feature")
		void EnableGrapnel ();

	UFUNCTION(BlueprintCallable, Category = "Feature")
		void DisableGrapnel ();

	UFUNCTION(BlueprintCallable, Category = "Feature")
		void TakeOrPlaceItem();

	UFUNCTION(BlueprintCallable, Category = "Feature")
		void UpgradePawn();
};
