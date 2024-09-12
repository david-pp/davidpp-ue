#include "TinyHttpRestTest.h"


/**
* GET   /device-management/devices     : Get all devices
* POST  /device-management/devices     : Create a new device
* GET   /device-management/devices/{id} : Get the device information identified by "id"
* PUT   /device-management/devices/{id} : Update the device information identified by "id"
* DELETE /device-management/devices/{id} : Delete device by "id"
 */

// @formatter:off
void FTinyHttpRestTest::RegisterRoutes()
{
	FTinyHttpService::RegisterRoutes();
	
	RegisterRoute({
		TEXT("Get all devices"),
		FHttpPath(TEXT("/devices")),
		EHttpServerRequestVerbs::VERB_GET,
		FHttpServiceHandler::CreateRaw(this, &FTinyHttpRestTest::HandleQueryDevices)
	});
		
	RegisterRoute({
		TEXT("Create a new device"),
		FHttpPath(TEXT("/devices")),
		EHttpServerRequestVerbs::VERB_POST,
		FHttpServiceHandler::CreateRaw(this, &FTinyHttpRestTest::HandleCreateDevice)
	});

	// RegisterRoute<FTestDeviceGetRequest,FTestDeviceGetReply> (
	// 	TEXT("Get the device information"),
	// 	FHttpPath(TEXT("/devices/:device")),
	// 	EHttpServerRequestVerbs::VERB_GET, [this](const FTestDeviceGetRequest& Request, FTestDeviceGetReply& Reply, FString& Error) -> int
	// 	{
	// 		return HandleGetDeviceEx(Request, Reply, Error);
	// 	});

	RegisterRoute({
		TEXT("Update the device information"),
		FHttpPath(TEXT("/devices/:device")),
		EHttpServerRequestVerbs::VERB_PUT,
		FHttpServiceHandler::CreateRaw(this, &FTinyHttpRestTest::HandleUpdateDevice)
	});
		
	RegisterRoute({
		TEXT("Destroy Session"),
		FHttpPath(TEXT("/devices/:devices")),
		EHttpServerRequestVerbs::VERB_DELETE,
		FHttpServiceHandler::CreateRaw(this, &FTinyHttpRestTest::HandleDeleteDevice)
	});
}
// @formatter:on

bool FTinyHttpRestTest::HandleQueryDevices(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	FString DeviceType = Request.QueryParams.FindRef(TEXT("type"));

	FTestDeviceCollection DeviceCollection;
	for (TTuple<FGuid, FTestDevice>& It : Devices)
	{
		// filter by ?type=XXX
		if (DeviceType.IsEmpty() || DeviceType == It.Value.DeviceType)
		{
			DeviceCollection.Devices.Add(It.Value);
		}
	}

	OnComplete(FTinyHttp::ServiceOK(DeviceCollection));

	// TArray<uint8> JsonPayload;
	// FTinyHttp::SerializeResponse(DeviceCollection, JsonPayload);
	// auto Response = FHttpServerResponse::Create(JsonPayload, TEXT("application/json"));
	// OnComplete(MoveTemp(Response));
	return true;
}

bool FTinyHttpRestTest::HandleCreateDevice(const FHttpServerRequest& HttpRequest, const FHttpResultCallback& OnComplete)
{
	UE_LOG(LogTinyHttp, Warning, TEXT("HandleCreateDevice : \n%s"), *FTinyHttp::RequestToDebugString(HttpRequest));

	FTestDeviceCreateRequest CreateRequest;
	if (FTinyHttp::DeserializeRequest(HttpRequest, CreateRequest))
	{
		CreateRequest.Device.DeviceId = FGuid::NewGuid();
		Devices.Add(CreateRequest.Device.DeviceId, CreateRequest.Device);

		OnComplete(FHttpServerResponse::Ok());
	}

	return true;
}

bool FTinyHttpRestTest::HandleGetDevice(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	return true;
}

int FTinyHttpRestTest::HandleGetDeviceEx(const FTestDeviceGetRequest& Request, FTestDeviceGetReply& Reply, FString& Error)
{
	FGuid DeviceID;
	FGuid::Parse(Request.DeviceID, DeviceID);

	FTestDevice* Device = Devices.Find(DeviceID);
	if (!Device)
	{
		Error = TEXT("Can't find device");
		return 100;
	}

	Reply.Device = *Device;
	return SERVICE_OK;
}

bool FTinyHttpRestTest::HandleUpdateDevice(const FHttpServerRequest& HttpRequest, const FHttpResultCallback& OnComplete)
{
	FGuid DeviceID;
	FString DeviceIDString = HttpRequest.PathParams.FindRef(TEXT("device"));
	FGuid::Parse(DeviceIDString, DeviceID);

	FTestDeviceUpdateRequest UpdateRequest;
	if (!FTinyHttp::DeserializeRequest(HttpRequest, UpdateRequest))
	{
		OnComplete(FTinyHttp::ServiceError(100, TEXT("Invalid request")));
		return true;
	}

	FTestDevice* Device = Devices.Find(DeviceID);
	if (!Device)
	{
		OnComplete(FTinyHttp::ServiceError(101, TEXT("Invalid device"), UpdateRequest));
		return true;
	}

	Device->DeviceName = UpdateRequest.DeviceName;
	Device->DeviceType = UpdateRequest.DeviceType;
	Device->DeviceUsers = UpdateRequest.DeviceUsers;
	OnComplete(FTinyHttp::ServiceOK());
	return true;
}

bool FTinyHttpRestTest::HandleDeleteDevice(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	return true;
}


// ------------------------- Test -----------------------------------

BEGIN_DEFINE_SPEC(FTinyHttpServiceSpec, "TinyHttp.Rest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
	TSharedPtr<FTinyHttpRestTest> Service;
END_DEFINE_SPEC(FTinyHttpServiceSpec)

void FTinyHttpServiceSpec::Define()
{
	Service = MakeShared<FTinyHttpRestTest>(8090, TEXT("TestService"));
	Service->RegisterRoutes();
	Service->Start();

	Describe("Spec for Http Service", [this]
	{
		BeforeEach([this]
		{
		});

		AfterEach([this]
		{
		});
	});

	// Service->Stop();
}
