// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "SceneCaptureCubeHDRRenderPrivatePCH.h"
#include "SceneCaptureCubeHDRRenderBPLibrary.h"

#define LOCTEXT_NAMESPACE "USceneCaptureCubeHDRRenderBPLibrary"

USceneCaptureCubeHDRRenderBPLibrary::USceneCaptureCubeHDRRenderBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void USceneCaptureCubeHDRRenderBPLibrary::ExportRenderTargetCube(UObject* WorldContextObject, UTextureRenderTargetCube* TextureRenderTarget, const FString& FilePath, const FString& FileName)
{
	FString TotalFileName = FPaths::Combine(*FilePath, *FileName);
	FText PathError;
	FPaths::ValidatePath(TotalFileName, &PathError);

	if (TextureRenderTarget && !FileName.IsEmpty() && PathError.IsEmpty())
	{
		FArchive* Ar = IFileManager::Get().CreateFileWriter(*TotalFileName);

		if (Ar)
		{
			FBufferArchive Buffer;
			bool bSuccess = FImageUtils::ExportRenderTargetCubeAsHDR(TextureRenderTarget, Buffer);

			if (bSuccess)
			{
				Ar->Serialize(const_cast<uint8*>(Buffer.GetData()), Buffer.Num());
			}

			delete Ar;
		}
		else
		{
			FMessageLog("Blueprint").Warning(LOCTEXT("ExportRenderTarget_FileWriterFailedToCreate", "ExportRenderTarget: FileWrite failed to create."));
		}
	}

	else if (!TextureRenderTarget)
	{
		FMessageLog("Blueprint").Warning(LOCTEXT("ExportRenderTarget_InvalidTextureRenderTarget", "ExportRenderTarget: TextureRenderTarget must be non-null."));
	}
	if (!PathError.IsEmpty())
	{
		FMessageLog("Blueprint").Warning(FText::Format(LOCTEXT("ExportRenderTarget_InvalidFilePath", "ExportRenderTarget: Invalid file path provided: '{0}'"), PathError));
	}
	if (FileName.IsEmpty())
	{
		FMessageLog("Blueprint").Warning(LOCTEXT("ExportRenderTarget_InvalidFileName", "ExportRenderTarget: FileName must be non-empty."));
	}
}

void USceneCaptureCubeHDRRenderBPLibrary::ProcessAndExport2DSet(UObject* WorldContextObject, USceneCaptureComponent2D* SceneCapture, float EyeDistance, int NumLongitudinalSections, int NumLatitudinalSections, int RenderTargetXResolution, int RenderTargetYResolution, const FString& FilePath, const FString& BaseFileName)
{
	float HorizontalStep = 360.0 / (NumLongitudinalSections);
	float VerticalStep = 180.0 / (NumLatitudinalSections-1); /* one less division because the first and last images will not be the same view, (due to not rotating completely about Y axis) */
	float EyeDistanceFromCenter = EyeDistance / 2.0;

	FVector RootLocation = SceneCapture->GetComponentLocation();
	FRotator RootRotation = SceneCapture->GetComponentRotation();

	for (int i = 0; i < NumLongitudinalSections; i++)
	{
		/*  Look straight down and pivot about Z axis */
		SceneCapture->SetWorldRotation(*(new FRotator(-90.0, HorizontalStep*i, 0.0)));

		for (int j = 0; j < NumLatitudinalSections; j++)
		{
			UTextureRenderTarget2D* LeftEyeRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(WorldContextObject, RenderTargetXResolution, RenderTargetYResolution);
			UTextureRenderTarget2D* RightEyeRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(WorldContextObject, RenderTargetXResolution, RenderTargetYResolution);

			/* Render right eye */
			SceneCapture->TextureTarget = LeftEyeRenderTarget;
			FString CompleteFileName = BaseFileName + FString::Printf(TEXT("_Right_%d_%d.hdr"), i, j);
			SceneCapture->AddLocalOffset(*(new FVector(0.0, EyeDistanceFromCenter, 0.0)));
			SceneCapture->CaptureScene();
			UKismetRenderingLibrary::ExportRenderTarget(WorldContextObject, LeftEyeRenderTarget, FilePath, CompleteFileName);
			
			/* Render left eye */
			SceneCapture->TextureTarget = RightEyeRenderTarget;
			CompleteFileName = BaseFileName + FString::Printf(TEXT("_Left_%d_%d.hdr"), i, j);
			SceneCapture->AddLocalOffset(*(new FVector(0.0, -EyeDistance, 0.0)));
			SceneCapture->CaptureScene();
			UKismetRenderingLibrary::ExportRenderTarget(WorldContextObject, RightEyeRenderTarget, FilePath, CompleteFileName);

			/* Return Scene Capture to root location */
			SceneCapture->SetWorldLocation(RootLocation);

			/* Look up in vertical step increments */
			SceneCapture->AddLocalRotation(*(new FRotator(VerticalStep, 0.0, 0.0)));
		}
	}
}



