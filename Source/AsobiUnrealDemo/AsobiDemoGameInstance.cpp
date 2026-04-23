#include "AsobiDemoGameInstance.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UAsobiDemoGameInstance::Init()
{
	Super::Init();

	Client = NewObject<UAsobiClient>(this);
	Client->SetBaseUrl(HttpBaseUrl);

	Auth = NewObject<UAsobiAuth>(this);
	Auth->Init(Client);

	Matchmaker = NewObject<UAsobiMatchmaker>(this);
	Matchmaker->Init(Client);

	WebSocket = NewObject<UAsobiWebSocket>(this);
	WebSocket->OnConnected.AddDynamic(this, &UAsobiDemoGameInstance::HandleWsConnected);
	WebSocket->OnDisconnected.AddDynamic(this, &UAsobiDemoGameInstance::HandleWsDisconnected);
	WebSocket->OnMatchJoined.AddDynamic(this, &UAsobiDemoGameInstance::HandleMatchJoined);
	WebSocket->OnMatchLeft.AddDynamic(this, &UAsobiDemoGameInstance::HandleMatchLeft);
}

void UAsobiDemoGameInstance::Shutdown()
{
	if (WebSocket)
	{
		WebSocket->Disconnect();
	}
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HeartbeatTimer);
	}
	Super::Shutdown();
}

void UAsobiDemoGameInstance::SetStatus(EAsobiDemoStatus NewStatus)
{
	if (Status == NewStatus) return;
	Status = NewStatus;
	OnStatusChanged.Broadcast(Status);
}

void UAsobiDemoGameInstance::StartDemo()
{
	SetStatus(EAsobiDemoStatus::LoggingIn);

	const FString RandomName = FString::Printf(TEXT("demo_%d"), FMath::Rand());
	const FString Password = TEXT("demo_password_123");

	FOnAsobiAuthResponse OnReg;
	OnReg.BindDynamic(this, &UAsobiDemoGameInstance::HandleAuthResult);
	Auth->Register(RandomName, Password, RandomName, OnReg);
}

void UAsobiDemoGameInstance::HandleAuthResult(bool bSuccess, const FAsobiAuthTokens& Tokens)
{
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("Asobi auth failed"));
		SetStatus(EAsobiDemoStatus::Failed);
		return;
	}

	LocalPlayerId = Tokens.PlayerId;
	SetStatus(EAsobiDemoStatus::Connecting);

	const FString FullWsUrl = FString::Printf(TEXT("%s?token=%s"), *WebSocketUrl, *Tokens.AccessToken);
	WebSocket->Connect(FullWsUrl);
}

void UAsobiDemoGameInstance::HandleWsConnected()
{
	WebSocket->Authenticate(Client->GetAuthToken());

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			HeartbeatTimer, this, &UAsobiDemoGameInstance::SendHeartbeat, 15.0f, true);
	}

	SetStatus(EAsobiDemoStatus::Queuing);

	FOnAsobiMatchmakerResponse OnMm;
	OnMm.BindDynamic(this, &UAsobiDemoGameInstance::HandleMatchmakerResult);
	Matchmaker->Add(MatchMode, TArray<FString>{LocalPlayerId}, OnMm);
}

void UAsobiDemoGameInstance::HandleWsDisconnected(const FString& Reason)
{
	UE_LOG(LogTemp, Warning, TEXT("Asobi WS disconnected: %s"), *Reason);
	SetStatus(EAsobiDemoStatus::Idle);
}

void UAsobiDemoGameInstance::SendHeartbeat()
{
	if (WebSocket && WebSocket->IsConnected())
	{
		WebSocket->SendHeartbeat();
	}
}

void UAsobiDemoGameInstance::HandleMatchmakerResult(bool bSuccess, const FAsobiMatchmakerTicket& Ticket)
{
	if (!bSuccess)
	{
		UE_LOG(LogTemp, Error, TEXT("Matchmaker add failed"));
		SetStatus(EAsobiDemoStatus::Failed);
	}
}

void UAsobiDemoGameInstance::HandleMatchJoined(const FString& InfoJson)
{
	SetStatus(EAsobiDemoStatus::InMatch);
	UE_LOG(LogTemp, Log, TEXT("Joined match: %s"), *InfoJson);
}

void UAsobiDemoGameInstance::HandleMatchLeft()
{
	SetStatus(EAsobiDemoStatus::Idle);
}
