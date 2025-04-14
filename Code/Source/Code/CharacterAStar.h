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

private:
	// �滮����·���㣨�������꣩
	TArray<FVector> PathPoints;

	// ��ǰ���ڸ����·���ڵ�����
	int32 CurrentPathIndex;

	// ����Ԫ��С��ÿһ�����100x100��λ�����ò������ڱ༭��������
	UPROPERTY(EditAnywhere, Category = "Pathfinding")
	float GridCellSize;

	// ����������A* ·�����Һ��������������յ㣨�������꣩���ϰ����ά����
	TArray<FVector2D> FindBFSPath(const FVector2D& Start, const FVector2D& End, const TArray<TArray<bool>>& ObstacleGrid);
	// �� A* �����ṹ����Ϊ�ڲ�����
	//struct FAStarNode
	//{
	//	FVector2D GridPos;
	//	float G; // ����㵽�˽ڵ�Ĵ���
	//	float H; // ����ʽ���ƴ���
	//	float F; // G + H
	//	TSharedPtr<FAStarNode> Parent;

	//	FAStarNode(const FVector2D& InPos)
	//		: GridPos(InPos), G(0.f), H(0.f), F(0.f), Parent(nullptr)
	//	{}
	//};
};
