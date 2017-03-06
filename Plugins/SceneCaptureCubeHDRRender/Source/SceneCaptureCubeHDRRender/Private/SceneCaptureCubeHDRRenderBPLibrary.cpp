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
			FMessageLog("Blueprint").Warning(LOCTEXT("ExportRenderTargetCube_FileWriterFailedToCreate", "ExportRenderTargetCube: FileWrite failed to create."));
		}
	}

	else if (!TextureRenderTarget)
	{
		FMessageLog("Blueprint").Warning(LOCTEXT("ExportRenderTargetCube_InvalidTextureRenderTarget", "ExportRenderTargetCube: TextureRenderTarget must be non-null."));
	}
	if (!PathError.IsEmpty())
	{
		FMessageLog("Blueprint").Warning(FText::Format(LOCTEXT("ExportRenderTargetCube_InvalidFilePath", "ExportRenderTargetCube: Invalid file path provided: '{0}'"), PathError));
	}
	if (FileName.IsEmpty())
	{
		FMessageLog("Blueprint").Warning(LOCTEXT("ExportRenderTargetCube_InvalidFileName", "ExportRenderTargetCube: FileName must be non-empty."));
	}
}

void USceneCaptureCubeHDRRenderBPLibrary::CaptureAndExport2DCaptureSet(
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
	int FrameNum)
{
	FString FilePathToValidate = FPaths::Combine(*FilePath, *BaseFileName);
	FText PathError;
	FPaths::ValidatePath(FilePathToValidate, &PathError);

	if (NumLongitudinalSections > 0 || NumLatitudinalSections > 0 && RenderTargetXResolution > 0 && RenderTargetYResolution > 0 && !BaseFileName.IsEmpty() && PathError.IsEmpty())
	{
		float HorizontalStep = 360.0 / (NumLongitudinalSections);
		float VerticalStep = 180.0 / (NumLatitudinalSections - 1); /* one less division because the first and last images will not be the same view, (Rotaes about Y axis from 0 to 180) */
		float EyeDistanceFromCenter = EyeDistance / 2.0;
		SceneCapture->FOVAngle = FOVAngle;

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
				FString CompleteFileName = BaseFileName + FString::Printf(TEXT("_%d_X%05d_Y%05d_R.hdr"), FrameNum, i, j);
				SceneCapture->AddLocalOffset(*(new FVector(0.0, EyeDistanceFromCenter, 0.0)));
				SceneCapture->CaptureScene();
				UKismetRenderingLibrary::ExportRenderTarget(WorldContextObject, LeftEyeRenderTarget, FilePath, CompleteFileName);

				/* Render left eye */
				SceneCapture->TextureTarget = RightEyeRenderTarget;
				CompleteFileName = BaseFileName + FString::Printf(TEXT("_%d_X%05d_Y%05d_L.hdr"), FrameNum, i, j);
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

	else if (!PathError.IsEmpty())
	{
		FMessageLog("Blueprint").Warning(FText::Format(LOCTEXT("ProcessAndExport2DSet_InvalidFilePath", "ProcessAndExport2DSet: Invalid file path provided: '{0}'"), PathError));
	}
	if (BaseFileName.IsEmpty())
	{
		FMessageLog("Blueprint").Warning(LOCTEXT("ProcessAndExport2DSet_InvalidFileName", "ProcessAndExport2DSet: FileName must be non-empty."));
	}
	if (NumLatitudinalSections <= 0)
	{
		FMessageLog("Blueprint").Warning(LOCTEXT("ProcessAndExport2DSet_InvalidLatitudinalSections", "ProcessAndExport2DSet: NumLatitudinalSections must be greater than 0."));
	}
	if (NumLongitudinalSections <= 0)
	{
		FMessageLog("Blueprint").Warning(LOCTEXT("ProcessAndExport2DSet_InvalidLatitudinalSections", "ProcessAndExport2DSet: NumLongitudinalSections must be greater than 0."));
	}
	if (RenderTargetXResolution <= 0)
	{
		FMessageLog("Blueprint").Warning(LOCTEXT("ProcessAndExport2DSet_InvalidXResolution", "ProcessAndExport2DSet: RenderTargetXResolution must be greater than 0."));
	}
	if (RenderTargetYResolution <= 0)
	{
		FMessageLog("Blueprint").Warning(LOCTEXT("ProcessAndExport2DSet_InvalidYResolution", "ProcessAndExport2DSet: RenderTargetYResolution must be greater than 0."));
	}
}



