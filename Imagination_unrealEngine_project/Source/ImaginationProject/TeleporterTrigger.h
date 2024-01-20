// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleporterTrigger.generated.h"

UENUM(BlueprintType)
enum class ETeleporterType : uint8
{
	Out UMETA(DisplayName = "Out"),
	In UMETA(DisplayName = "In"),
	Both UMETA(DisplayName = "Both")
};


UCLASS()
class IMAGINATIONPROJECT_API ATeleporterTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleporterTrigger();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeleporterTrigger")
		ETeleporterType TeleporterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeleporterTrigger")
	ATeleporterTrigger* connectedTeleportTrigger;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TeleporterTrigger")
		FVector teleporterDirection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
