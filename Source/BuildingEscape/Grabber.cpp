// Kaede 2020


#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	InputController = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputController)
	{
		UE_LOG(LogTemp, Error, TEXT("fond input controller for: %s"), *GetOwner()->GetName());
	}
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("no physics handle component found attached to: %s"), *GetOwner()->GetName());
	}
	// ...
	InputController->BindAction
	(
		TEXT("Grab"),
		EInputEvent::IE_Pressed,
		this,
		&UGrabber::Grab
	);
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Key pressed!"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);
	// ...

	FVector LineTraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * Reach;
	DrawDebugLine(
		GetWorld(),
		PlayerViewLocation,
		LineTraceEnd,
		FColor::Red,
		false,
		0.f,
		0,
		5.f);
	
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* HitActor = Hit.GetActor();

	if (HitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("collided with: %s"), *HitActor->GetName());
	}
}

