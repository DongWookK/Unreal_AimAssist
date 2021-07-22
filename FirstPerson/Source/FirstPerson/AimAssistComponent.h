// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AimAssistComponent.generated.h"


class UCameraComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UAimAssistComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAimAssistComponent();

	bool IsAiming;
	UCameraComponent* CameraComp;
	AController* PController;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FCollisionQueryParams CollisionParams;

	ETraceTypeQuery TraceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RinterpSpeed")
		float RintSpeed;


	FVector Start;
	FVector ForwardVector;
	FVector End;
	FVector TargetLoc;
	FVector CameraLoc;
	FRotator TargetRot;
	FRotator CurrentRot;
	FRotator AutoTransitRot;

	void AutoRotation(FRotator target, FRotator curren);




public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;




};
