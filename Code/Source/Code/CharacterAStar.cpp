// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAStar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ACharacterAStar::ACharacterAStar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // 初始化移动变量
    bIsMoving = false;
    MoveSpeed = 300.f; // 可根据需求调整速度
    // 加载 Skeletal Mesh（确保路径是你项目中资源的正确路径）
    MoveTo2DPoint(FVector2D(100.f, 100.f), FVector2D(300.f, 900.f));
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

// Called when the game starts or when spawned
void ACharacterAStar::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ACharacterAStar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bIsMoving)
    {
        // 使用恒定速率插值，平滑移动到目标位置
        FVector CurrentLocation = GetActorLocation();
        FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);
        SetActorLocation(NewLocation);

        // 如果足够接近目标，停止移动
        if (FVector::Dist(NewLocation, TargetLocation) < 1.0f)
        {
            bIsMoving = false;
        }
    }

}

// Called to bind functionality to input
void ACharacterAStar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterAStar::MoveTo2DPoint(const FVector2D& StartPoint, const FVector2D& EndPoint)
{
    // 设置起始位置，保持原有 Z 值（或指定一个固定 Z）
    FVector NewStart(StartPoint.X, StartPoint.Y, 100.0f);
    SetActorLocation(NewStart);

    // 计算目标位置，使用起始位置 Z 值
    TargetLocation = FVector(EndPoint.X, EndPoint.Y, NewStart.Z);

    // 开始移动
    bIsMoving = true;

    // 如果你希望角色朝向移动方向，可以更新角色朝向
    FVector Direction = (TargetLocation - NewStart).GetSafeNormal();
    if (!Direction.IsNearlyZero())
    {
        FRotator NewRotation = Direction.Rotation();
        SetActorRotation(NewRotation);
    }
}

