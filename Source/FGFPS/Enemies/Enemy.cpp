#include "Enemy.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "RotationMatrix.h"
#include "GameFramework/Actor.h"
#include "Player/FGPlayerCharacter.h"
#include "Interaction/ExtractZone.h"
#include "GameFramework/Controller.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/FGLuaComponent.h"
#include "Weapon/FGWeapon.h"
#include "Weapon/FGWeaponSchematic.h"
#include "Components/SkeletalMeshComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"

AEnemy::AEnemy(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemy::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AEnemy::OnNoiseHeard);

	EnemyState = EAIState::Idle;

	NoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetHiddenInGame(true);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::Lose);
	Collision->AttachTo(GetMesh());
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();
	if (bPatrol)
		MoveToNextPatrolPoint();
}

void AEnemy::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr || SeenPawn->IsA<AEnemy>())
		return;

	SeenCharacter = SeenPawn;

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

		    GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
			GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AEnemy::ResetOrientation, 3.0f);
	   
	MyPlayer = Cast<AFGPlayerCharacter>(SeenPawn);

	if (MyPlayer)
	{
		MakeNoise(100.f, this, GetActorLocation(), 200.f);

		for (int i = 0; i < 10; i++)
		{
			StartFire();
		}
		   
			SetEnemyState(EAIState::Alerted);
	}

	AController* Controller = GetController();
	if (Controller)
		Controller->StopMovement();
}

void AEnemy::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{

	if (EnemyState == EAIState::Alerted)
		return;

	AINoisePawn = NoiseInstigator;

	/*AFGPlayerCharacter* MyPlayer = Cast<AFGPlayerCharacter>(NoiseInstigator);*/

	if (NoiseInstigator->IsA<AFGPlayerCharacter>() && EnemyState == EAIState::Idle)
	{

		DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

		FVector Direction = Location - GetActorLocation();
		Direction.Normalize();

		FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
		NewLookAt.Pitch = 0.0f;
		NewLookAt.Roll = 0.0f;

		SetActorRotation(NewLookAt);

		GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
		GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AEnemy::ResetOrientation, 3.0f);

		MyPlayer = Cast<AFGPlayerCharacter>(NoiseInstigator);

		SetEnemyState(EAIState::Suspicious);

			AController* Controller = GetController();
			if (Controller)
				Controller->StopMovement();
	}

	else if (NoiseInstigator->IsA<AEnemy>() && EnemyState == EAIState::Idle)
	{
		UE_LOG(LogTemp, Error, TEXT("HELLO"));

		DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Yellow, false, 10.0f);

		FVector Direction = Location - GetActorLocation();
		Direction.Normalize();

		FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
		NewLookAt.Pitch = 0.0f;
		NewLookAt.Roll = 0.0f;

		MyEnemy = NoiseInstigator;

		SetActorRotation(NewLookAt);

		GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
		GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AEnemy::ResetOrientation, 3.0f);

		SetEnemyState(EAIState::SuspiciousByRobot);
	}

	NoiseInstigator = nullptr;


}

void AEnemy::ResetOrientation()
{
	SeenCharacter = nullptr;
	AINoisePawn = nullptr;
	MyEnemy = nullptr;
	MyPlayer = nullptr;

	SetActorRotation(OriginalRotation);

	SetEnemyState(EAIState::Idle);

	if (bPatrol)
		MoveToNextPatrolPoint();
}

void AEnemy::SetEnemyState(EAIState NewState)
{
	if (EnemyState == NewState)
		return;

	EnemyState = NewState;

	OnStateChanged(EnemyState);
}

void AEnemy::Lose(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AEnemy>())
	{
		return;
	}

	AFGPlayerCharacter* Player = Cast<AFGPlayerCharacter>(OtherActor);

	if (Player)
	{
	     MyExtractZone->CompleteMission(SeenCharacter, false);
	}
}

void AEnemy::MoveToNextPatrolPoint()
{
	if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == FourthPatrolPoint)
		CurrentPatrolPoint = FirstPatrolPoint;

	else if (CurrentPatrolPoint == FirstPatrolPoint)
		CurrentPatrolPoint = SecondPatrolPoint;

	else if (CurrentPatrolPoint == SecondPatrolPoint)
		CurrentPatrolPoint = ThirdPatrolPoint;

	else if (CurrentPatrolPoint == ThirdPatrolPoint)
		CurrentPatrolPoint = FourthPatrolPoint;

	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);
}

void AEnemy::EquipNewWeapon(class UFGWeaponSchematic* NewWeaponSchematic, FName SocketName)
{
	if (!NewWeaponSchematic || !NewWeaponSchematic->WeaponClass)
	{
		return;
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Instigator = this;
	Params.Owner = this;

	AFGWeapon* NewWeapon = GetWorld()->SpawnActor<AFGWeapon>(NewWeaponSchematic->WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);

	if (NewWeapon)
	{
		CurrentWeapon = NewWeapon;
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), TEXT("Weapon"));
		CurrentWeapon->OnFire.AddDynamic(this, &AFGCharacter::HandleWeaponFire);
		WeaponSchematic = NewWeaponSchematic;
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentPatrolPoint && EnemyState == EAIState::Idle)
	{
		FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		float DistanceToGoal = Delta.Size();

		if (DistanceToGoal < 50)
			MoveToNextPatrolPoint();
	}
	if (EnemyState == EAIState::Alerted)
	{

		if (!MyPlayer)
		{
			return;
		}

		float DistToPlayer = FVector::Distance(GetActorLocation(), MyPlayer->GetActorLocation());

		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), MyPlayer);

		UE_LOG(LogTemp, Error, TEXT("%f"), DistToPlayer);

		if (DistToPlayer > PawnSensingComp->SightRadius)
		{
			/*GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
			GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AEnemy::ResetOrientation, 1.0f);*/

			ResetOrientation();
		}

	}
    else if (EnemyState == EAIState::SuspiciousByRobot)
	{
		float DistToEnemmy = FVector::Distance(GetActorLocation(), MyEnemy->GetActorLocation());

		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), MyEnemy);

		if (DistToEnemmy < 400)
		{
			/*GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
			GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AEnemy::ResetOrientation, 3.0f);

			SetEnemyState(EAIState::Suspicious);*/

			ResetOrientation();
		}
	}
	
}

void AEnemy::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}