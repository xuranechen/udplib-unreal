// Fill out your copyright notice in the Description page of Project Settings.


#include "UDPRecive.h"

// Sets default values
AUDPRecive::AUDPRecive()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don’t need it.
	PrimaryActorTick.bCanEverTick = true;

	ListenSocket = NULL;
}

// Called when the game starts or when spawned
void AUDPRecive::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AUDPRecive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//结束时出发事件
void AUDPRecive::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//UDPReceiver置空
	delete UDPReceiver;
	UDPReceiver = nullptr;

	//Clear all sockets!
	if (ListenSocket)
	{
		ListenSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
	}
}

//初始化Receiver
void AUDPRecive::StartUDPReceiver(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, bool& success)
{
	//TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	FIPv4Address Addr;
	FIPv4Address::Parse(TheIP, Addr);

	//使用指定的NetID和端口创建并初始化新的IPv4端点。
	// Any: Defines the wild card endpoint, which is 0.0.0.0:0
	FIPv4Endpoint Endpoint(FIPv4Address::Any, ThePort); //所有ip地址本地
	//FUdpSocketBuilder: Implements a fluent builder for UDP sockets.
	ListenSocket = FUdpSocketBuilder(*YourChosenSocketName)
		.AsNonBlocking()//将套接字操作设置为非阻塞。 这个实例（用于方法链）。
		.AsReusable()//使绑定的地址可以被其他套接字重用。 这个实例（用于方法链）。
		.BoundToEndpoint(Endpoint)//设置将端口绑定到本地端点。 这个实例（用于方法链）。
		.WithReceiveBufferSize(2 * 1024 * 1024)//设置接收数据大小
		;
	//BUFFER SIZE
	int32 BufferSize = 2 * 1024 * 1024;
	ListenSocket->SetSendBufferSize(BufferSize, BufferSize);
	ListenSocket->SetReceiveBufferSize(BufferSize, BufferSize);

	if (ListenSocket)
	{
		success = true;
	}
	else {
		ScreenMsg("No socket");
		success = false;
	}

	//return true;
}

void AUDPRecive::DataRecv(FString& str, bool& success)
{
	if (!ListenSocket)
	{
		ScreenMsg("No sender socket");
		success = false;

	}

	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	TArray<uint8> ReceivedData;//定义一个接收器
	uint32 Size;
	//ListenSocket->HasPendingData(Size) 查询套接字以确定队列中是否有挂起的数据，如果套接字有数据，则为true，否则为false Size参数指示单个recv调用的管道上有多少数据
	if (ListenSocket->HasPendingData(Size))
	{
		success = true;
		str = "";
		uint8* Recv = new uint8[Size];
		int32 BytesRead = 0;

		//将数组调整到给定数量的元素。 新元素将被初始化。
		ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));
		ListenSocket->RecvFrom(ReceivedData.GetData(), ReceivedData.Num(), BytesRead, *targetAddr);
		char ansiiData[1024];
		memcpy(ansiiData, ReceivedData.GetData(), BytesRead);//拷贝数据到接收器
		ansiiData[BytesRead] = 0; //判断数据结束
		FString debugData = UTF8_TO_TCHAR(ansiiData); //字符串转换
		str = debugData;
		// memset(ansiiData,0,1024);//清空
	}
	else
	{
		success = false;
	}
}

