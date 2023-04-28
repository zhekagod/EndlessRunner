// Copyright Epic Games, Inc. All Rights Reserved.


#include "EndlessRunnerGameModeBase.h"
#include "FloorTile.h"
#include "GameHud.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


void AEndlessRunnerGameModeBase::BeginPlay()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

	GameHud = Cast<UGameHud>(CreateWidget(GetWorld(), GameHudClass));
	check(GameHud);

	//Инициальзация пользовательского интерфейса и его вывод
	GameHud->InitializeHUD(this);
	GameHud->AddToViewport();
	//создание первых 3 плиток без препятствий

	NumberOfLives = MaxLives;

	CreateIntitalFloorTiles();
}


void AEndlessRunnerGameModeBase::CreateIntitalFloorTiles()
{
	AFloorTile* Tile = AddFloorTile(false);

	if (Tile)
	{
		LaneSwitchValues.Add(Tile->LeftLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->CenterLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->RightLane->GetComponentLocation().Y);
	}

	/*for (float Val : LaneSwitchValues)
	{
		UE_LOG(LogTemp, Warning, TEXT("LANE VALUE: %f"), Val);
	}*/

	AddFloorTile(false); // 2 начальные плитки без препятствий
	AddFloorTile(false);

	for (int i = 0; i < NumInitialFloorTiles; i++)
	{
		AddFloorTile(true);
	}
}

AFloorTile* AEndlessRunnerGameModeBase::AddFloorTile(const bool bSpawnItems)
{
	UWorld* World = GetWorld(); //Указатель на объект текущей локации

	if (World) //Если мир непустой и загружен, то...
	{
		AFloorTile* Tile = GetWorld()->SpawnActor<AFloorTile>(FloorTileClass, NextSpawnPoint);
		if (Tile)
		{
			FloorTiles.Add(Tile);

			if (bSpawnItems)
			{
				Tile->SpawnItems();
			}

			NextSpawnPoint = Tile->GetAttachTransform();
		}

		return Tile;
	}

	return nullptr;
	
}

void AEndlessRunnerGameModeBase::AddCoin()
{
	//обновление счётчика монет и их вывод на экран
	TotalCoins++;
	OnCoinsCountChanged.Broadcast(TotalCoins);
	//UE_LOG(LogTemp, Warning, TEXT("TOTAL COINS %d"), TotalCoins);
}

void AEndlessRunnerGameModeBase::PlayerDied()
{
	NumberOfLives -= 1;
	OnLivesCountChanged.Broadcast(NumberOfLives);

	if (NumberOfLives > 0)
	{
		// Interate all FloorTiles and call DestroyFloorTile
		for (auto Tile : FloorTiles)
		{
			Tile->DestroyFloorTile();
		}

		// Empty our array
		FloorTiles.Empty();

		// Set NextSpawnPoint to initial value
		NextSpawnPoint = FTransform();

		// Create out Initial floor tiles
		CreateIntitalFloorTiles();

		// Broadcast level reset event
		OnLevelReset.Broadcast();
	}
	else
	{
		GameOver();
	}
}

void AEndlessRunnerGameModeBase::GameOver()
{
	if (IsValid(GameOverScreenClass))
	{
		UUserWidget* Widget = CreateWidget(GetWorld(), GameOverScreenClass);

		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
}

void AEndlessRunnerGameModeBase::RemoveTile(AFloorTile* Tile)
{
	FloorTiles.Remove(Tile);
}
