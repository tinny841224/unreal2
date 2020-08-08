//	not the auto-generated Epic Games text
//	tinny841224
//	8/8/2020
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture.h"
#include "PixelsBPF.generated.h"

UENUM(BlueprintType)
enum class EPixelErrorType :uint8
{
	SUCCEED = 0 UMETA(DisplayName = "Succeed"),
	ERROR_WIDTH = 1 UMETA(DisplayName = "Error Width"),
	ERROR_HEIGHT = 2 UMETA(DisplayName = "Error Height"),
	NULL_TEXTURE2D = 3 UMETA(DisplayName = "Null Texture2D"),
	NULL_PIXELSDATA = 4 UMETA(DisplayName = "Null PixelsData"),
	NULL_MASKTEXTURE = 5 UMETA(DisplayName = "Null MaskTexture"),
};

USTRUCT(BlueprintType)
struct FPixelsStruct
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PixelBPF")
		int32 Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PixelBPF")
		int32 Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PixelBPF")
		TArray<FColor> Colors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PixelBPF")
		TEnumAsByte<enum TextureCompressionSettings> CompressionSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PixelBPF")
		bool SRGB;

	FPixelsStruct()
	{
		Width = 1;
		Height = 1;
		CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		SRGB = false;
	}
};

/**
 * 
 */
UCLASS()
class PIXELSACTION_API UPixelsBPF : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "PixelBPF")
		static FString FormatHandle(int format_len, int value);

	UFUNCTION(BlueprintCallable, Category = "PixelBPF")
		static FPixelsStruct GetPixels(UTexture2D* MyTexture2D, EPixelErrorType& _result);

	UFUNCTION(BlueprintCallable, Category = "PixelBPF")
		static FPixelsStruct GetMaskPixels(UTexture2D* MyTexture2D, EPixelErrorType& _result, int32 _x, int32 _y, UTexture2D* MaskTexture);

	UFUNCTION(BlueprintCallable, Category = "PixelBPF")
		static FPixelsStruct GetRectPixels(UTexture2D* MyTexture2D, int32 _x, int32 _y, EPixelErrorType& _result, int32 _width = 1, int32 _height = 1);

	UFUNCTION(BlueprintCallable, Category = "PixelBPF")
		static EPixelErrorType SetPixels(UTexture2D* MyTexture2D, int32 _x, int32 _y, FPixelsStruct _pPiexls);

	UFUNCTION(BlueprintCallable, Category = "PixelBPF")
		static UTexture2D* CreateTexture2D(FPixelsStruct _pPiexls);
};
