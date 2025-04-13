// Copyright Epic Games, Inc. All Rights Reserved.

#include "CodeGameMode.h"
#include "CodeCharacter.h"
#include "CharacterAStar.h"
#include "UObject/ConstructorHelpers.h"

ACodeGameMode::ACodeGameMode()
{
    // set default pawn class to our Blueprinted character
    //static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_1"));
    //if (PlayerPawnBPClass.Class != nullptr)
    //{
    //    DefaultPawnClass = PlayerPawnBPClass.Class;
    //    UE_LOG(LogTemp, Display, TEXT("GameMode: 1111111111111111Blueprint Pawn��%s"), *PlayerPawnBPClass.Class->GetName());
    //}
    //else
    //{

   DefaultPawnClass = ACodeCharacter::StaticClass();
   UE_LOG(LogTemp, Warning, TEXT("GameMode: 22222222222222 /Game/ThirdPerson/Blueprints/BP_1 ��use CodeCharacter as Pawn"));      //
    //}
}


void ACodeGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld())
    {
        // ��������λ�ú���ת�������������
        FTransform SpawnTransform;
        SpawnTransform.SetLocation(FVector(0.f, 0.f, 100.f));
        SpawnTransform.SetRotation(FQuat(FRotator(0.f, 0.f, 0.f)));

        // ���ɽ�ɫ�������ϣ��ʹ�� BP_ThirdPersonCharacter���������� BP ��Ϊ�����࣬
        // ��ǰ���Ǹ���ͼ�ĸ���Ϊ ACharacterAStar��
        ACharacterAStar* SpawnedCharacter = GetWorld()->SpawnActor<ACharacterAStar>(ACharacterAStar::StaticClass(), SpawnTransform);

        if (SpawnedCharacter)
        {
            // �������룬ȷ����ɫ����Ӧ����
            SpawnedCharacter->DisableInput(nullptr);
        }
    }
}