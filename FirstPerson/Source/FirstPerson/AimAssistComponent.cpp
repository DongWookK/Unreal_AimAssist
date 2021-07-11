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
Brief: TickComponent 내부에서 컨트롤러(크로스헤어 위치)의 변경을 담당하는 함수
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
Brief: 캐릭터가 우클릭으로 조준 시 Target에 해당하는 액터를 식별해 해당 액터로 자동 조준해주는 함수
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

	//3인칭 액터인만큼 
	CameraLoc = CameraComp->GetComponentLocation();
	TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2);
	TArray<AActor*> ActorsToIgnore{ this->GetOwner() };


	//디버그시 EDrawDebugTrace의 Type변경 요함
	//현재 Capsule 트레이스의 시작점점에서 바로 bBlockingHit 되어 트레이스되지 않는 문제 발생. 보정 필요.
	//Bullet이 아래 캡슐 트레이스에서 무시되게끔 Collision Ignore 처리 필요
	if (UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), CameraLoc, End, 75.0f, 25.0f, TraceType, false, ActorsToIgnore, EDrawDebugTrace::Type::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, 1.0f))
	{
		if (OutHit.bBlockingHit)
		{
			// Tag : Target에 해당하는 액터를 검출했을 경우
			if (OutHit.GetActor()->ActorHasTag(TEXT("Target")))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
				TargetLoc = OutHit.GetActor()->GetActorLocation();
				//노멀라이제이션을 통해 오차허용, Aim과 함께 Fire 함수 실행 시 끊김현상 제거
				TargetLoc = (TargetLoc - OutHit.ImpactPoint).GetSafeNormal(0.0001);
				TargetRot = UKismetMathLibrary::MakeRotFromX(TargetLoc);


				CurrentRot = CameraComp->GetComponentRotation();
				AutoTransitRot = UKismetMathLibrary::RInterpTo(CurrentRot, TargetRot, DeltaTime, RintSpeed);
				PController->SetControlRotation(AutoTransitRot);

			}
			// Tag : Target이 아닌 오브젝트가 검출됐을 경우
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
			}


		}
	}





}