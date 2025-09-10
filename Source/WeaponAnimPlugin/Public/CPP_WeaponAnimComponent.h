// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "WeaponBobStruct.h"
#include "WeaponSwayStruct.h"
#include "WeaponRecoilStruct.h"
#include "CPP_WeaponAnimComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class WEAPONANIMPLUGIN_API UCPP_WeaponAnimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCPP_WeaponAnimComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float SqrtAlpha(float DeltaTime, float Rate);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	float ElapsedTime = 0.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool PlayAnimationOnStart = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Roots")
	USceneComponent *WeaponRoot = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Roots")
	USceneComponent *Sight = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Roots")
	USceneComponent *CameraRoot = nullptr;
	UFUNCTION(BlueprintCallable)
	void Init(USceneComponent *WeaponRootToSet, USceneComponent *SightToSet, USceneComponent *CameraRootToSet);
	APlayerController *Controller = nullptr;
	void TrySetController();
	FVector CamInitialLocation;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateSettings();
	APawn *OwnerPawn = nullptr;
	UFUNCTION(BlueprintCallable)
	void StopAnimate();
	// 设置玩家输入数据
	UFUNCTION(BlueprintCallable)
	void SetInputVector(FVector Vector);
	UFUNCTION(BlueprintCallable)
	void SetInputRotator(FRotator Rotator);
	FVector2D InputVector2D;
	FVector InputVector;
	FRotator InputRotator;
	float MoveSize;
	// 结算
	FVector Result;
	FRotator RotationResult;
	FVector JitterVector(FVector Input, FVector Jitter);
	// 基准
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	TMap<FName, FTransform> BaseStates;
	// 默认基准名
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	FName DefaultBase = "IdleBase"; 
	// 在游戏开始时，将WeaponRoot的Transform赋值给这个Base
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	TArray<FName> InitializeBases = { "IdleBase" }; 
	// 当前基准参数
	FTransform TargetBaseTransform;
	UFUNCTION(BlueprintCallable)
	void SetBase(FName BaseName);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	float BaseLocationInterpolationRate = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
	float BaseRotationInterpolationRate = 5.f;
	FVector CurrentBaseLocation;
	FRotator CurrentBaseRotation;
	// 后坐力相关
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	TMap<FName, FWeaponRecoilStruct> RecoilStates;
	// 后坐力动画时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float RecoilAnimTime = 0.2f; 
	// 后坐力曲线
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	UCurveFloat *RecoilCurve; 
	// 后坐力震动
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	UForceFeedbackEffect *RecoilForceFeedbackEffect; 
	// 如果为否，会在Init时将WeaponRoot挂在到CameraManager的Transform Component下，使镜头摇晃等效果能够传递到WeaponRoot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Decouple Camera Manager Transform")
	bool DecoupleCamManagerTransform = false; 
	FName DefaultRecoil = "DefaultRecoil";
	FWeaponRecoilStruct CurrentRecoilStruct;
	float CurrentRecoilTime = 0.0f;
	// 后坐力位置偏移
	FVector RecoilTargetOffset;
	FVector GradualRecoilOffsetTarget;
	// ADS状态下这个变量会影响到准星偏移
	UPROPERTY(BlueprintReadOnly)
	FVector CurrentRecoilOffset;						   
	FRotator RecoilRotationResult = FRotator::ZeroRotator; // 后坐力旋转偏移
	FVector CurrentRecoilGradualOffset;		// 后坐力渐进偏移
	FVector RecoilRotationTargetOffset;		// 后坐力旋转偏移
	FRotator CurrentRecoilGradualRotOffset; // 后坐力旋转渐进偏移
	void UpdateRecoilEnd();
	void UpdateRecoil(float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void StartRecoilAnim();
	bool IsPlayingRecoilAnim = false;
	UFUNCTION(BlueprintCallable)
	void SetRecoil(FName RecoilName);
	// 武器晃动相关
	FVector CurrentBobResult = FVector::ZeroVector;
	FVector BobResult;
	FRotator CurrentBobResultRot;
	FRotator BobResultRot;
	void UpdateBob();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bob")
	float BobInterpolationRate = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bob")
	float BobRotationInterpolationRate = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bob")
	TMap<FName, FWeaponBobStruct> BobStates;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bob")
	FName DefaultBobStatic = "IdleBob";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bob")
	FName DefaultBobMovement = "WalkBob";
	FWeaponBobStruct CurrentStaticBob;
	FWeaponBobStruct CurrentMovementBob;
	FWeaponBobStruct CurrentBob;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bob")
	float BobMultiplier = 1.f;
	UFUNCTION(BlueprintCallable)
	void SetStaticBob(FName BobName);
	UFUNCTION(BlueprintCallable)
	void SetMovementBob(FName BobName);
	// Sway相关
	FRotator CurrentSway = FRotator::ZeroRotator;
	FRotator TargetSway = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sway")
	float SwayInterpolationRate = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sway")
	TMap<FName, FWeaponSwayStruct> SwayStates;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sway")
	FName DefaultSway = "DefaultSway";
	FWeaponSwayStruct CurrentSwayStruct;
	UFUNCTION(BlueprintCallable)
	void SetSway(FName SwayName);
	void UpdateSway();
	// MovementOffset相关
	FVector CurrentMovementOffset = FVector::ZeroVector;
	FVector TargetMovementOffset = FVector::ZeroVector;
	float CurrentMovementRotationOffset = 0.f;
	float TargetMovementRotationOffset = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementOffset")
	float MovementOffsetInterpolationRate = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementOffset")
	FVector MovementOffsetMax = FVector(2.f, 2.f, 5.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementOffset")
	float MovementRotationOffsetMax = 5.f;
	void UpdateMovementOffset();
	// 跳跃相关
	enum class EJumpState
	{
		Default,
		Start,
		MidAir,
		Land
	};
	EJumpState CurrentJumpState = EJumpState::Default;
	float CurrentJumpOffset = 0.f;
	float DefaultJumpOffset = 0.f;
	float *TargetJumpOffset = &DefaultJumpOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float JumpOffset = 8.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float MidAirOffset = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float LandOffset = -3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float JumpOffsetInterpolationRateUp = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float JumpOffsetInterpolationRateDown = 15.f;
	// 跳跃动画转换阶段时允许的误差范围，值越大转换越早
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float JumpTransitionTolerance = 0.7f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float JumpMultiplier = 1.f;
	float *CurrentJumpInterpolationRate = &JumpOffsetInterpolationRateUp;
	UFUNCTION(BlueprintCallable)
	void StartJump();
	UFUNCTION(BlueprintCallable)
	void MidAir();
	UFUNCTION(BlueprintCallable)
	void EndJump();
	void UpdateJumpState();
	void UpdateJump(float DeltaTime);
	// 侧头相关
	FVector CurrentTiltOffset = FVector::ZeroVector;
	UPROPERTY(BlueprintReadonly)
	float CurrentTiltRoll = 0.f;
	// 侧头插值速率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tilt")
	float TiltInterpolationRate = 5.f; 
	// 左侧头偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tilt")
	FVector TiltOffsetLeft = FVector(0.f, -20.f, 0.f); 
	// 右侧头偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tilt")
	FVector TiltOffsetRight = FVector(0.f, 20.f, 0.f); 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tilt")
	float TiltRoll = 15.f;
	// -1为左侧头，1为右侧头，0为无侧头
	UPROPERTY(BlueprintReadonly)
	int TiltDirection = 0; 
	UFUNCTION(BlueprintCallable)
	void SetTilt(int Direction);
	void UpdateTilt(float DeltaTime);
	// ADS相关
	bool ToADS = false;
	UPROPERTY(BlueprintReadonly)
	bool PlayingADSAnimation = false;
	UPROPERTY(BlueprintReadonly)
	bool IsAiming = false;
	FVector CurrentADSCorrection;
	FVector TargetADSCorrection = FVector::ZeroVector;
	FVector CurrentADSOffset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ADS")
	FRotator ADSBaseRotation = FRotator(0.f, 0.f, 0.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ADS")
	UCurveFloat *ADSCurve;
	UFUNCTION(BlueprintCallable)
	void StartADS();
	UFUNCTION(BlueprintCallable)
	void EndADS();
	float CurrentADSTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ADS")
	float ADSTime = 0.2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ADS")
	float TargetADSXOffset = 41;
	float CurrentADSXOffset = TargetADSXOffset;
	UFUNCTION(BlueprintCallable)
	void SetSight(USceneComponent *SightToSet, float Offset, FRotator SightRotation);
	// 准星位置修正
	void ADSCorrection(FVector TotalOffset, FRotator TotalRotationOffset, float DeltaTime);
	FVector TargetSightOffset;
	FVector CurrentSightOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ADS")
	float SightOffsetInterpolationRate = 5.f;
	float ADSAlpha = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ADS")
	float ADSInterpolationRate = 5.f; // ADS插值速率
};