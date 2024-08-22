// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Snake.h"

game::Snake::Snake(const game::settings::snake& NewSettings)
{
	const auto Start = NewSettings.StartPosition;
	for (uint32 i = 0; i < NewSettings.DefaultSize; ++i) 
	{
		Links.AddTail(FUintVector2{ Start.X - i, Start.Y });
	}
}

void game::Snake::Update(const FIntVector2& Input)
{
	Links.RemoveNode(Links.GetTail());
	Links.InsertNode(Links.GetHead()->GetValue(), Links.GetHead()->GetNextNode());

	FUintVector2& HeadPosition = Links.GetHead()->GetValue();

	HeadPosition.X = static_cast<uint32>(static_cast<int32>(HeadPosition.X) + Input.X);
	HeadPosition.Y = static_cast<uint32>(static_cast<int32>(HeadPosition.Y) + Input.Y);
}
