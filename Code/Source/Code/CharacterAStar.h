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

	// �����ӵĺ���������2D�����յ�
	void MoveTo2DPoint(const FVector2D& StartPoint, const FVector2D& EndPoint);

protected:
	// �ƶ����Ʊ���
	bool bIsMoving;
	// �洢Ŀ��λ�ã��������꣩�����ﱣ�� Z Ϊ��ɫ��ǰ Z
	FVector TargetLocation;
	// �ƶ��ٶȣ���λ�����絥λ/�룩
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveSpeed;
};
