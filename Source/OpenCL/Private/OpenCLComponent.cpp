#include "OpenCLComponent.h"
#include "EngineMinimal.h"
#include "Async.h"
#include "IOpenCLPlugin.h"

UOpenCLComponent::UOpenCLComponent(const FObjectInitializer &init) : UActorComponent(init)
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	bWatchKernelsFolderOnStartup = true;

	DeviceGroup = EnumerateDevices();
}

bool UOpenCLComponent::IsWatchingFolders()
{
	return WatchedFolders.Num() > 0;
}

bool UOpenCLComponent::HasValidHardware()
{
	TArray<FOpenCLDeviceData> Devices = EnumerateDevices();
	IOpenCLPlugin::Get().EnumerateDevices(Devices);
	return Devices.Num() > 0;
}

TArray<FOpenCLDeviceData> UOpenCLComponent::EnumerateDevices()
{
	TArray<FOpenCLDeviceData> Devices;
	if (IOpenCLPlugin::Get().IsAvailable())
	{
		IOpenCLPlugin::Get().EnumerateDevices(Devices);
	}
	return Devices;
}

FString UOpenCLComponent::ReadKernelFromFile(const FString& FilePath, bool bIsContentRelative /*= true*/)
{
	FString AbsolutePath;
	if (bIsContentRelative)
	{
		AbsolutePath = FPaths::ProjectContentDir() + FilePath;
	}
	else
	{
		AbsolutePath = FilePath;
	}

	FString ResultString;
	FFileHelper::LoadFileToString(ResultString, *AbsolutePath);
	return ResultString;
}

void UOpenCLComponent::RunOpenCLKernel(const FString& Kernel, const FString& KernelName /*= TEXT("main")*/, const FString& InputArgs /*= TEXT("")*/)
{
	if (IOpenCLPlugin::Get().IsAvailable())
	{
		IOpenCLPlugin::Get().RunKernelOnDevices(Kernel, KernelName, InputArgs, [this](const FString& Result)
		{
			OnResult.Broadcast(Result);
		}, DeviceGroup);
	}
}

void UOpenCLComponent::WatchKernelFolder(const FString& ProjectRelativeFolder)
{
	UnwatchKernelFolder(ProjectRelativeFolder);
	FString AbsolutePath = FPaths::ProjectContentDir() + ProjectRelativeFolder;

	FDirectoryWatcherModule& DirectoryWatcherModule = FModuleManager::Get().LoadModuleChecked<FDirectoryWatcherModule>(TEXT("DirectoryWatcher"));
	IDirectoryWatcher* DirectoryWatcher = DirectoryWatcherModule.Get();

	Changed = IDirectoryWatcher::FDirectoryChanged::CreateLambda([&](const TArray<FFileChangeData>& FileChanges) {

		FTimespan Difference = FDateTime::Now() - LastWatchEventCall;

		//Rate limit file change callbacks
		if (Difference.GetTotalSeconds() > 0.5f)
		{
			FFunctionGraphTask::CreateAndDispatchWhenReady([this, FileChanges]() {
				for (auto Change : FileChanges)
				{
					FPaths::NormalizeFilename(Change.Filename);
					OnKernelSourceChanged.Broadcast(Change.Filename, (EKernelFileChangeAction)Change.Action);
				}
				LastWatchEventCall = FDateTime::Now();
			}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
	});

	if (IFileManager::Get().DirectoryExists(*AbsolutePath))
	{
		DirectoryWatcher->RegisterDirectoryChangedCallback_Handle(AbsolutePath, Changed, DelegateHandle, true);
		WatchedFolders.Add(ProjectRelativeFolder);
	}
	else
	{
		UE_LOG(LogOpenCL, Log, TEXT("WatchKernelFolder:: %s folder doesn't exist."), *AbsolutePath);
	}
}

void UOpenCLComponent::UnwatchKernelFolder(const FString& ProjectRelativeFolder)
{
	FString AbsolutePath = FPaths::ProjectContentDir() + ProjectRelativeFolder;

	FDirectoryWatcherModule& DirectoryWatcherModule = FModuleManager::Get().LoadModuleChecked<FDirectoryWatcherModule>(TEXT("DirectoryWatcher"));
	IDirectoryWatcher* DirectoryWatcher = DirectoryWatcherModule.Get();

	if (IFileManager::Get().DirectoryExists(*AbsolutePath))
	{
		DirectoryWatcher->UnregisterDirectoryChangedCallback_Handle(AbsolutePath, DelegateHandle);
		WatchedFolders.Remove(ProjectRelativeFolder);
	}
	else
	{
		UE_LOG(LogOpenCL, Log, TEXT("UnwatchKernelFolder:: %s folder doesn't exist."), *AbsolutePath);
	}
}

void UOpenCLComponent::InitializeComponent()
{
	Super::InitializeComponent();
	DeviceGroup = EnumerateDevices();

	LastWatchEventCall = FDateTime::Now();
	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull);
	if (bWatchKernelsFolderOnStartup && World->IsGameWorld())
	{
		WatchKernelFolder();
	}
	
}

void UOpenCLComponent::UninitializeComponent()
{	
	TArray<FString> AllWatched = WatchedFolders;
	for (auto Folder : AllWatched)
	{
		UnwatchKernelFolder(Folder);
	}
	Super::UninitializeComponent();
}
