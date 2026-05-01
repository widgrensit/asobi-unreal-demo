#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AsobiClient.h"
#include "AsobiAuth.h"
#include "AsobiMatchmaker.h"
#include "AsobiWebSocket.h"
#include "AsobiDemoGameInstance.generated.h"

UENUM(BlueprintType)
enum class EAsobiDemoStatus : uint8
{
	Idle,
	Connecting,
	LoggingIn,
	Queuing,
	InMatch,
	Failed
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAsobiDemoStatusChanged, EAsobiDemoStatus, NewStatus);

UCLASS()
class ASOBIUNREALDEMO_API UAsobiDemoGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable, Category = "Asobi Demo")
	void StartDemo();

	UFUNCTION(BlueprintPure, Category = "Asobi Demo")
	EAsobiDemoStatus GetStatus() const { return Status; }

	UFUNCTION(BlueprintPure, Category = "Asobi Demo")
	FString GetLocalPlayerId() const { return LocalPlayerId; }

	UPROPERTY(BlueprintAssignable, Category = "Asobi Demo")
	FOnAsobiDemoStatusChanged OnStatusChanged;

	UPROPERTY(EditDefaultsOnly, Category = "Asobi Demo")
	FString HttpBaseUrl = TEXT("http://localhost:8084");

	UPROPERTY(EditDefaultsOnly, Category = "Asobi Demo")
	FString WebSocketUrl = TEXT("ws://localhost:8084/ws");

	UPROPERTY(EditDefaultsOnly, Category = "Asobi Demo")
	FString MatchMode = TEXT("demo");

	UPROPERTY(BlueprintReadOnly, Category = "Asobi Demo")
	UAsobiClient* Client;

	UPROPERTY(BlueprintReadOnly, Category = "Asobi Demo")
	UAsobiAuth* Auth;

	UPROPERTY(BlueprintReadOnly, Category = "Asobi Demo")
	UAsobiMatchmaker* Matchmaker;

	UPROPERTY(BlueprintReadOnly, Category = "Asobi Demo")
	UAsobiWebSocket* WebSocket;

private:
	void SetStatus(EAsobiDemoStatus NewStatus);

	UFUNCTION()
	void HandleWsConnected();

	UFUNCTION()
	void HandleWsDisconnected(const FString& Reason);

	UFUNCTION()
	void HandleMatchJoined(const FString& InfoJson);

	UFUNCTION()
	void HandleMatchMatched(const FString& InfoJson);

	UFUNCTION()
	void HandleMatchLeft();

	UFUNCTION()
	void HandleAuthResult(bool bSuccess, const FAsobiAuthTokens& Tokens);

	UFUNCTION()
	void HandleMatchmakerResult(bool bSuccess, const FAsobiMatchmakerTicket& Ticket);

	EAsobiDemoStatus Status = EAsobiDemoStatus::Idle;
	FString LocalPlayerId;
	FTimerHandle HeartbeatTimer;

	void SendHeartbeat();
};
