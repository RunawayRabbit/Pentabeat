#pragma once

struct FRCameraInfo
{
	float FoV;
	float SpringArmLength;

	bool operator==(const FRCameraInfo& Other) const;
	bool operator!=(const FRCameraInfo& Other) const;
	bool IsZero() const;
	bool IsNotZero() const;
};

inline bool FRCameraInfo::operator==(const FRCameraInfo& Other) const
{
	return
		(FoV == Other.FoV) &&
		(SpringArmLength == Other.SpringArmLength);
}

inline bool FRCameraInfo::operator!=(const FRCameraInfo& Other) const
{
	return !operator==(Other);
}


inline bool FRCameraInfo::IsZero() const
{
	return
		FMath::Abs(FoV) < SMALL_NUMBER &&
		FMath::Abs(SpringArmLength) < SMALL_NUMBER;
}

inline bool FRCameraInfo::IsNotZero() const
{
	return !IsZero();
}
