// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "CableComponent.h"
#include "PhysicalActor.generated.h"

UCLASS()
class IMAGINATIONPROJECT_API APhysicalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhysicalActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UPhysicsConstraintComponent* PhysicsConstraint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UCableComponent* cableComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		USphereComponent* shpereComponent;


	void OnHit(UPrimitiveComponent* hitcomponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
