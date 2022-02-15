//----------------------------------------------------------------------------
// Copyright 2021, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_CONTROLLER_H
#define ANIM_CONTROLLER_H

#include "Anim.h"
#include "TimerController.h"
#include "GameObjectAnim.h"

namespace Azul
{
	class Skeleton;
	class AnimController : public PCSNode
	{
	public:
		AnimController(AnimTime delta, Clip* _pClip, Skeleton* pSkel);
		AnimController();
		AnimController(const AnimController&) = delete;
		AnimController& operator = (const AnimController&) = delete;
		virtual ~AnimController();

		void SetClip(Clip* pClip) const;
		void IncreaseSpeed(float speed) const;
		void DecreaseSpeed(float speed) const;
		void Forward() const;
		void Reverse() const;

		void PlayPause();

		void Update() const;
	private:
		
		TimerController* poTimerControl;
		Anim* poAnim;
		Skeleton* pSkeleton;
		bool bPause;

	};
}

#endif

// --- End of File ---