// Yeet <3

#include "RBulletPulser.h"

URBulletPulser::URBulletPulser()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void URBulletPulser::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float t = 0.5f + FMath::Cos(BeatProgress * PI / PulseTime) * 0.5f;

	for( auto i = Bullets.Num()-1; i >= 0; --i)
	{
		if (Bullets[i] && IsValid(Bullets[i]))
		{
			Bullets[i]->SetScaleFactor(t);
		}
		else
		{
			Bullets.RemoveAtSwap(i);
		}
	}
	/*Bullets.RemoveAll(
		[t](const ARBullet* Bullet)
		{
			if (IsValid(Bullet))
			{
				Bullet->SetScaleFactor(t);
				return false;
			}
			return true;
		});*/
}

void URBulletPulser::RegisterMe(ARBullet* Bullet)
{
	Bullets.Add(Bullet);
}
