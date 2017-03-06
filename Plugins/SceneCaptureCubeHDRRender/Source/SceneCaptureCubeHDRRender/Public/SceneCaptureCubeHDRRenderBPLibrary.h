// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "ImageUtils.h"
#include "MessageLog.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTargetCube.h"
#include "Runtime/Engine/Classes/Kismet/KismetRenderingLibrary.h"
#include "Runtime/Engine/Classes/Engine/SceneCapture2D.h"

#include "SceneCaptureCubeHDRRenderBPLibrary.generated.h"

UCLASS()
class USceneCaptureCubeHDRRenderBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "RenderingPlugin", meta = (Keywords = "ExportRenderTargetCube", WorldContext = "WorldContextObject"))
		static void ExportRenderTargetCube(UObject* WorldContextObject, UTextureRenderTargetCube* TextureRenderTarget, const FString& FilePath, const FString& FileName);

	UFUNCTION(BlueprintCallable, Category = "RenderingPlugin", meta = (Keywords = "ProcessAndExport2DSet", WorldContext = "WorldContextObject"))
		static void CaptureAndExport2DCaptureSet(
			UObject* WorldContextObject, 
			USceneCaptureComponent2D* SceneCapture, 
			float EyeDistance, 
			int NumLongitudinalSections, 
			int NumLatitudinalSections, 
			int RenderTargetXResolution, 
			int RenderTargetYResolution,
			float FOVAngle,
			const FString& FilePath, 
			const FString& BaseFileName,
			int FrameNum);
};