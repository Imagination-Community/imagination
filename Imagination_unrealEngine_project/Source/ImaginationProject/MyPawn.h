// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicalActor.h"
#include "ItemTrigger.h"
#include "Components/TextRenderComponent.h"
#include "Delegates/Delegate.h"

#include "UpgradeZone.h"
#include "MyPawn.generated.h"
UCLASS(BlueprintType)
class IMAGINATIONPROJECT_API AMyPawn : public APawn
{
	GENERATED_BODY()

private :

	APhysicalActor* physicalActor;
	AItemTrigger* m_itemTrigger = nullptr;
	AUpgradeZone* m_upgradeZone = nullptr;

	FVector m_inputMovement;
	FVector m_velocity;
	FVector m_inputRotation;
	float m_jumpHeightRuntime = false;

	FVector m_grapForward;
	FVector m_grapnelPoint;
	float m_grapnelLength;

	bool onGetItemTrigger = false;
	bool onSetItemTrigger = false;
	bool m_isGrapnel = false;
	bool m_mustJump = false;
	bool m_isJump = false;
	

private:
	bool ComputeGrapPoint(FVector& point, FVector groundPoint, FVector ceilPoint);
	void Grap(FVector point, float grapLength);

public:
	// Sets default values for this pawn's properties
	AMyPawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MoveForceMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float TurnForceMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MaxSpeed = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float breakingforce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float angularYawDamping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float maxAngularYawVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float rotationRollSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float checkGroundDistance = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float checkGroundDistanceHorizontalOffset = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float jumpHeight = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float maxXLocation = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float minXLocation = -1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feature1")
		float grabMaxDistance = 500;

	



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feature1")
		float checkFloorAndCeilDistance = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Feature1")
		float grabMinimalDistanceFromGround = 120;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UPhysicsConstraintComponent* PhysicsConstraint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* CubeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UTextRenderComponent* idText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* springArmComponent;

	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAddItem);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRemoveItem);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgraded, AUpgradeZone*, UpgradeZone);

	UPROPERTY(BlueprintAssignable, Category = "Event")
		FOnAddItem OnAddItem;

	UPROPERTY(BlueprintAssignable, Category = "Event")
		FOnRemoveItem OnRemoveItem;

	UPROPERTY(BlueprintAssignable, Category = "Event")
		FOnUpgraded OnUpgraded;


	UFUNCTION(BlueprintCallable, Category = "Feature")
	void OnHit(UPrimitiveComponent* hitcomponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "Feature")
		void OnComponentBeginOverlap(UPrimitiveComponent* overlapComponent, UPrimitiveComponent* OtherComp);

	UFUNCTION(BlueprintCallable, Category = "Feature")
		void OnComponentEndOverlap(UPrimitiveComponent* overlapComponent, UPrimitiveComponent* OtherComp);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AsyncPhysicsTickActor(float DeltaTime, float SimTime) override;

	void AddInputMovement(FVector value);
	void AddYawInputRotation(float value);
	void RotateCamera(FVector2D value);
	void FindPointAndGrap();
	bool ComputeGroundAndCeilPoint(FVector point, FVector& groundPoint, FVector& ceilPoint);
	bool ComputeGroundPoint(FVector point, FVector& groundPoint);
	void Jump();
	void TakeOrPlaceItem();
	void AddItem();
	void RemoveItem();
	void UpgradePawn();
	bool IsOnGround();
	void DisableGrapnel ();

	bool isInUpgradeState = false;
	bool haveGrapnel = false;

};
