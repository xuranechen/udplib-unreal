// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Networking/Public/Networking.h"
#include "UDPSend.generated.h"

UCLASS()
class UDPLIB_API AUDPSend : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUDPSend();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//新建isudp的bool变量，表示用UDP进行通信
	bool IsUDP;

	//新建函数RamaUDPSender_SendString(),用于发送消息
	UFUNCTION(BlueprintCallable, Category = "UDP")
	bool RamaUDPSender_SendString(FString ToSend);
public:
	TSharedPtr<FInternetAddr> RemoteAddr;

	FSocket* SenderSocket;

	// SocketName,IP,Port,IsUdp
	UFUNCTION(BlueprintCallable, Category = "UDP")
	bool StartUDPSender(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, bool UDP);

	//此函数用于在虚幻C++内获取本地IP，转化为String类型
	UFUNCTION(BlueprintPure, Category = "UDP")
	FString GetIP();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UDP")
	bool ShowOnScreenDebugMessages;

	FORCEINLINE void ScreenMsg(const FString& Msg)
	{
		if (!ShowOnScreenDebugMessages) return;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *Msg);
	}
	FORCEINLINE void ScreenMsg(const FString& Msg, const float Value)
	{
		if (!ShowOnScreenDebugMessages) return;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %f"), *Msg, Value));
	}
	FORCEINLINE void ScreenMsg(const FString& Msg, const FString& Msg2)
	{
		if (!ShowOnScreenDebugMessages) return;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s %s"), *Msg, *Msg2));
	}

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};