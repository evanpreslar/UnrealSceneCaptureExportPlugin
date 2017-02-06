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

void USceneCaptureCubeHDRRenderBPLibrary::ExportRenderTarget2DSet(UObject* WorldContextObject, UTextureRenderTargetCube* TextureRenderTarget, const FString& FilePath, const FString& FileName)
{

}



