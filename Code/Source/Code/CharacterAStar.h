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

private:
	// 规划出的路径点（世界坐标）
	TArray<FVector> PathPoints;

	// 当前正在跟随的路径节点索引
	int32 CurrentPathIndex;

	// 网格单元大小（每一格代表100x100单位），该参数可在编辑器中设置
	UPROPERTY(EditAnywhere, Category = "Pathfinding")
	float GridCellSize;

	// 辅助函数：A* 路径查找函数，输入起点和终点（网格坐标）、障碍物二维数组
	TArray<FVector2D> FindBFSPath(const FVector2D& Start, const FVector2D& End, const TArray<TArray<bool>>& ObstacleGrid);
	// 将 A* 辅助结构体设为内部类型
	//struct FAStarNode
	//{
	//	FVector2D GridPos;
	//	float G; // 从起点到此节点的代价
	//	float H; // 启发式估计代价
	//	float F; // G + H
	//	TSharedPtr<FAStarNode> Parent;

	//	FAStarNode(const FVector2D& InPos)
	//		: GridPos(InPos), G(0.f), H(0.f), F(0.f), Parent(nullptr)
	//	{}
	//};
};
