#pragma once

#include "SnakeGame/Core/Intrinsics.h"
#include "CoreMinimal.h"

namespace game
{
	class WorldUtils 
	{
	public:
		static FVector LinkPositionToVector(const FUintVector2& Pos, uint32 CellSize, const game::dim& Dims)
		{
			return FVector((Dims.Height - 1.0 - Pos.Y) * CellSize, Pos.X * CellSize, 0.0) + FVector(CellSize * 0.5);
		}

		static FText FormatSeconds(double Time) 
		{
			const int32 TotalSeconds = FMath::RoundToInt(Time);
			// const int32 Hours = TotalSeconds / 3600;
			const int32 Minutes = (TotalSeconds % 3600) / 60;
			const int32 Seconds = TotalSeconds % 60;
			const FString FormattedTime = FString::Printf(TEXT("%02i:%02i"), Minutes, Seconds);
			return FText::FromString(FormattedTime);
		}

		static FText FormatScore(uint32 Score)
		{
			const FString FormattedScore = FString::Printf(TEXT("%02i"), Score);
			return FText::FromString(FormattedScore);
		}
	};
};