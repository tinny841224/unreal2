//	not the auto-generated Epic Games text
//	tinny841224 for PixelsAction
//	8/8/2020

#include "PixelsBPF.h"
#include "ImageUtils.h"
#include "Misc/FileHelper.h"
#include "Serialization/StructuredArchive.h"

FString UPixelsBPF::FormatHandle(int format_len, int value)
{
	FString res = FString::FromInt(value);
	while (res.Len() < format_len)
	{
		res = "0" + res;
	}
	return res;
}

FPixelsStruct UPixelsBPF::GetPixels(UTexture2D* MyTexture2D, EPixelErrorType& _result)
{
	if (MyTexture2D == NULL)
	{
		_result = EPixelErrorType::NULL_TEXTURE2D;
		return FPixelsStruct();
	}

	FPixelsStruct _pPixels;

	MyTexture2D->CompressionSettings = _pPixels.CompressionSettings;
	MyTexture2D->SRGB = _pPixels.SRGB;
	MyTexture2D->UpdateResource();

	FColor* FormatedImageData = reinterpret_cast<FColor*>(MyTexture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
	_pPixels.Width = MyTexture2D->GetSizeX();
	_pPixels.Height = MyTexture2D->GetSizeY();

	for (int32 _sy = 0; _sy < _pPixels.Height; _sy++)
	{
		for (int32 _sx = 0; _sx < _pPixels.Width; _sx++)
		{
			FColor _color;
			_color.R = FormatedImageData[_sy * _pPixels.Width + _sx].R;
			_color.G = FormatedImageData[_sy * _pPixels.Width + _sx].G;
			_color.B = FormatedImageData[_sy * _pPixels.Width + _sx].B;
			_color.A = FormatedImageData[_sy * _pPixels.Width + _sx].A;
			_pPixels.Colors.Push(_color);
		}
	}

	MyTexture2D->PlatformData->Mips[0].BulkData.Unlock();
	MyTexture2D->CompressionSettings = _pPixels.CompressionSettings;
	MyTexture2D->SRGB = _pPixels.SRGB;
	MyTexture2D->UpdateResource();

	_result = EPixelErrorType::SUCCEED;
	return _pPixels;
}

FPixelsStruct UPixelsBPF::GetMaskPixels(UTexture2D * MyTexture2D, EPixelErrorType & _result, int32 _mask_x, int32 _mask_y, UTexture2D * MaskTexture)
{
	if (MyTexture2D == NULL)
	{
		_result = EPixelErrorType::NULL_TEXTURE2D;
		return FPixelsStruct();
	}
	if (MaskTexture == NULL)
	{
		_result = EPixelErrorType::NULL_MASKTEXTURE;
		return FPixelsStruct();
	}

	FPixelsStruct _pPixels;

	MyTexture2D->CompressionSettings = _pPixels.CompressionSettings;
	MyTexture2D->SRGB = _pPixels.SRGB;
	MyTexture2D->UpdateResource();
	FColor* SampleImageData = reinterpret_cast<FColor*>(MyTexture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_ONLY));
	int32 SampleSizeX = MyTexture2D->GetSizeX();
	int32 SampleSizeY = MyTexture2D->GetSizeY();

	MaskTexture->CompressionSettings = _pPixels.CompressionSettings;
	MaskTexture->SRGB = _pPixels.SRGB;
	MaskTexture->UpdateResource();
	FColor* MaskImageData = reinterpret_cast<FColor*>(MaskTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_ONLY));
	int32 MaskSizeX = MaskTexture->GetSizeX();
	int32 MaskSizeY = MaskTexture->GetSizeY();

	_pPixels.Width = MaskSizeX;
	_pPixels.Height = MaskSizeY;
	_pPixels.CompressionSettings = MyTexture2D->CompressionSettings;
	_pPixels.SRGB = MyTexture2D->SRGB;

	_pPixels.Colors.Empty();

	for (int32 _y = 0; _y < MaskSizeY; _y++)
	{
		for (int32 _x = 0; _x < MaskSizeX; _x++)
		{
			int32 tmp_x = _x + _mask_x;
			int32 tmp_y = _y + _mask_y;
			if (tmp_x < 0 || tmp_x > SampleSizeX || tmp_y < 0 || tmp_y > SampleSizeY)
			{
				FColor _color;
				_color.R = 0;
				_color.G = 0;
				_color.B = 0;
				_color.A = 0;
				_pPixels.Colors.Push(_color);
				continue;
			}
			FColor m_color = MaskImageData[_y * MaskSizeX + _x];
			FColor _color;
			if (m_color.A <= 0)
			{
				_color.R = 0;
				_color.G = 0;
				_color.B = 0;
				_color.A = 0;
			}
			else
			{
				_color.R = SampleImageData[tmp_y * SampleSizeX + tmp_x].R;
				_color.G = SampleImageData[tmp_y * SampleSizeX + tmp_x].G;
				_color.B = SampleImageData[tmp_y * SampleSizeX + tmp_x].B;
				_color.A = SampleImageData[tmp_y * SampleSizeX + tmp_x].A;
			}
			_pPixels.Colors.Push(_color);
		}
	}
	MyTexture2D->PlatformData->Mips[0].BulkData.Unlock();
	MyTexture2D->CompressionSettings = _pPixels.CompressionSettings;
	MyTexture2D->SRGB = _pPixels.SRGB;
	MyTexture2D->UpdateResource();

	MaskTexture->PlatformData->Mips[0].BulkData.Unlock();
	MaskTexture->CompressionSettings = _pPixels.CompressionSettings;
	MaskTexture->SRGB = _pPixels.SRGB;
	MaskTexture->UpdateResource();

	_result = EPixelErrorType::SUCCEED;
	return _pPixels;
}

FPixelsStruct UPixelsBPF::GetRectPixels(UTexture2D * MyTexture2D, int32 _x, int32 _y, EPixelErrorType& _result, int32 _width, int32 _height)
{
	if (MyTexture2D == NULL)
	{
		_result = EPixelErrorType::NULL_TEXTURE2D;
		return FPixelsStruct();
	}

	FPixelsStruct _pPixels;

	MyTexture2D->CompressionSettings = _pPixels.CompressionSettings;
	MyTexture2D->SRGB = _pPixels.SRGB;
	MyTexture2D->UpdateResource();

	FColor* FormatedImageData = reinterpret_cast<FColor*>(MyTexture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
	int32 size_x = MyTexture2D->GetSizeX();
	int32 size_y = MyTexture2D->GetSizeY();


	_pPixels.Width = _width;
	_pPixels.Height = _height;

	for (int32 _sY = _y; _sY < _y + _height; _sY++)
	{
		for (int32 _sX = _x; _sX < _x + _width; _sX++)
		{
			FColor _color;
			if (_sY < 0 || _sY >= size_y || _sX < 0 || _sX >= size_x)
			{
				_color.R = 0;
				_color.G = 0;
				_color.B = 0;
				_color.A = 0;
			}
			else
			{
				_color.R = FormatedImageData[_sY * size_x + _sX].R;
				_color.G = FormatedImageData[_sY * size_x + _sX].G;
				_color.B = FormatedImageData[_sY * size_x + _sX].B;
				_color.A = FormatedImageData[_sY * size_x + _sX].A;
			}
			_pPixels.Colors.Push(_color);
		}
	}

	MyTexture2D->PlatformData->Mips[0].BulkData.Unlock();
	MyTexture2D->CompressionSettings = _pPixels.CompressionSettings;
	MyTexture2D->SRGB = _pPixels.SRGB;
	MyTexture2D->UpdateResource();

	_result = EPixelErrorType::SUCCEED;
	return _pPixels;
}

EPixelErrorType UPixelsBPF::SetPixels(UTexture2D* MyTexture2D, int32 _x, int32 _y, FPixelsStruct _pPiexls)
{
	if (MyTexture2D == NULL)
	{
		return EPixelErrorType::NULL_TEXTURE2D;
	}
	if (_pPiexls.Colors.Num() <= 0)
	{
		return EPixelErrorType::NULL_PIXELSDATA;
	}

	MyTexture2D->CompressionSettings = _pPiexls.CompressionSettings;
	MyTexture2D->SRGB = _pPiexls.SRGB;
	MyTexture2D->UpdateResource();

	FColor* FormatedImageData = reinterpret_cast<FColor*>(MyTexture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	int32 size_x = MyTexture2D->GetSizeX();
	int32 size_y = MyTexture2D->GetSizeY();

	int32 _idx = 0;
	for (int32 j = 0; j < _pPiexls.Height; j++)
	{
		for (int32 i = 0; i < _pPiexls.Width; i++)
		{
			int32 tmp_y = j + _y;
			int32 tmp_x = i + _x;
			if (tmp_x < 0 || tmp_x >= size_x || tmp_y < 0 || tmp_y >= size_y)
			{
				_idx++;
				continue;
			}
			if (_pPiexls.Colors[_idx].A <= 0)
			{
				_idx++;
				continue;
			}
			else
			{
				FormatedImageData[tmp_y * size_x + tmp_x] = _pPiexls.Colors[_idx];
				_idx++;
			}
		}
	}
	MyTexture2D->PlatformData->Mips[0].BulkData.Unlock();
	MyTexture2D->CompressionSettings = _pPiexls.CompressionSettings;
	MyTexture2D->SRGB = _pPiexls.SRGB;
	MyTexture2D->UpdateResource();

	return EPixelErrorType::SUCCEED;
}

UTexture2D* UPixelsBPF::CreateTexture2D(FPixelsStruct _pPiexls)
{
	UTexture2D* tex = UTexture2D::CreateTransient(_pPiexls.Width, _pPiexls.Height, PF_B8G8R8A8);
	FColor* colors = reinterpret_cast<FColor*>(tex->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	int _idx = 0;
	for (int32 j = 0; j < _pPiexls.Height; j++)
	{
		for (int32 i = 0; i < _pPiexls.Width; i++)
		{
			colors[j * _pPiexls.Width + i] = _pPiexls.Colors[_idx];
			_idx++;
		}
	}
	tex->PlatformData->Mips[0].BulkData.Unlock();
	tex->CompressionSettings = _pPiexls.CompressionSettings;
	tex->SRGB = _pPiexls.SRGB;
	tex->UpdateResource();
	return tex;
}
