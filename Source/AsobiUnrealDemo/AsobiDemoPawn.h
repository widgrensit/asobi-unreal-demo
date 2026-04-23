#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AsobiDemoPawn.generated.h"

UCLASS()
class ASOBIUNREALDEMO_API AAsobiDemoPawn : public APawn
{
	GENERATED_BODY()

public:
	AAsobiDemoPawn();

	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Asobi Demo")
	void SetPlayerId(const FString& InPlayerId) { PlayerId = InPlayerId; }

	UFUNCTION(BlueprintCallable, Category = "Asobi Demo")
	void SetIsLocal(bool bInIsLocal) { bIsLocal = bInIsLocal; }

	UFUNCTION(BlueprintCallable, Category = "Asobi Demo")
	void SetTargetPosition(const FVector2D& Target);

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComponent;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void SendInputToServer();

	FString PlayerId;
	bool bIsLocal = false;
	FVector2D InputVector = FVector2D::ZeroVector;
	FVector2D TargetPosition = FVector2D::ZeroVector;
	float InputSendTimer = 0.0f;

	static constexpr float InputSendInterval = 1.0f / 30.0f;
	static constexpr float MoveSpeed = 600.0f;
	static constexpr float RemoteLerpSpeed = 12.0f;
};
