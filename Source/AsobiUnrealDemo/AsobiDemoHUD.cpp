#include "AsobiDemoHUD.h"
#include "AsobiDemoGameInstance.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"

void AAsobiDemoHUD::DrawHUD()
{
	Super::DrawHUD();

	UAsobiDemoGameInstance* GI = Cast<UAsobiDemoGameInstance>(GetGameInstance());
	if (!GI || !Canvas) return;

	FString StatusText;
	switch (GI->GetStatus())
	{
	case EAsobiDemoStatus::Idle:       StatusText = TEXT("Asobi: Idle"); break;
	case EAsobiDemoStatus::Connecting: StatusText = TEXT("Asobi: Connecting…"); break;
	case EAsobiDemoStatus::LoggingIn:  StatusText = TEXT("Asobi: Registering…"); break;
	case EAsobiDemoStatus::Queuing:    StatusText = TEXT("Asobi: Queuing for arena…"); break;
	case EAsobiDemoStatus::InMatch:    StatusText = FString::Printf(TEXT("Asobi: In match as %s"), *GI->GetLocalPlayerId()); break;
	case EAsobiDemoStatus::Failed:     StatusText = TEXT("Asobi: Failed — check server"); break;
	}

	Canvas->DrawText(GEngine->GetSmallFont(), StatusText, 24.0f, 24.0f);
}
