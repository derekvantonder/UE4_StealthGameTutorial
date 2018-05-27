// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSCharacter.h"
#include "FPSProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h" 
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// SQLLite test
// SQLLite test
// SQLLite test
// SQLLite test
#include "sqlite_modern_cpp.h"
using namespace sqlite;
using namespace std;
// SQLLite test
// SQLLite test
// SQLLite test
// SQLLite test



AFPSCharacter::AFPSCharacter()
{
	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->RelativeLocation = FVector(0, 0, BaseEyeHeight); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1PComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1PComponent->SetupAttachment(CameraComponent);
	Mesh1PComponent->CastShadow = false;
	Mesh1PComponent->RelativeRotation = FRotator(2.0f, -15.0f, 5.0f);
	Mesh1PComponent->RelativeLocation = FVector(0, 0, -160.0f);

	// Create a gun mesh component
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	GunMeshComponent->CastShadow = false;
	GunMeshComponent->SetupAttachment(Mesh1PComponent, "GripPoint");




    // SQLLite test
    // SQLLite test
    // SQLLite test
    // SQLLite test
    
    // creates a database file 'dbfile.db' if it does not exists.
    database db("dbfile.db");

    // executes the query and creates a 'user' table
    db <<
        "create table if not exists user ("
        "   _id integer primary key autoincrement not null,"
        "   age int,"
        "   name text,"
        "   weight real"
        ");";

    // inserts a new user record.
    // binds the fields to '?' .
    // note that only types allowed for bindings are :
    //      int ,long, long long, float, double
    //      string , u16string
    // sqlite3 only supports utf8 and utf16 strings, you should use std::string for utf8 and std::u16string for utf16.
    // note that u"my text" is a utf16 string literal of type char16_t * .
    db << "insert into user (age,name,weight) values (?,?,?);"
        << 20
        << u"bob"
        << 83.25;

    int age = 21;
    float weight = 68.5;
    string name = "jack";
    db << u"insert into user (age,name,weight) values (?,?,?);" // utf16 query string
        << age
        << name
        << weight;

    cout << "The new record got assigned id " << db.last_insert_rowid() << endl;

    // slects from user table on a condition ( age > 18 ) and executes
    // the lambda for each row returned .
    db << "select age,name,weight from user where age > ? ;"
        << 18
        >> [&](int age, string name, double weight) {
        cout << age << ' ' << name << ' ' << weight << endl;
    };

    // selects the count(*) from user table
    // note that you can extract a single culumn single row result only to : int,long,long,float,double,string,u16string
    int count = 0;
    db << "select count(*) from user" >> count;
    cout << "cout : " << count << endl;

    // you can also extract multiple column rows
    db << "select age, name from user where _id=1;" >> tie(age, name);
    cout << "Age = " << age << ", name = " << name << endl;

    // this also works and the returned value will be automatically converted to string
    string str_count;
    db << "select count(*) from user" >> str_count;
    cout << "scount : " << str_count << endl;
    
    // SQLLite test
    // SQLLite test
    // SQLLite test
    // SQLLite test
}


void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}


void AFPSCharacter::Fire()
{
	// try and fire a projectile
	if (ProjectileClass)
	{
		FVector MuzzleLocation = GunMeshComponent->GetSocketLocation("Muzzle");
		FRotator MuzzleRotation = GunMeshComponent->GetSocketRotation("Muzzle");

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}

	// try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1PComponent->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(FireAnimation, "Arms", 0.0f);
		}
	}
}


void AFPSCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}


void AFPSCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}