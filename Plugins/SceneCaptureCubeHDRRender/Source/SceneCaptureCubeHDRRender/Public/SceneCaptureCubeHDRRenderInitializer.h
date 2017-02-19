// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "SceneCaptureCubeHDRRenderInitializer.generated.h"

UCLASS()
class USceneCaptureCubeHDRRenderInitializer : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY()
		class UObject* WorldContextObject;
	UPROPERTY()
		class USceneCaptureComponent2D* SceneCapture;
	UPROPERTY()
		float EyeDistance;
	UPROPERTY()
		int NumLongitudinalSections;
	UPROPERTY()
		int NumLatitudinalSections;
	UPROPERTY()
		int RenderTargetXResolution;
	UPROPERTY()
		int RenderTargetYResolution;
	UPROPERTY()
		FString FilePath;
	UPROPERTY()
		FString BaseFileName;
};