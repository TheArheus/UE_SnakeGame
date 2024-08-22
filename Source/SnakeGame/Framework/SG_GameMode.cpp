// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/SG_GameMode.h"
#include "Framework/SG_Camera.h"
#include "World/SG_Grid.h"
#include "World/SG_Snake.h"
#include "World/SG_Food.h"
#include "UI/SG_HUD.h"
#include "SG_GameMode.h"

#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Kismet/GameplayStatics.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All);

ASG_GameMode::ASG_GameMode() 
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASG_GameMode::StartPlay()
{
	Super::StartPlay();

	game::settings GameSettings = {};
	GameSettings.GridSize = { GridSize.X, GridSize.Y };
	GameSettings.Snake.DefaultSize = SnakeDefaultSize;
	GameSettings.Snake.StartPosition = {GridSize.X / 2, GridSize.Y / 2};
	Game = MakeUnique<game::Game>(GameSettings);
	check(Game.IsValid());

	const FTransform GridOrigin = FTransform::Identity;
	check(GetWorld());
	GridVisual = GetWorld()->SpawnActorDeferred<ASG_Grid>(GridVisualClass, GridOrigin);
	check(GridVisual);
	GridVisual->SetModel(Game->m_grid, CellSize);
	GridVisual->FinishSpawning(GridOrigin);

	SnakeVisual = GetWorld()->SpawnActorDeferred<ASG_Snake>(SnakeVisualClass, GridOrigin);
	SnakeVisual->SetModel(Game->m_snake, CellSize, Game->m_grid->c_dim);
	SnakeVisual->FinishSpawning(GridOrigin);

	FoodVisual = GetWorld()->SpawnActorDeferred<ASG_Food>(FoodVisualClass, GridOrigin);
	FoodVisual->SetModel(Game->m_food, CellSize, Game->m_grid->c_dim);
	FoodVisual->FinishSpawning(GridOrigin);

	auto* Player = GetWorld()->GetFirstPlayerController();
	check(Player);

	HUD = Cast<ASG_HUD>(Player->GetHUD());
	check(HUD);

	auto* Camera = Cast<ASG_Camera>(Player->GetPawn());
	check(Camera);
	check(Game->m_grid.IsValid());
	Camera->UpdateLocation(Game->m_grid->c_dim, CellSize, GridOrigin);

	FindFog();

	check(ColorsTable);
	check(ColorsTable->GetRowNames().Num() >= 1);
	UpdateColors();

	SetupInput();
	SubscribeOnGameEvents();
}

void ASG_GameMode::Tick(float dt)
{
	Super::Tick(dt);

	if(Game.IsValid())
	{
		Game->Update(dt, SnakeInput);
		HUD->SetGameTime(Game->m_gameTime);
		HUD->SetGameScore(Game->m_score);
	}
}

void ASG_GameMode::NextColor()
{
	if(ColorsTable)
		ColorTableIndex = (ColorTableIndex + 1) % ColorsTable->GetRowNames().Num();
	UpdateColors();
}

void ASG_GameMode::FindFog()
{
	TArray<AActor*> Fogs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExponentialHeightFog::StaticClass(), Fogs);
	if (Fogs.Num())
		Fog = Cast<AExponentialHeightFog>(Fogs[0]);
}

void ASG_GameMode::UpdateColors()
{
	const auto RowName   = ColorsTable->GetRowNames()[ColorTableIndex];
	const auto* ColorSet = ColorsTable->FindRow<FSnakeColors>(RowName, {});

	if (ColorSet) 
	{
		GridVisual->UpdateColors(*ColorSet);
		SnakeVisual->UpdateColors(*ColorSet);
		FoodVisual->UpdateColor(ColorSet->FoodColor);

		if (Fog && Fog->GetComponent())
		{
			Fog->GetComponent()->SkyAtmosphereAmbientContributionColorScale = ColorSet->SkyAtmosphereColor;
			Fog->MarkComponentsRenderStateDirty();
		}
	}
}

void ASG_GameMode::SetupInput()
{
	if (!GetWorld()) return;

	if (auto* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (auto* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			InputSystem->AddMappingContext(InputMapping, 0);
		}

		auto* Input = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
		check(Input);
		Input->BindAction(MoveForwardInputAction, ETriggerEvent::Started, this, &ThisClass::OnMoveForward);
		Input->BindAction(MoveRightInputAction, ETriggerEvent::Started, this, &ThisClass::OnMoveRight);
		Input->BindAction(ResetGameInputAction, ETriggerEvent::Started, this, &ThisClass::OnGameReset);
	}
}

void ASG_GameMode::OnMoveForward(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (InputValue.X == 0.0) return;
	SnakeInput = FIntVector2{ 0, static_cast<int8>(InputValue.X) };
}

void ASG_GameMode::OnMoveRight(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();
	if (InputValue.X == 0.0) return;
	SnakeInput = FIntVector2{ static_cast<int8>(InputValue.X), 0 };
}

void ASG_GameMode::OnGameReset(const FInputActionValue& Value)
{
	if (Value.Get<bool>()) 
	{
		game::settings GameSettings = {};
		GameSettings.GridSize = { GridSize.X, GridSize.Y };
		GameSettings.Snake.DefaultSize = SnakeDefaultSize;
		GameSettings.Snake.StartPosition = { GridSize.X / 2 + 1, GridSize.Y / 2 + 1 };
		Game.Reset(new game::Game(GameSettings));

		check(Game.IsValid());

		SubscribeOnGameEvents();

		GridVisual->SetModel(Game->m_grid, CellSize);
		SnakeVisual->SetModel(Game->m_snake, CellSize, Game->m_grid->c_dim);
		FoodVisual->SetModel(Game->m_food, CellSize, Game->m_grid->c_dim);

		SnakeInput = {1, 0};

		NextColor();
	}
}

void ASG_GameMode::SubscribeOnGameEvents()
{
	using namespace game;

	Game->SubscribeOnGameplayEvent([&](GameplayEvent Event) 
		{
			switch (Event) 
			{
			case GameplayEvent::FoodTaken: 
			{
				FoodVisual->Explode();
			} break;
			case GameplayEvent::GameOver:
			{
				UE_LOG(LogGame, Display, TEXT("-------------- GAME OVER --------------"));
				UE_LOG(LogGame, Display, TEXT("-------------- SCORE: %i --------------"), Game->m_score);
				SnakeVisual->Explode();
			} break;
			case GameplayEvent::GameCompleted:
			{

			} break;
			}
		});
}
