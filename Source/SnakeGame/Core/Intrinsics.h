// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


namespace game
{
	struct dim 
	{
		uint32 Width;
		uint32 Height;
	};

	enum class cell_type 
	{
		empty = 0,
		wall,
		snake,
		food
	};

	struct settings 
	{
		struct snake 
		{
			uint32 DefaultSize = 4;
			FUintVector2 StartPosition{ 0, 0 };
		};

		dim GridSize;
		snake Snake;
	};


	using SnakeList = TDoubleLinkedList<FUintVector2>;
	using SnakePtr = SnakeList::TDoubleLinkedListNode;

	enum class GameplayEvent 
	{
		GameOver = 0,
		GameCompleted,
		FoodTaken,
	};

	using GameplayEventCallback = TFunction<void(GameplayEvent)>;
};