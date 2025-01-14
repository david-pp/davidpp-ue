#pragma once

#include "CoreMinimal.h"
#include <string>

#include "TinyRedisTypes.h"

struct redisContext;
struct redisReply;

/**
 * A Redis Client by Hiredis
 */
class TINYREDIS_API FRedisConnection
{
public:
	FRedisConnection();
	~FRedisConnection();

	/* Connect */
	bool ConnectToRedis(const FString& InHost, int32 InPort, const FString& InPassword);
	void DisconnectRedis();

	void Quit();
	bool SelectIndex(int32 InIndex);
	bool ExecCommand(const FString& InCommand);

	/* Key */
	bool ExistsKey(const FString& InKey);
	bool ExpireKey(const FString& InKey, int32 Sec);
	bool PersistKey(const FString& InKey);
	bool RenameKey(const FString& CurrentKey, const FString& NewKey);
	bool DelKey(const FString& InKey);
	bool TypeKey(const FString& InKey, FString& OutType);

	/* Str */
	bool MSet(TMap<FString, FString>& InMemberMap);
	bool MGet(const TArray<FString>& InKeyList, TArray<FString>& OutMemberList);
	bool SetInt(const FString& InKey, int64 InValue, int32 ExpireTime = 0);
	bool GetInt(const FString& InKey, int64& OutValue);
	bool SetStr(const FString& InKey, const FString& InValue, int32 ExpireTime = 0);
	bool GetStr(const FString& InKey, FString& OutValue);
	bool SetBin(const FString& InKey, const TArray<char>& InValue, int32 ExpireTime = 0);
	bool GetBin(const FString& InKey, TArray<char>& OutValue);
	bool SetCompressBin(const FString& InKey, const TArray<char>& InValue, int32 ExpireTime = 0);
	bool GetCompressBin(const FString& InKey, TArray<char>& OutValue);
	bool Append(const FString& InKey, const FString& InValue);

	bool SetNxInt(const FString& InKey, int64 InValue, int32 ExpireTime = 0);
	bool SetNxStr(const FString& InKey, const FString& InValue, int32 ExpireTime = 0);

	/* Set */
	bool SAdd(const FString& InKey, const TArray<FString>& InMemberList);
	bool SCard(const FString& InKey, int32& OutValue);
	bool SRem(const FString& InKey, const TArray<FString>& InMemberList);
	bool SMembers(const FString& InKey, TArray<FString>& OutMemberList);

	/* Hash */
	bool HSet(const FString& InKey, const FString& Field, const FString& InValue);
	bool HGet(const FString& InKey, const FString& Field, FString& OutValue);
	bool HSetBin(const FString& InKey, const FString& Field, const TArray<char>& InValue);
	bool HGetBin(const FString& InKey, const FString& Field, TArray<char>& OutValue);
	bool HSetCompressBin(const FString& InKey, const FString& Field, const TArray<char>& InValue);
	bool HGetCompressBin(const FString& InKey, const FString& Field, TArray<char>& OutValue);
	bool HIncrby(const FString& InKey, const FString& Field, int32 Incre, int64& OutVal);
	bool HMSet(const FString& InKey, const TMap<FString, FString>& InMemberMap);
	bool HDel(const FString& InKey, const TArray<FString>& InFieldList);
	bool HExists(const FString& InKey, const FString& Field);
	bool HMGet(const FString& InKey, const TSet<FString>& InFieldList, TMap<FString, FString>& OutMemberMap);
	bool HGetAll(const FString& InKey, TMap<FString, FString>& OutMemberMap);

	/* List */
	bool LIndex(const FString& InKey, int32 InIndex, FString& OutValue);
	bool LInsertBefore(const FString& InKey, const FString& Pivot, const FString& InValue);
	bool LInsertAfter(const FString& InKey, const FString& Pivot, const FString& InValue);
	bool LLen(const FString& InKey, int32& Len);
	bool LPop(const FString& InKey, FString& OutValue);
	bool LPush(const FString& InKey, const TArray<FString>& InFieldList);
	bool LRange(const FString& InKey, int32 Start, int32 End, TArray<FString>& OutMemberList);
	bool LRem(const FString& InKey, const FString& InValue, int32 Count = 0);
	bool LSet(const FString& InKey, int32 InIndex, const FString& InValue);
	bool LTrim(const FString& InKey, int32 Start, int32 Stop);
	bool RPop(const FString& InKey, FString& OutValue);
	bool RPush(const FString& InKey, const TArray<FString>& InFieldList);

public:
	// Interfaces for Async Usage
	bool ExecCommand(const FString& Command, FRedisReply& Value, FString& Err, ERedisCommandType CommandType = ERedisCommandType::UNKNOWN);
	bool ExecCommandEx(FRedisReply& Value, FString& Err, ERedisCommandType CommandType, const char* CommandFormat, ...);
	bool ExecCommandEx(ERedisCommandType CommandType, FRedisReply& Value, const char* CommandFormat, ...);
	bool ExecPipelineCommandsEx(const TArray<FString>& PipelineCommands, TArray<FRedisReply>& Values, FString& Err);

	// Pipeline support
	bool AppendPipelineCommand(ERedisCommandType CommandType, const char* CommandFormat, ...);
	bool GetPipelineCommandReply(ERedisCommandType CommandType, FRedisReply& Value);

	bool IsConnected() const;
	bool IsServerClosed() const;
	bool GetError(FString& Err, redisReply* Reply = nullptr) const;

	// ping
	bool Ping(FString& Err);

	// Try to reconnect when disconnected
	bool TryReconnectToRedis(const FString& InHost, int32 InPort, const FString& InPassword, float IntervalSeconds = 0.f);

private:
	FString Host;
	uint16 Port = 0;
	FString Password;
	redisContext* RedisContextPtr = nullptr;
	redisReply* RedisReplyPtr = nullptr;

	bool bIsConnected = false;
	FDateTime LastTryConnectTime;
};

typedef TSharedPtr<FRedisConnection> FRedisConnectionPtr;
