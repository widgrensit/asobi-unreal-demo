#include "AsobiDemoPawn.h"
#include "AsobiDemoGameInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"

AAsobiDemoPawn::AAsobiDemoPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded())
	{
		MeshComponent->SetStaticMesh(Cube.Object);
	}
	MeshComponent->SetWorldScale3D(FVector(0.5f));

	// Top-down spring-arm + camera so pressing Play actually shows the pawn.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 1200.0f;
	SpringArm->SetRelativeRotation(FRotator(-70.0f, 0.0f, 0.0f));
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void AAsobiDemoPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsLocal)
	{
		FVector Loc = GetActorLocation();
		Loc.X += InputVector.X * MoveSpeed * DeltaSeconds;
		Loc.Y += InputVector.Y * MoveSpeed * DeltaSeconds;
		SetActorLocation(Loc);

		InputSendTimer += DeltaSeconds;
		if (InputSendTimer >= InputSendInterval)
		{
			InputSendTimer = 0.0f;
			SendInputToServer();
		}
	}
	else
	{
		FVector Cur = GetActorLocation();
		FVector Target(TargetPosition.X, TargetPosition.Y, Cur.Z);
		FVector Next = FMath::VInterpTo(Cur, Target, DeltaSeconds, RemoteLerpSpeed);
		SetActorLocation(Next);
	}
}

void AAsobiDemoPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AAsobiDemoPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AAsobiDemoPawn::MoveRight);
}

void AAsobiDemoPawn::MoveForward(float Value)
{
	InputVector.X = FMath::Clamp(Value, -1.0f, 1.0f);
}

void AAsobiDemoPawn::MoveRight(float Value)
{
	InputVector.Y = FMath::Clamp(Value, -1.0f, 1.0f);
}

void AAsobiDemoPawn::SetTargetPosition(const FVector2D& Target)
{
	TargetPosition = Target;
}

void AAsobiDemoPawn::SendInputToServer()
{
	if (UAsobiDemoGameInstance* GI = Cast<UAsobiDemoGameInstance>(GetGameInstance()))
	{
		if (GI->WebSocket && GI->WebSocket->IsConnected())
		{
			const FString DataJson = FString::Printf(
				TEXT("{\"move_x\":%f,\"move_y\":%f}"), InputVector.X, InputVector.Y);
			GI->WebSocket->SendMatchInput(DataJson);
		}
	}
}
