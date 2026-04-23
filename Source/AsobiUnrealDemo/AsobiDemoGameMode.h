#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AsobiDemoGameMode.generated.h"

class AAsobiDemoPawn;

UCLASS()
class ASOBIUNREALDEMO_API AAsobiDemoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAsobiDemoGameMode();

	virtual void StartPlay() override;

private:
	UFUNCTION()
	void HandleMatchState(const FString& StateJson);

	AAsobiDemoPawn* GetOrSpawnPawn(const FString& PlayerId, bool bIsLocal);

	UPROPERTY()
	TMap<FString, AAsobiDemoPawn*> Pawns;
};
