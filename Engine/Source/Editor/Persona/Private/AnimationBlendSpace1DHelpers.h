// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

struct FIndexLinePoint
{
	float Position;
	int32 Index;
	FIndexLinePoint(const float InPosition, const int32 InIndex) : Position(InPosition), Index(InIndex) {}
};

struct FLineElement
{
	// Explicit constructor since we need to populate the Range value
	explicit FLineElement(const FIndexLinePoint& InStart, const FIndexLinePoint& InEnd) : Start(InStart), End(InEnd), bIsFirst(false), bIsLast(false)
	{
		Range = End.Position - Start.Position;
	}

	bool PopulateElement(const float ElementPosition, struct FEditorElement& InOutElement) const;
	bool IsBlendInputOnLine(const FVector& BlendInput) const
	{
		return (BlendInput.X >= Start.Position) && (BlendInput.X <= End.Position);
	}

	const FIndexLinePoint Start;
	const FIndexLinePoint End;
	bool bIsFirst;
	bool bIsLast;
	float Range;
};

/** Generates a line list between the supplied sample points to
aid blend space sample generation */
class FLineElementGenerator
{
public:
	void Init(const struct FBlendParameter& BlendParameter);

	/** Populates EditorElements based on the Sample points previously supplied to AddSamplePoint */
	void CalculateEditorElements();
	
	/**
	* Data Structure for line generation
	* SamplePointList is the input data
	*/
	TArray<float> SamplePointList;

	/** Editor elements generated by CalculateEditorElements */
	TArray<struct FEditorElement> EditorElements;

private:

	/** Defines the range of the editor */
	float MinGridValue;
	float MaxGridValue;

	/** Number of points that we have to generate FEditorElements for */
	int32 NumGridPoints;
	int32 NumGridDivisions;

	/** Line elements generated from the given samples */
	TArray<FLineElement> LineElements;
};