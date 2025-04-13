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
    //    UE_LOG(LogTemp, Display, TEXT("GameMode: 1111111111111111Blueprint Pawn：%s"), *PlayerPawnBPClass.Class->GetName());
    //}
    //else
    //{

   DefaultPawnClass = ACodeCharacter::StaticClass();
   UE_LOG(LogTemp, Warning, TEXT("GameMode: 22222222222222 /Game/ThirdPerson/Blueprints/BP_1 ，use CodeCharacter as Pawn"));      //
    //}
}


void ACodeGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld())
    {
        // 定义生成位置和旋转，根据需求调整
        FTransform SpawnTransform;
        SpawnTransform.SetLocation(FVector(0.f, 0.f, 100.f));
        SpawnTransform.SetRotation(FQuat(FRotator(0.f, 0.f, 0.f)));

        // 生成角色（如果你希望使用 BP_ThirdPersonCharacter，可以设置 BP 作为生成类，
        // 但前提是该蓝图的父类为 ACharacterAStar）
        ACharacterAStar* SpawnedCharacter = GetWorld()->SpawnActor<ACharacterAStar>(ACharacterAStar::StaticClass(), SpawnTransform);

        if (SpawnedCharacter)
        {
            // 禁用输入，确保角色不响应输入
            SpawnedCharacter->DisableInput(nullptr);
        }
    }
}