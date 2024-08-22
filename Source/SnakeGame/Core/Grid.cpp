// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Grid.h"

using namespace game;

Grid::Grid(const dim& NewDim) : c_dim(dim{NewDim.Width + 2, NewDim.Height + 2})
{
	m_cells.Init(cell_type::empty, c_dim.Width * c_dim.Height);
	InitWalls();
}

void game::Grid::Update(const SnakePtr* Links, cell_type Type)
{
	FreeCellsByType(Type);

	const SnakePtr* Link = Links;
	while(Link)
	{
		uint32 Index = PosToIndex(Link->GetValue());
		m_cells[Index] = Type;
		m_indByType[Type].Add(Index);
		Link = Link->GetNextNode();
	}
}

void game::Grid::Update(const FUintVector2& Pos, cell_type Type)
{
	FreeCellsByType(Type);

	uint32 Index = PosToIndex(Pos);
	m_cells[Index] = Type;
	m_indByType[Type].Add(Index);
}

bool game::Grid::IfCollided(const FUintVector2& Pos, cell_type Type)
{
	uint32 Index = PosToIndex(Pos);
	return m_cells[Index] == Type;
}

FUintVector2 game::Grid::GetRandomPos()
{
	const uint32 GridSize = c_dim.Width * c_dim.Height;
	const uint32 Index = FMath::RandRange(0, GridSize - 1);

	for (uint32 i = Index; i < GridSize; i++) 
	{
		if (m_cells[i] == cell_type::empty) return IndexToPos(Index);
	}

	for (uint32 i = 0; i < Index; i++)
	{
		if (m_cells[i] == cell_type::empty) return IndexToPos(Index);
	}

	return FUintVector2();
}

void Grid::InitWalls() 
{
	for (uint32 x = 0; x < c_dim.Width; x++) 
	{
		for (uint32 y = 0; y < c_dim.Height; y++) 
		{
			if (x == 0 || x == c_dim.Width - 1 || y == 0 || y == c_dim.Height - 1) 
			{
				uint32 Index = PosToIndex(x, y);
				m_cells[Index] = cell_type::wall;
				m_indByType[cell_type::wall].Add(Index);
			}
		}
	}
}

void Grid::FreeCellsByType(cell_type Type)
{
	for (int32 i = 0; i < m_indByType[Type].Num(); ++i)
	{
		const uint32 Index = m_indByType[Type][i];
		m_cells[Index] = cell_type::empty;
	}
	m_indByType[Type].Empty();
}