// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Intrinsics.h"

namespace game
{
	class SNAKEGAME_API Snake
	{
	public:
		Snake(const game::settings::snake& NewSettings);

		const FUintVector2 Head() const { return Links.GetHead()->GetValue(); }
		const SnakePtr* Body() const { return Links.GetHead()->GetNextNode(); }
		void AddTail() { Links.AddTail(Links.GetTail()->GetValue()); }

		void Update(const FIntVector2& Input);

		SnakeList Links;
	};
};