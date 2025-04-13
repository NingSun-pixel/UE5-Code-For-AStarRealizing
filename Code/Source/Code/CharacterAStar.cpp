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

    // ��ʼ���ƶ�����
    bIsMoving = false;
    MoveSpeed = 300.f; // �ɸ�����������ٶ�
    // ���� Skeletal Mesh��ȷ��·��������Ŀ����Դ����ȷ·����
    MoveTo2DPoint(FVector2D(100.f, 100.f), FVector2D(300.f, 900.f));
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
        // ʹ�ú㶨���ʲ�ֵ��ƽ���ƶ���Ŀ��λ��
        FVector CurrentLocation = GetActorLocation();
        FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);
        SetActorLocation(NewLocation);

        // ����㹻�ӽ�Ŀ�ֹ꣬ͣ�ƶ�
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
    // ������ʼλ�ã�����ԭ�� Z ֵ����ָ��һ���̶� Z��
    FVector NewStart(StartPoint.X, StartPoint.Y, 100.0f);
    SetActorLocation(NewStart);

    // ����Ŀ��λ�ã�ʹ����ʼλ�� Z ֵ
    TargetLocation = FVector(EndPoint.X, EndPoint.Y, NewStart.Z);

    // ��ʼ�ƶ�
    bIsMoving = true;

    // �����ϣ����ɫ�����ƶ����򣬿��Ը��½�ɫ����
    FVector Direction = (TargetLocation - NewStart).GetSafeNormal();
    if (!Direction.IsNearlyZero())
    {
        FRotator NewRotation = Direction.Rotation();
        SetActorRotation(NewRotation);
    }
}

