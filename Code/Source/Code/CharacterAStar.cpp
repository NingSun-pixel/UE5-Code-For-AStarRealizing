// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAStar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"  // 添加头文件用于调试绘制


// Sets default values
ACharacterAStar::ACharacterAStar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // 初始化移动变量
    bIsMoving = false;
    MoveSpeed = 300.f; // 可根据需求调整速度
    CurrentPathIndex = 0;
    GridCellSize = 100.f;  // 每格代表 100 x 100 单位
    // 加载 Skeletal Mesh（确保路径是你项目中资源的正确路径）
    MoveTo2DPoint(FVector2D(0, 0), FVector2D(3, 8));


    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
    if (MeshAsset.Succeeded())
    {
        // 获取 Mesh 组件并设置 Skeletal Mesh
        GetMesh()->SetSkeletalMesh(MeshAsset.Object);

        // 设置 Mesh 的相对位置和旋转，使其正确显示在 Capsule 内（数值根据你的项目调整）
        GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
        GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("未能加载 Skeletal Mesh 资源！"));
    }

    // 加载 Animation Blueprint 类（请确保路径正确，并已用“_C”后缀）
    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("AnimBlueprint'/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C'"));
    if (AnimBPClass.Succeeded())
    {
        // 将动画蓝图设置给 Mesh 组件，这样角色就会自动播放相应的动画（例如待机动画）
        GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("未能加载动画蓝图资源！"));
    }
}

//////////////////////////////////////////////////////////////
TArray<FVector2D> ACharacterAStar::FindBFSPath(const FVector2D& Start, const FVector2D& End, const TArray<TArray<bool>>& ObstacleGrid)
{
	// BFS 用到队列与父节点映射（用于回溯路径）
	TQueue<FVector2D> Queue;
	TMap<FVector2D, FVector2D> CameFrom;  // 键为节点，值为该节点的父节点

	// 将起点加入队列，并将父节点设为自身（方便回溯）
	Queue.Enqueue(Start);
	CameFrom.Add(Start, Start);

	// 定义四个方向（右、左、上、下）扩展
	TArray<FVector2D> Directions = { FVector2D(1, 0), FVector2D(-1, 0), FVector2D(0, 1), FVector2D(0, -1) };

	// 开始 BFS 搜索
	while (!Queue.IsEmpty())
	{
		FVector2D Current;
		Queue.Dequeue(Current);

		// 如果到达终点，直接退出循环
		if (Current.Equals(End, 0.01f))
		{
			break;
		}

		// 遍历每个邻居
		for (const FVector2D& Dir : Directions)
		{
			FVector2D Neighbor = Current + Dir;
			int32 X = static_cast<int32>(Neighbor.X);
			int32 Y = static_cast<int32>(Neighbor.Y);

			// 检查是否在障碍数组内
			if (!ObstacleGrid.IsValidIndex(Y) || !ObstacleGrid[Y].IsValidIndex(X))
			{
				continue;
			}
			// 如果该单元是障碍，则跳过
			if (!ObstacleGrid[Y][X])
			{
				continue;
			}
			// 如果该邻居还未访问过，则加入队列
			if (!CameFrom.Contains(Neighbor))
			{
				Queue.Enqueue(Neighbor);
				CameFrom.Add(Neighbor, Current);
			}
		}
	}

	// 回溯构造最终路径
	TArray<FVector2D> FinalPath;
	if (!CameFrom.Contains(End))
	{
		// 如果终点未被访问，即没有路径
		return FinalPath;
	}

	FVector2D Current = End;
	while (!Current.Equals(Start, 0.01f))
	{
		FinalPath.Insert(Current, 0);  // 前插，使路径从起点开始
		Current = CameFrom[Current];
	}
	FinalPath.Insert(Start, 0);

	return FinalPath;
}

// Called when the game starts or when spawned
void ACharacterAStar::BeginPlay()
{
	Super::BeginPlay();

	
}

// Tick：沿着路径逐节点移动
void ACharacterAStar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving && PathPoints.Num() > 0 && CurrentPathIndex < PathPoints.Num())
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NextTarget = PathPoints[CurrentPathIndex];
		// 用恒定速率插值到下一个节点
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, NextTarget, DeltaTime, MoveSpeed);
		SetActorLocation(NewLocation);

		// 若足够接近当前节点则切换到下一个节点
		if (FVector::Dist(NewLocation, NextTarget) < 1.0f)
		{
			CurrentPathIndex++;
		}

		// 更新朝向
		if (CurrentPathIndex < PathPoints.Num())
		{
			FVector Direction = (PathPoints[CurrentPathIndex] - NewLocation).GetSafeNormal();
			if (!Direction.IsNearlyZero())
			{
				SetActorRotation(Direction.Rotation());
			}
		}
		else
		{
			// 路径跟随完毕，结束移动
			bIsMoving = false;
			PathPoints.Empty();
		}
	}
}

// Called to bind functionality to input
void ACharacterAStar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//////////////////////////////////////////////////////////////
// MoveTo2DPoint 实现：接收起点和终点（均为网格坐标），调用 A* 算法规划路径，然后将路径转换为世界坐标后开始移动
void ACharacterAStar::MoveTo2DPoint(const FVector2D& StartPoint, const FVector2D& EndPoint)
{
	// 将起点转换为世界坐标（使用 GridCellSize），设定一个固定 Z 值（例如 100.f）
	FVector NewStart(StartPoint.X * GridCellSize, StartPoint.Y * GridCellSize, 100.f);
	SetActorLocation(NewStart);

	// 构造一个简单的障碍物网格（例如 10×10）
	const int32 NumRows = 15;
	const int32 NumCols = 15;
	TArray<TArray<bool>> ObstacleGrid;
	ObstacleGrid.SetNum(NumRows);
	for (int32 y = 0; y < NumRows; y++)
	{
		ObstacleGrid[y].SetNum(NumCols);
		for (int32 x = 0; x < NumCols; x++)
		{
			// 默认所有格子均为可通行
			ObstacleGrid[y][x] = true;
		}
	}

	for (int32 y = 0; y < NumRows; y++)
	{
		for (int32 x = 0; x < NumCols; x++)
		{
			// 示例：将 4-7 1-5 设为障碍（注意：此处按照网格索引）
			if (y > 3 && y < 7 && x > 0 && x < 5)
			{
				ObstacleGrid[y][x] = false;
			}

			if (x == 0)
			{
				ObstacleGrid[y][x] = false;
			}
		}
	}

	// 可视化整个网格，并区分障碍格子
	for (int32 y = 0; y < NumRows; y++)
	{
		for (int32 x = 0; x < NumCols; x++)
		{
			// 根据网格坐标计算当前单元格中心位置（设定一个固定高度Z，比如100.f）
			FVector CellCenter = FVector(x * GridCellSize + GridCellSize * 0.5f,
				y * GridCellSize + GridCellSize * 0.5f,
				100.f);

			// 先绘制整个网格单元（绿色边框），显示所有单元
			DrawDebugBox(GetWorld(),
				CellCenter,
				FVector(GridCellSize * 0.5f, GridCellSize * 0.5f, 2.f),  // 边框较薄
				FColor::Green,
				true,    // 持久显示
				100.f,   // 显示持续时间
				0,
				1.f);   // 线宽

			// 如果当前单元为障碍，绘制红色覆盖
			if (!ObstacleGrid[y][x])
			{
				DrawDebugBox(GetWorld(),
					CellCenter,
					FVector(GridCellSize * 0.5f, GridCellSize * 0.5f, 5.f),  // 可稍微加厚显示
					FColor::Red,
					true,
					100.f,
					0,
					2.f);  // 线宽更厚
			}
		}
	}

	// 调用 A* 算法，传入网格坐标以及障碍数组
	TArray<FVector2D> PathGrid = FindBFSPath(StartPoint, EndPoint, ObstacleGrid);
	if (PathGrid.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("A* Didn't find Path"));
		return;
	}

	// 将规划出的网格路径转换为世界坐标路径（乘以 GridCellSize）
	PathPoints.Empty();
	for (const FVector2D& GridPos : PathGrid)
	{
		FVector WorldPos(GridPos.X * GridCellSize, GridPos.Y * GridCellSize, NewStart.Z);
		PathPoints.Add(WorldPos);
	}

	CurrentPathIndex = 0;
	bIsMoving = true;

	// 可选：更新角色初始朝向指向下一个路径节点
	if (PathPoints.Num() > 1)
	{
		FVector Direction = (PathPoints[1] - NewStart).GetSafeNormal();
		if (!Direction.IsNearlyZero())
		{
			SetActorRotation(Direction.Rotation());
		}
	}
}


