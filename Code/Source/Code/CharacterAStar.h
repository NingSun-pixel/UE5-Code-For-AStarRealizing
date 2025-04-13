// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterAStar.generated.h"

UCLASS()
class CODE_API ACharacterAStar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterAStar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 新增加的函数：接受2D起点和终点
	void MoveTo2DPoint(const FVector2D& StartPoint, const FVector2D& EndPoint);

protected:
	// 移动控制变量
	bool bIsMoving;
	// 存储目标位置（世界坐标），这里保持 Z 为角色当前 Z
	FVector TargetLocation;
	// 移动速度（单位：世界单位/秒）
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveSpeed;
};
