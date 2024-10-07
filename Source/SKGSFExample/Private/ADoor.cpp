#include "ADoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

AADoor::AADoor()
{
    PrimaryActorTick.bCanEverTick = true;

    DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
    RootComponent = DoorFrame;

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(DoorFrame);

    TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
    TriggerVolume->SetupAttachment(DoorFrame);

    bIsDoorOpen = false;
    bIsPlayerInRange = false;
    DoorOpenAngle = 90.0f;
}

void AADoor::SetIsCanOpen(bool bb)
{
    isCanOpen = bb;
}

void AADoor::CloseDoor()
{
    bIsDoorOpen = false;
}

void AADoor::BeginPlay()
{
    Super::BeginPlay();
    InitialRotation = DoorMesh->GetComponentRotation();
    TargetRotation = InitialRotation + FRotator(0.0f, DoorOpenAngle, 0.0f);

    TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AADoor::OnOverlapBegin);
    TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AADoor::OnOverlapEnd);

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->InputComponent->BindAction("OpenDoor", IE_Pressed, this, &AADoor::HandleInput);
    }
}

void AADoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsDoorOpen)
    {
        DoorMesh->SetWorldRotation(FMath::Lerp(DoorMesh->GetComponentRotation(), TargetRotation, DeltaTime * 2.0f));
    }
    else
    {
        DoorMesh->SetWorldRotation(FMath::Lerp(DoorMesh->GetComponentRotation(), InitialRotation, DeltaTime * 2.0f));
    }
}

void AADoor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp && isCanOpen)
    {
        bIsPlayerInRange = true;
    }
}

void AADoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && (OtherActor != this) && OtherComp && isCanOpen)
    {
        bIsPlayerInRange = false;
    }
}

void AADoor::HandleInput()
{
    if (bIsPlayerInRange)
    {
        bIsDoorOpen = !bIsDoorOpen;
    }
}
