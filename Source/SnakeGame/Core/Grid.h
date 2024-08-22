// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Intrinsics.h"

namespace game
{
	class SNAKEGAME_API Grid
	{
		void InitWalls();

	public:
		Grid(const dim& NewDim);

		void Update(const SnakePtr* Links, cell_type Type);
		void Update(const FUintVector2& Pos, cell_type Type);

		bool IfCollided(const FUintVector2& Pos, cell_type Type);

		FUintVector2 GetRandomPos();

		const dim c_dim;

	private:
		uint32 PosToIndex(uint32 X, uint32 Y) { return X + Y * c_dim.Width; }
		uint32 PosToIndex(const FUintVector2& Pos) { return PosToIndex(Pos.X, Pos.Y); }
		FUintVector2 IndexToPos(uint32 I) { return FUintVector2(I % c_dim.Width, I / c_dim.Width); }

		void FreeCellsByType(cell_type Type);

		TArray<cell_type> m_cells;
		TMap<cell_type, TArray<uint32>> m_indByType = {
			{cell_type::snake, {}},  //
			{cell_type::wall, {}},   //
			{cell_type::food, {}},   //
		};
	};
};

