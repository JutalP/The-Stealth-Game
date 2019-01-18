#pragma once

#include "CoreMinimal.h"
#include "Character/FGCharacter.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,

	Suspicious,

	SuspiciousByRobot,

	Alerted
};

UCLASS()
class FGFPS_API AEnemy : public AFGCharacter
{
	GENERATED_BODY()

public:
	AEnemy(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = Component)
		class AExtractZone* MyExtractZone;

	virtual void EquipNewWeapon(class UFGWeaponSchematic* NewWeaponSchematic, FName SocketName) override;

	virtual void Tick(float DeltaTime) override;

	class AFGPlayerCharacter* MyPlayer = nullptr;

	APawn* MyEnemy;

	void StartFire();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Components)
		class UPawnSensingComponent* PawnSensingComp;

	UFUNCTION(BlueprintCallable)
		void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
		void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	FRotator OriginalRotation;

	UFUNCTION()
		void ResetOrientation();

	FTimerHandle TimerHandle_ResetOrientation;

	EAIState EnemyState;

	void SetEnemyState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = AI)
	void OnStateChanged(EAIState NewState);

	UFUNCTION()
	void Lose(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Chasing")
		float MovementSpeed = 100.f;

	UPROPERTY(EditInstanceOnly, Category = AI)
		bool bPatrol;

	UPROPERTY(EditInstanceOnly, Category = AI, meta = (EditCondition = "bPatrol"))
		class AActor* FirstPatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = AI, meta = (EditCondition = "bPatrol"))
		class AActor* SecondPatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = AI, meta = (EditCondition = "bPatrol"))
		class AActor* ThirdPatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = AI, meta = (EditCondition = "bPatrol"))
		class AActor* FourthPatrolPoint;

	class AActor* CurrentPatrolPoint;

	void MoveToNextPatrolPoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
		class UPawnNoiseEmitterComponent* NoiseEmitterComp;

	class APawn* AINoisePawn;

	class APawn* SeenCharacter;

	UPROPERTY(VisibleAnywhere, Category = Components)
	class USphereComponent* Collision;


};
