// Fill out your copyright notice in the Description page of Project Settings.

#include "Mannequin.h"
#include "Gun.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"

// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT ("CameraComp"));
	CameraComp->SetupAttachment(SpringArm);
	CameraComp->bUsePawnControlRotation = false;

	WeaponSocketName = "WeaponSocket";
	
	
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		if (GunBlueprint == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("No GunBlueprint"));
			return;
		}
		Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);

		Gun->AnimInstance3P = GetMesh()->GetAnimInstance();

		if (IsPlayerControlled())
		{
			Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), WeaponSocketName);

		}
		else
		{
			Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), WeaponSocketName);

		}


		if (InputComponent != nullptr)
		{
			InputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::PullTrigger);
		}

	}
	
	

}

//FRotator AMannequin::GetDeltaRotation(float DeltaTime) const
//{
//	return FRotator((GetAxisDeltaRotation(RotationRate.Pitch, DeltaTime), GetAxisDeltaRotation(RotationRate.Yaw, DeltaTime), GetAxisDeltaRotation(RotationRate.Roll, DeltaTime));
//}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMannequin::UnPossessed()
{
	Super::UnPossessed();
	if (Gun)
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint_0"));
	}
	

}

//void AMannequin::PullTrigger()
//{
//
//	Gun->OnFire();
//}
void AMannequin::PullTrigger_Implementation()
{
	Gun->OnFire();
}
bool AMannequin::PullTrigger_Validate()
{
	return true;
}


void AMannequin::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMannequin, GunBlueprint);
}