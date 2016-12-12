// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/SceneCaptureCube.h"
#include "MySceneCaptureCube.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT2_API AMySceneCaptureCube : public ASceneCaptureCube
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "Rendering")
	bool Test();
	
};
