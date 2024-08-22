// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Intrinsics.h"


namespace game
{
	class Grid;
	class Snake;
	struct Food;
	class SNAKEGAME_API Game
	{
	public:
		Game(const settings& NewSettings);

		void Update(float dt, const FIntVector2& Input);
		bool UpdateTime(float dt);

		void GenerateFood();

		void SubscribeOnGameplayEvent(GameplayEventCallback Callback);

		TSharedPtr<Grid> m_grid;
		TSharedPtr<Snake> m_snake;
		TSharedPtr<Food> m_food;

		uint32 m_score = 0;
		double m_gameTime = 0.0;

	private:
		GameplayEventCallback m_gameplayEventCallback;

		const settings c_settings;
		bool GameOver;

		double m_moveSeconds = 0.0;
	};
};
