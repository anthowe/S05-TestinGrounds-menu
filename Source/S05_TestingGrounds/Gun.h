// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"




UCLASS()
class S05_TESTINGGROUNDS_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun(const FObjectInitializer& ObjectInitializer);

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;



	///** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	//	class USkeletalMeshComponent* VR_Gun;

	///** Location on VR gun mesh where projectiles should spawn. */
	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	//	class USceneComponent* VR_MuzzleLocation;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UMotionControllerComponent* L_MotionController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	// APawn interface
	void SetupPlayerInputComponent(UInputComponent* InputComponent);
	// End of APawn interface

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	///** Gun muzzle's offset from the characters location */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	//FVector GunOffset;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ASBallProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;

	/** AnimMontages to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation1P;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation3P;
	
	UPROPERTY()
	class UAnimInstance* AnimInstance1P;

	UPROPERTY()
	UAnimInstance* AnimInstance3P;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UParticleSystem* ImpactEffect;

	/** Fires a projectile. */
	UFUNCTION(BlueprintCallable, Category ="Input")
	void OnFire();

	///** Returns Mesh1P subobject **/
	//FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
};
