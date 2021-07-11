// Fill out your copyright notice in the Description page of Project Settings.


#include "AimAssistComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Pawn.h"

// Sets default values for this component's properties
UAimAssistComponent::UAimAssistComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	RintSpeed = 5.0f;
	// ...
}


// Called when the game starts
void UAimAssistComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


/*
Brief: TickComponent ���ο��� ��Ʈ�ѷ�(ũ�ν���� ��ġ)�� ������ ����ϴ� �Լ�
Input: Target Rotation, Current Pawn's Rotation
Output: Controller Rotation
*/
void UAimAssistComponent::AutoRotation(FRotator Target, FRotator Current)
{
	float Deltatime = GetWorld()->GetDeltaSeconds();
	AutoTransitRot = UKismetMathLibrary::RInterpTo(Current, Target, Deltatime, RintSpeed);
	PController->SetControlRotation(AutoTransitRot);
}



// Called every frame
/*
Brief: ĳ���Ͱ� ��Ŭ������ ���� �� Target�� �ش��ϴ� ���͸� �ĺ��� �ش� ���ͷ� �ڵ� �������ִ� �Լ�
Input: Controller, CameraComponent
Output: Controller Rotation
*/
void UAimAssistComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	FHitResult OutHit;
	Start = CameraComp->GetComponentLocation();
	ForwardVector = CameraComp->GetForwardVector();
	End = Start + (ForwardVector * 10000);

	//3��Ī �����θ�ŭ 
	CameraLoc = CameraComp->GetComponentLocation();
	TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2);
	TArray<AActor*> ActorsToIgnore{ this->GetOwner() };


	//����׽� EDrawDebugTrace�� Type���� ����
	//���� Capsule Ʈ���̽��� ������������ �ٷ� bBlockingHit �Ǿ� Ʈ���̽����� �ʴ� ���� �߻�. ���� �ʿ�.
	//Bullet�� �Ʒ� ĸ�� Ʈ���̽����� ���õǰԲ� Collision Ignore ó�� �ʿ�
	if (UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), CameraLoc, End, 75.0f, 25.0f, TraceType, false, ActorsToIgnore, EDrawDebugTrace::Type::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, 1.0f))
	{
		if (OutHit.bBlockingHit)
		{
			// Tag : Target�� �ش��ϴ� ���͸� �������� ���
			if (OutHit.GetActor()->ActorHasTag(TEXT("Target")))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
				TargetLoc = OutHit.GetActor()->GetActorLocation();
				//��ֶ������̼��� ���� �������, Aim�� �Բ� Fire �Լ� ���� �� �������� ����
				TargetLoc = (TargetLoc - OutHit.ImpactPoint).GetSafeNormal(0.0001);
				TargetRot = UKismetMathLibrary::MakeRotFromX(TargetLoc);


				CurrentRot = CameraComp->GetComponentRotation();
				AutoTransitRot = UKismetMathLibrary::RInterpTo(CurrentRot, TargetRot, DeltaTime, RintSpeed);
				PController->SetControlRotation(AutoTransitRot);

			}
			// Tag : Target�� �ƴ� ������Ʈ�� ������� ���
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
			}


		}
	}





}