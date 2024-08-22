// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Game.h"
#include "Core/Grid.h"
#include "Core/Snake.h"
#include "Core/Food.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All);

using namespace game;

Game::Game(const settings& NewSettings) : c_settings(NewSettings), GameOver(false)
{
	m_grid = MakeShared<Grid>(c_settings.GridSize);
	m_snake = MakeShared<Snake>(c_settings.Snake);
	m_food = MakeShared<Food>();

	m_grid->Update(m_snake->Body(), cell_type::snake);
	GenerateFood();
}

void game::Game::Update(float dt, const FIntVector2& Input)
{
	if (GameOver || !UpdateTime(dt)) return;

	m_snake->Update(Input);
	m_grid->Update(m_snake->Body(), cell_type::snake);

	if (m_grid->IfCollided(m_snake->Head(), cell_type::wall) || m_grid->IfCollided(m_snake->Head(), cell_type::snake))
	{
		GameOver = true;
		m_gameplayEventCallback(GameplayEvent::GameOver);
	}

	if (m_grid->IfCollided(m_snake->Head(), cell_type::food)) 
	{
		m_score++;
		m_gameplayEventCallback(GameplayEvent::FoodTaken);
		GenerateFood();
	}
}


bool game::Game::UpdateTime(float dt)
{
	m_gameTime += dt;
	m_moveSeconds += dt;
	if (m_moveSeconds < 0.1) return false;
	m_moveSeconds = 0.0f;
	return true;
}

void game::Game::GenerateFood()
{
	m_food->Pos = m_grid->GetRandomPos();
	m_grid->Update(m_food->Pos, cell_type::food);
	//m_gameplayEventCallback(GameplayEvent::GameCompleted);
}

void game::Game::SubscribeOnGameplayEvent(GameplayEventCallback Callback)
{
	m_gameplayEventCallback = Callback;
}
