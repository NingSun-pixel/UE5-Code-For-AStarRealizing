// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAStar.h"

// Sets default values
ACharacterAStar::ACharacterAStar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    // 加载 Skeletal Mesh（确保路径是你项目中资源的正确路径）
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

}

// Called to bind functionality to input
void ACharacterAStar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

