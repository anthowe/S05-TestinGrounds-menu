// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Mannequin.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class S05_TESTINGGROUNDS_API AMannequin : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMannequin();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	UCameraComponent* CameraComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName WeaponSocketName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual FRotator GetDeltaRotation(float DeltaTime) const;

	//UPROPERTY(Category = "Character Movement (Rotation Settings)", EditAnywhere, BlueprintReadWrite)
	//FRotator RotationRate;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void UnPossessed() override;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class AGun> GunBlueprint;
	
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category ="Weapon")
	void PullTrigger();
	void  PullTrigger_Implementation();
	bool PullTrigger_Validate();


private:
	
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	AGun* Gun;
};
