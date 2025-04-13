// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAStar.h"

// Sets default values
ACharacterAStar::ACharacterAStar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    // ���� Skeletal Mesh��ȷ��·��������Ŀ����Դ����ȷ·����
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

}

// Called to bind functionality to input
void ACharacterAStar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

