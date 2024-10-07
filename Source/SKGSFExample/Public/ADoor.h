

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ADoor.generated.h"

UCLASS()
class SKGSFEXAMPLE_API AADoor : public AActor
{
    GENERATED_BODY()

public:    
    AADoor();

    UFUNCTION(BlueprintCallable)
    void SetIsCanOpen(bool bb);

    UFUNCTION(BlueprintCallable, Category = "Door")
    void CloseDoor();

    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* DoorMesh;

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* DoorFrame;

    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* TriggerVolume;

    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void HandleInput();

private:
    bool bIsDoorOpen;
    bool bIsPlayerInRange;
    FRotator InitialRotation;
    FRotator TargetRotation;
    float DoorOpenAngle;

    UPROPERTY(EditAnywhere)
    bool isCanOpen;
protected:
    virtual void BeginPlay() override;


};
