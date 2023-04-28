// Fill out your copyright notice in the Description page of Project Settings.


#include "RunAnimInstance1.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void URunAnimInstance1::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (IsValid(Pawn))
	{
		MovementComponent = Pawn->GetMovementComponent();
	}
}

void URunAnimInstance1::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Pawn && MovementComponent)
	{
		bIsInAir = MovementComponent->IsFalling();
		Speed = Pawn->GetVelocity().Size();
	}
}
