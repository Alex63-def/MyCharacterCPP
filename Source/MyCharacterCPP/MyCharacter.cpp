// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** ????????????? ?????? ??????? ?????????? */
	GetCapsuleComponent()->InitCapsuleSize(45.0f, 105.0f);

	/** ????????? ??????????? ???????? ????????? ?? ???????? ?????? */
	bUseControllerRotationPitch = false;	// ?? ???????????
	bUseControllerRotationYaw = false;		// ??-????????? TRUE, ???????????? ????????? ?????? ? ??????? 
	bUseControllerRotationRoll = false;		// ?? ???????????

	/** ??????? ? ??????????? ????????? "SpringArm" - ???? ??? ?????? */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	/** ??????? ? ??????????? ?????? ????????? */
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	/** ??????????? MovementComponent */
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);	// ??????????? ???????? ?????????
	GetCharacterMovement()->JumpZVelocity = 700.0f;							// ??????????? ???? ?????? - ??????
	GetCharacterMovement()->AirControl = 0.2f;								// ??????????? ???????? ????????? ? ?????? 
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;							// ??????????? ???????? ??????
	GetCharacterMovement()->SetWalkableFloorAngle(60.0f);					// ??????????? ???????????? ???? ??????? ????????? ?? ???????????

	/** ?????????????? ?????????? ?????? ????????? */
	bDead = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** ?????? ??????? ??????? */
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	/** ?????? ?????? ? ????????? ??????*/
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::StopJumping);

	/** ?????? ???????? ??????-?????, ?????-?????? */
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
}

void AMyCharacter::MoveForward(float Axis)
{
	if ((Controller != NULL) && (Axis != 0.0f) && (bDead != true))
	{
		/** ???????? ???? ??????????? ?????? ?? ??? Yaw */
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		/** ???????????? ?????? (???????????) ???????? ? ?????? ???????? */
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}

void AMyCharacter::MoveRight(float Axis)
{
	if ((Controller != NULL) && (Axis != 0.0f) && (bDead != true))
	{
		/** ???????? ???? ??????????? ?????? ?? ??? Yaw */
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		/** ???????????? ?????? (???????????) ???????? ? ?????? ???????? */
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
}

void AMyCharacter::Jump()
{
	if ((Controller != NULL) && (bDead != true))
	{
		ACharacter::Jump();
	}
}

void AMyCharacter::StopJumping()
{
	if ((Controller != NULL) && (bDead != true))
	{
		ACharacter::StopJumping();
	}
}
