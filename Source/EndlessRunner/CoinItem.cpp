// Fill out your copyright notice in the Description page of Project Settings.

#include "CoinItem.h"

#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
//#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Kismet/GameplayStatics.h"
#include "RunCharacter.h"


#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "EndlessRunnerGameModeBase.h"
#include <EndlessRunner/RunCharacter.h>
#include "Obstacle.h"
#include "Kismet/KismetMathLibrary.h"



// Sets default values
ACoinItem::ACoinItem()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(SceneComponent);
	SphereCollider->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoinMesh"));
	CoinMesh->SetupAttachment(SphereCollider);
	CoinMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->RotationRate = FRotator(0, 180, 0);

}

// Called when the game starts or when spawned
void ACoinItem::BeginPlay()
{
	Super::BeginPlay();
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this,&ACoinItem::OnSphereOverlap);
}

void ACoinItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);

	UWorld* World = GetWorld();
	
	if (RunCharacter)
	{
		if (OverlapSound)
		{
			UGameplayStatics::PlaySoundAtLocation(World, OverlapSound, GetActorLocation());
			//UGameplayStatics::PlaySoundAtLocation(World, OverlapSound, GetActorLocation()); //без удачной компиляции
		}
		RunCharacter->AddCoin();
		Destroy();
	}
}

