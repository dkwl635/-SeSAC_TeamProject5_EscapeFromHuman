// Fill out your copyright notice in the Description page of Project Settings.

#include "LJW/AnimalDataAsset.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

void UAnimalDataAsset::LoadSkillCool()
{
    FString AnimalName = AnimalStats.AnimalName.ToString ();
	FString Path = FPaths::ProjectDir() + TEXT("SKillData/SkillCooldowns_") + AnimalName + TEXT(".json");

    // 파일 없으면 기본 파일 생성
    if (!FPaths::FileExists(Path))
    {
        UE_LOG(LogTemp, Warning, TEXT("JSON file not found. Creating default file: %s"), *Path);

        // 예시 데이터 만들기
        TSharedPtr<FJsonObject> RootObject = MakeShared<FJsonObject>();

        {
            TSharedPtr<FJsonObject> HeroA = MakeShared<FJsonObject>();
            HeroA->SetNumberField(TEXT("Skill1"), 3.5f);
            RootObject->SetObjectField(TEXT("HeroA"), HeroA);
        }

        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);

        // 폴더 경로가 없을 경우 생성
        FString Directory = FPaths::GetPath(Path);
        IFileManager::Get().MakeDirectory(*Directory, true);

        // 파일 저장
        FFileHelper::SaveStringToFile(OutputString, *Path);
        UE_LOG(LogTemp, Log, TEXT("Default SkillCooldowns.json created."));
    }
  
    // 기존 또는 새로 생성한 파일 읽기
    FString JsonString;
    if (!FFileHelper::LoadFileToString ( JsonString , *Path ))
    {
        UE_LOG ( LogTemp , Error , TEXT ( "Failed to load JSON file: %s" ) , *Path );
        return;
    }
  
    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create ( JsonString );

    if (!FJsonSerializer::Deserialize ( Reader , RootObject ) || !RootObject.IsValid ())
    {
        UE_LOG ( LogTemp , Error , TEXT ( "Failed to parse JSON." ) );
        return;
    }

    for (const auto& Entry : RootObject->Values)
    {
        FString CharacterID = Entry.Key;
        TSharedPtr<FJsonObject> SkillData = Entry.Value->AsObject ();

        if (SkillData.IsValid ())
        {
            FSkillCooldown Cooldown;
            Cooldown.SkillCool = SkillData->GetNumberField ( "Skill1" );
         
            AnimalStats.AnimalSkillCoolTime = Cooldown.SkillCool;
        }
    }


}

