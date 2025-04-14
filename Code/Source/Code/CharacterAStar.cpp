// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAStar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"  // ���ͷ�ļ����ڵ��Ի���


// Sets default values
ACharacterAStar::ACharacterAStar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // ��ʼ���ƶ�����
    bIsMoving = false;
    MoveSpeed = 300.f; // �ɸ�����������ٶ�
    CurrentPathIndex = 0;
    GridCellSize = 100.f;  // ÿ����� 100 x 100 ��λ
    // ���� Skeletal Mesh��ȷ��·��������Ŀ����Դ����ȷ·����
    MoveTo2DPoint(FVector2D(0, 0), FVector2D(3, 8));


    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
    if (MeshAsset.Succeeded())
    {
        // ��ȡ Mesh ��������� Skeletal Mesh
        GetMesh()->SetSkeletalMesh(MeshAsset.Object);

        // ���� Mesh �����λ�ú���ת��ʹ����ȷ��ʾ�� Capsule �ڣ���ֵ���������Ŀ������
        GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
        GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("δ�ܼ��� Skeletal Mesh ��Դ��"));
    }

    // ���� Animation Blueprint �ࣨ��ȷ��·����ȷ�������á�_C����׺��
    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("AnimBlueprint'/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C'"));
    if (AnimBPClass.Succeeded())
    {
        // ��������ͼ���ø� Mesh �����������ɫ�ͻ��Զ�������Ӧ�Ķ������������������
        GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("δ�ܼ��ض�����ͼ��Դ��"));
    }
}

//////////////////////////////////////////////////////////////
TArray<FVector2D> ACharacterAStar::FindBFSPath(const FVector2D& Start, const FVector2D& End, const TArray<TArray<bool>>& ObstacleGrid)
{
	// BFS �õ������븸�ڵ�ӳ�䣨���ڻ���·����
	TQueue<FVector2D> Queue;
	TMap<FVector2D, FVector2D> CameFrom;  // ��Ϊ�ڵ㣬ֵΪ�ýڵ�ĸ��ڵ�

	// ����������У��������ڵ���Ϊ����������ݣ�
	Queue.Enqueue(Start);
	CameFrom.Add(Start, Start);

	// �����ĸ������ҡ����ϡ��£���չ
	TArray<FVector2D> Directions = { FVector2D(1, 0), FVector2D(-1, 0), FVector2D(0, 1), FVector2D(0, -1) };

	// ��ʼ BFS ����
	while (!Queue.IsEmpty())
	{
		FVector2D Current;
		Queue.Dequeue(Current);

		// ��������յ㣬ֱ���˳�ѭ��
		if (Current.Equals(End, 0.01f))
		{
			break;
		}

		// ����ÿ���ھ�
		for (const FVector2D& Dir : Directions)
		{
			FVector2D Neighbor = Current + Dir;
			int32 X = static_cast<int32>(Neighbor.X);
			int32 Y = static_cast<int32>(Neighbor.Y);

			// ����Ƿ����ϰ�������
			if (!ObstacleGrid.IsValidIndex(Y) || !ObstacleGrid[Y].IsValidIndex(X))
			{
				continue;
			}
			// ����õ�Ԫ���ϰ���������
			if (!ObstacleGrid[Y][X])
			{
				continue;
			}
			// ������ھӻ�δ���ʹ�����������
			if (!CameFrom.Contains(Neighbor))
			{
				Queue.Enqueue(Neighbor);
				CameFrom.Add(Neighbor, Current);
			}
		}
	}

	// ���ݹ�������·��
	TArray<FVector2D> FinalPath;
	if (!CameFrom.Contains(End))
	{
		// ����յ�δ�����ʣ���û��·��
		return FinalPath;
	}

	FVector2D Current = End;
	while (!Current.Equals(Start, 0.01f))
	{
		FinalPath.Insert(Current, 0);  // ǰ�壬ʹ·������㿪ʼ
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

// Tick������·����ڵ��ƶ�
void ACharacterAStar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving && PathPoints.Num() > 0 && CurrentPathIndex < PathPoints.Num())
	{
		FVector CurrentLocation = GetActorLocation();
		FVector NextTarget = PathPoints[CurrentPathIndex];
		// �ú㶨���ʲ�ֵ����һ���ڵ�
		FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, NextTarget, DeltaTime, MoveSpeed);
		SetActorLocation(NewLocation);

		// ���㹻�ӽ���ǰ�ڵ����л�����һ���ڵ�
		if (FVector::Dist(NewLocation, NextTarget) < 1.0f)
		{
			CurrentPathIndex++;
		}

		// ���³���
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
			// ·��������ϣ������ƶ�
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
// MoveTo2DPoint ʵ�֣����������յ㣨��Ϊ�������꣩������ A* �㷨�滮·����Ȼ��·��ת��Ϊ���������ʼ�ƶ�
void ACharacterAStar::MoveTo2DPoint(const FVector2D& StartPoint, const FVector2D& EndPoint)
{
	// �����ת��Ϊ�������꣨ʹ�� GridCellSize�����趨һ���̶� Z ֵ������ 100.f��
	FVector NewStart(StartPoint.X * GridCellSize, StartPoint.Y * GridCellSize, 100.f);
	SetActorLocation(NewStart);

	// ����һ���򵥵��ϰ����������� 10��10��
	const int32 NumRows = 15;
	const int32 NumCols = 15;
	TArray<TArray<bool>> ObstacleGrid;
	ObstacleGrid.SetNum(NumRows);
	for (int32 y = 0; y < NumRows; y++)
	{
		ObstacleGrid[y].SetNum(NumCols);
		for (int32 x = 0; x < NumCols; x++)
		{
			// Ĭ�����и��Ӿ�Ϊ��ͨ��
			ObstacleGrid[y][x] = true;
		}
	}

	for (int32 y = 0; y < NumRows; y++)
	{
		for (int32 x = 0; x < NumCols; x++)
		{
			// ʾ������ 4-7 1-5 ��Ϊ�ϰ���ע�⣺�˴���������������
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

	// ���ӻ��������񣬲������ϰ�����
	for (int32 y = 0; y < NumRows; y++)
	{
		for (int32 x = 0; x < NumCols; x++)
		{
			// ��������������㵱ǰ��Ԫ������λ�ã��趨һ���̶��߶�Z������100.f��
			FVector CellCenter = FVector(x * GridCellSize + GridCellSize * 0.5f,
				y * GridCellSize + GridCellSize * 0.5f,
				100.f);

			// �Ȼ�����������Ԫ����ɫ�߿򣩣���ʾ���е�Ԫ
			DrawDebugBox(GetWorld(),
				CellCenter,
				FVector(GridCellSize * 0.5f, GridCellSize * 0.5f, 2.f),  // �߿�ϱ�
				FColor::Green,
				true,    // �־���ʾ
				100.f,   // ��ʾ����ʱ��
				0,
				1.f);   // �߿�

			// �����ǰ��ԪΪ�ϰ������ƺ�ɫ����
			if (!ObstacleGrid[y][x])
			{
				DrawDebugBox(GetWorld(),
					CellCenter,
					FVector(GridCellSize * 0.5f, GridCellSize * 0.5f, 5.f),  // ����΢�Ӻ���ʾ
					FColor::Red,
					true,
					100.f,
					0,
					2.f);  // �߿����
			}
		}
	}

	// ���� A* �㷨���������������Լ��ϰ�����
	TArray<FVector2D> PathGrid = FindBFSPath(StartPoint, EndPoint, ObstacleGrid);
	if (PathGrid.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("A* Didn't find Path"));
		return;
	}

	// ���滮��������·��ת��Ϊ��������·�������� GridCellSize��
	PathPoints.Empty();
	for (const FVector2D& GridPos : PathGrid)
	{
		FVector WorldPos(GridPos.X * GridCellSize, GridPos.Y * GridCellSize, NewStart.Z);
		PathPoints.Add(WorldPos);
	}

	CurrentPathIndex = 0;
	bIsMoving = true;

	// ��ѡ�����½�ɫ��ʼ����ָ����һ��·���ڵ�
	if (PathPoints.Num() > 1)
	{
		FVector Direction = (PathPoints[1] - NewStart).GetSafeNormal();
		if (!Direction.IsNearlyZero())
		{
			SetActorRotation(Direction.Rotation());
		}
	}
}


