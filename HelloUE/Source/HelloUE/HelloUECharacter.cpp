// Copyright Epic Games, Inc. All Rights Reserved.

#include "HelloUECharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "HelloUE.h"

//////////////////////////////////////////////////////////////////////////
// AHelloUECharacter

AHelloUECharacter::AHelloUECharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	MyObject = NewObject<UMyObject>();
	MyObject->Value = 100;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AHelloUECharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHelloUECharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHelloUECharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHelloUECharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHelloUECharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AHelloUECharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AHelloUECharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AHelloUECharacter::OnResetVR);
}


void AHelloUECharacter::HelloWorld()
{
	// UE_LOG(LogMy, Fatal, TEXT("致命错误")); // 会直接Crash掉
	UE_LOG(LogHello, Error, TEXT("错误"));
	UE_LOG(LogHello, Warning, TEXT("警告"));
	UE_LOG(LogHello, Display, TEXT("显示"));
	UE_LOG(LogHello, Log, TEXT("Log .."));
	UE_LOG(LogHello, Verbose, TEXT("Verbose Log .."));
	UE_LOG(LogHello, VeryVerbose, TEXT("Very Verbose Log ..."));
}

AMyActor* AHelloUECharacter::SpawnMyActor(TSubclassOf<AMyActor> MyActorClass, const FName& MyName,
                                          const FVector& Location, const FRotator& Rotator)
{
	AActor* Actor = GetWorld()->SpawnActor(MyActorClass, &Location, &Rotator);
	AMyActor* MyActor = Cast<AMyActor>(Actor);
	if (MyActor)
	{
		MyActor->MyName = MyName;
	}

	return MyActor;
}

void AHelloUECharacter::CreateMyObjects()
{
	MyObject1 = NewObject<UMyObject>();
	MyObject1->Value = 1;

	MyObject2 = NewObject<UMyObject>();
	MyObject2->Value = 2;

	// MyObject3 = new UMyObject();
	// MyObject3->Value = 3;

	UE_LOG(LogTemp, Log, TEXT("Create Objects"));

	// c++ class (No U)
	UClass* Class = FindObject<UClass>(ANY_PACKAGE, TEXT("MyDerivedObject2"));
	if (Class)
	{
		UMyObject* Object = NewObject<UMyObject>(GetTransientPackage(), Class);
		if (Object)
		{
			Object->HelloWorld();
		}
	}

	// blueprint class (Name_C)
	TSubclassOf<UMyObject> BPClass = LoadClass<UMyObject>(NULL, TEXT("Blueprint'/Game/HelloUE/BP_MyObject.BP_MyObject_C'"));
	if (BPClass)
	{
		UMyObject* Object = NewObject<UMyObject>(GetTransientPackage(), BPClass);
		if (Object)
		{
			Object->HelloWorld();
		}
	}
}

void AHelloUECharacter::DestroMyObjects()
{
	MyObject1 = nullptr;
	MyObject2 = nullptr;
	MyObject3 = nullptr;

	UE_LOG(LogTemp, Log, TEXT("Destroy Objects"));
}

void AHelloUECharacter::OnResetVR()
{
	// If HelloUE is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in HelloUE.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AHelloUECharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AHelloUECharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AHelloUECharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHelloUECharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHelloUECharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AHelloUECharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
