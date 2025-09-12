// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EStanceState.generated.h"

UENUM(BlueprintType)
enum class EStanceState: uint8
{
	Stand,
	Crouch,
	Sprint, 
	Prone
};