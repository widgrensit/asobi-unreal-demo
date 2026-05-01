#include "AsobiDemoGameMode.h"
#include "AsobiDemoGameInstance.h"
#include "AsobiDemoHUD.h"
#include "AsobiDemoPawn.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

AAsobiDemoGameMode::AAsobiDemoGameMode()
{
	DefaultPawnClass = AAsobiDemoPawn::StaticClass();
	HUDClass = AAsobiDemoHUD::StaticClass();
}

void AAsobiDemoGameMode::StartPlay()
{
	Super::StartPlay();

	if (UAsobiDemoGameInstance* GI = Cast<UAsobiDemoGameInstance>(GetGameInstance()))
	{
		if (GI->WebSocket)
		{
			GI->WebSocket->OnMatchState.AddDynamic(this, &AAsobiDemoGameMode::HandleMatchState);
		}
		GI->StartDemo();
	}
}

AAsobiDemoPawn* AAsobiDemoGameMode::GetOrSpawnPawn(const FString& PlayerId, bool bIsLocal)
{
	if (AAsobiDemoPawn** Existing = Pawns.Find(PlayerId))
	{
		return *Existing;
	}

	FActorSpawnParameters Params;
	AAsobiDemoPawn* Pawn = GetWorld()->SpawnActor<AAsobiDemoPawn>(
		AAsobiDemoPawn::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

	if (Pawn)
	{
		Pawn->SetPlayerId(PlayerId);
		Pawn->SetIsLocal(bIsLocal);
		if (bIsLocal)
		{
			if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
			{
				PC->Possess(Pawn);
			}
		}
		Pawns.Add(PlayerId, Pawn);
	}
	return Pawn;
}

void AAsobiDemoGameMode::HandleMatchState(const FString& StateJson)
{
	UAsobiDemoGameInstance* GI = Cast<UAsobiDemoGameInstance>(GetGameInstance());
	if (!GI) return;

	const FString LocalId = GI->GetLocalPlayerId();

	TSharedPtr<FJsonObject> Root;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(StateJson);
	if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid()) return;

	const TSharedPtr<FJsonObject>* PlayersObj;
	if (!Root->TryGetObjectField(TEXT("players"), PlayersObj) || !PlayersObj->IsValid()) return;

	for (const auto& Kvp : (*PlayersObj)->Values)
	{
		const FString& PlayerId = Kvp.Key;
		const TSharedPtr<FJsonObject>* PlayerObj;
		if (!Kvp.Value->TryGetObject(PlayerObj)) continue;

		double X = 0, Y = 0;
		(*PlayerObj)->TryGetNumberField(TEXT("x"), X);
		(*PlayerObj)->TryGetNumberField(TEXT("y"), Y);

		const bool bIsLocal = (PlayerId == LocalId);
		AAsobiDemoPawn* Pawn = GetOrSpawnPawn(PlayerId, bIsLocal);
		if (Pawn && !bIsLocal)
		{
			Pawn->SetTargetPosition(FVector2D(X, Y));
		}
	}
}
