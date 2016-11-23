// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Matinee/InterpTrack.h"
#include "InterpTrackDirector.generated.h"

class FCanvas;
class UInterpGroup;
class UInterpTrackInst;

/**
 *
 * A track type used for binding the view of a Player (attached to this tracks group) to the actor of a different group.
 *
 */

/** Information for one cut in this track. */
USTRUCT()
struct FDirectorTrackCut
{
	GENERATED_USTRUCT_BODY()

	/** Time to perform the cut. */
	UPROPERTY()
	float Time;

	/** Time taken to move view to new camera. */
	UPROPERTY()
	float TransitionTime;

	/** GroupName of UInterpGroup to cut viewpoint to. */
	UPROPERTY(EditAnywhere, Category=DirectorTrackCut)
	FName TargetCamGroup;

	/** Shot number for developer reference */
	UPROPERTY()
	int32 ShotNumber;


	FDirectorTrackCut()
		: Time(0)
		, TransitionTime(0)
		, ShotNumber(0)
	{
	}

};

UCLASS(MinimalAPI, meta=( DisplayName = "Director Track" ) )
class UInterpTrackDirector : public UInterpTrack
{
	GENERATED_UCLASS_BODY()

	/** Array of cuts between cameras. */
	UPROPERTY()
	TArray<struct FDirectorTrackCut> CutTrack;


private:
	/** True to allow clients to simulate their own camera cuts.  Can help with latency-induced timing issues. */
	UPROPERTY(EditAnywhere, Category=InterpTrackDirector)
	uint32 bSimulateCameraCutsOnClients:1;

#if WITH_EDITORONLY_DATA
	/** The camera actor which the track is currently focused on. Only valid if this track or it's group is selected */
	UPROPERTY(Transient)
	class ACameraActor* PreviewCamera;
#endif // WITH_EDITORONLY_DATA

public:
	//~ Begin UObject Interface
	virtual void PostLoad() override;
	//~ End UObject Interface
	
	//~ Begin UInterpTrack Interface
	virtual int32 GetNumKeyframes() const override;
	virtual void GetTimeRange( float& StartTime, float& EndTime ) const override;
	virtual float GetTrackEndTime() const override;
	virtual float GetKeyframeTime( int32 KeyIndex ) const override;
	virtual int32 GetKeyframeIndex( float KeyTime ) const override;
	virtual int32 AddKeyframe( float Time, UInterpTrackInst* TrackInst, EInterpCurveMode InitInterpMode ) override;
	virtual int32 SetKeyframeTime( int32 KeyIndex, float NewKeyTime, bool bUpdateOrder = true ) override;
	virtual void RemoveKeyframe( int32 KeyIndex ) override;
	virtual int32 DuplicateKeyframe(int32 KeyIndex, float NewKeyTime, UInterpTrack* ToTrack = NULL) override;
	virtual bool GetClosestSnapPosition( float InPosition, TArray<int32>& IgnoreKeys, float& OutPosition ) override;
	virtual void PreviewUpdateTrack(float NewPosition, class UInterpTrackInst* TrackInst) override;
	virtual void UpdateTrack( float NewPosition, UInterpTrackInst* TrackInst, bool bJump ) override;
	virtual const FString GetEdHelperClassName() const override;
	virtual const FString GetSlateHelperClassName() const override;
#if WITH_EDITORONLY_DATA
	virtual class UTexture2D* GetTrackIcon() const override;
#endif // WITH_EDITORONLY_DATA
	virtual void DrawTrack( FCanvas* Canvas, UInterpGroup* Group, const FInterpTrackDrawParams& Params ) override;
	//~ End UInterpTrack Interface

	/** Get the keyframe index nearest the time, without going over */
	int32 GetNearestKeyframeIndex( float KeyTime ) const;

	/** For the supplied time, find which group name we should be viewing from. */
	FName GetViewedGroupName(float CurrentTime, float& CutTime, float& CutTransitionTime);

	/** For the supplied time, get the name of our camera shot */
	ENGINE_API const FString GetViewedCameraShotName(float CurrentTime) const;
	
	/** 
	 * Get an autogenerated Camera Shot name for a given key frame index 
	 * @param KeyIndex   The camera cut key index
	 * @return   String of the newly generated shot name
	 */
	const int32 GenerateCameraShotNumber(int32 KeyIndex) const;
	const FString GetFormattedCameraShotName(int32 KeyIndex) const;

#if WITH_EDITOR
	/** Get the preview camera actor, if any */
	class ACameraActor* GetPreviewCamera() const { return PreviewCamera; }

	/** 
	 * Update the preview camera actor based on track selection
	 *
	 * @param MatineeActor  The matinee actor we need to get the viewed actor of
	 * @param bIsSelected   Whether or not the director track/group is selected
	 * @return	true if the actor is valid and has just changed		
	 */
	ENGINE_API bool UpdatePreviewCamera(class AMatineeActor* MatineeActor, bool bIsSelected);
#endif // WITH_EDITOR
};



