#include "SpriteAnimation.h"


SpriteAnimation::SpriteAnimation(int Cols, int Rows, float FPS, float animSpeed /*= 1.0f*/, bool isLooping /*= true*/)
{
	mFramesPerSecond	= (1000.0f / FPS) / 1000.0f;
	mAnimationSpeed		= animSpeed;
	mIsLooping			= isLooping;
	mCurrentSpeed		= 0;
	mCurrentFrame		= 0;
	mPreviousFrame		= -1;
	mCols				= Cols;
	mRows				= Rows;
	mCurrentAnimation	= 0.0f;
	mIdleAnim			= 0.0f;
	mPaused				= false;
}

SpriteAnimation::~SpriteAnimation()
{
}

void SpriteAnimation::Update(float dt)
{
	if (!mPaused)
	{
		mCurrentSpeed += mAnimationSpeed * dt;
		if (mCurrentSpeed > mFramesPerSecond)
		{
			mCurrentFrame++;
			mCurrentSpeed = 0.0f;
	
			//check if last frame 
			if (mCurrentFrame == mCols)
			{
				if (mIsLooping)
				{
					mCurrentFrame = 0.0f;
				}
				else
				{
					mCurrentFrame = mCols;
					//Probably set a boolean here flagging that animation is complete.
				}
				if (mPlayOnce)
				{
					mCurrentAnimation = mIdleAnim;
				}
			}
		}
		mCurrTexTransX = 1.0f * mCurrentFrame ;
		mCurrTexTransY = 1.0f * mCurrentAnimation;
	}
}

void SpriteAnimation::SetAnim(float anim, bool once)
{
	mCurrentAnimation = anim;
	mPlayOnce = once;
}

void SpriteAnimation::SetAnimSpeed(float s)
{
	mAnimationSpeed = s;
}

void SpriteAnimation::Pause(bool p)
{
	mPaused = p;
}

float SpriteAnimation::GetX()
{
	return mCurrTexTransX;
}

float SpriteAnimation::GetY()
{
	return mCurrTexTransY;
}