#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AsobiDemoHUD.generated.h"

UCLASS()
class ASOBIUNREALDEMO_API AAsobiDemoHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
};
