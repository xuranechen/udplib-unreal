// Fill out your copyright notice in the Description page of Project Settings.


#include "UDPSend.h"

AUDPSend::AUDPSend()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don’t need it.
	PrimaryActorTick.bCanEverTick = true;


	SenderSocket = NULL;
	ShowOnScreenDebugMessages = true;

}

// Called when the game starts or when spawned
void AUDPSend::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AUDPSend::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AUDPSend::StartUDPSender(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, bool UDP)
{
	RemoteAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool bIsValid;
	RemoteAddr->SetIp(*TheIP, bIsValid);
	RemoteAddr->SetBroadcastAddress();
	RemoteAddr->SetPort(ThePort);
	if (!bIsValid)
	{
		return false;
	}

	SenderSocket = FUdpSocketBuilder(*YourChosenSocketName)
		.AsReusable()//使绑定的地址可以被其他套接字重用
		.WithBroadcast()
		.WithSendBufferSize(2 * 1024 * 1024)
		/*.BoundToEndpoint(Endpoint)*/
		;

	check(SenderSocket->GetSocketType() == SOCKTYPE_Datagram);
	int32 SendSize = 2 * 1024 * 1024;
	SenderSocket->SetSendBufferSize(SendSize, SendSize);
	SenderSocket->SetReceiveBufferSize(SendSize, SendSize);
	if (bIsValid)
	{
		bIsValid = true;
	}
	return bIsValid;
}

bool AUDPSend::RamaUDPSender_SendString(FString ToSend)
{
	if (!SenderSocket)
	{
		//ScreenMsg("No sender socket");
		return false;
	}

	int32 BytesSent = 0;
	FString serialized = ToSend;
	TCHAR* serializedChar = serialized.GetCharArray().GetData();
	int32 size = FCString::Strlen(serializedChar);
	int32 sent = 0;
	SenderSocket->SendTo((uint8*)TCHAR_TO_UTF8(serializedChar), size * 4, BytesSent, *RemoteAddr);//发送给远端地址

	if (BytesSent <= 0)
	{
		const FString Str = "Socket is valid but the receiver received 0 bytes, make sure it is listening properly!";
		UE_LOG(LogTemp, Error, TEXT("%s"), *Str);
		ScreenMsg(Str);
		return false;
	}
	return true;
}

void AUDPSend::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (SenderSocket)
	{
		SenderSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(SenderSocket);
	}
}

FString AUDPSend::GetIP()
{
	bool canBind = false;
	TSharedRef<FInternetAddr> localIP = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
	// 获得本机的IP
	FString ipStr = (localIP->IsValid() ? localIP->ToString(false) : "");

	return ipStr;
}

