#ifndef SpriteAnimation_h__
#define SpriteAnimation_h__

//AS this class stands it can only do multiple animations all seperated on different rows, they also must equal the same amount of frames..
class SpriteAnimation
{
public:
	SpriteAnimation(int cols, int rows, float FPS, float animSpeed = 1.0f, bool isLooping = true);
	~SpriteAnimation();
	void Update(float dt);
	void SetAnim(float anim, bool once);
	void SetAnimSpeed(float s);
	void Pause(bool p);
	float GetX();
	float GetY();

private:
	bool  mPaused;
	float mCurrTexTransX;
	float mCurrTexTransY;
	float mFrameWidth;
	float mCurrentAnimation;
	float mCurrentFrame;
	float mPreviousFrame;
	float mIdleAnim;
	int   mCols;
	int	  mRows;
	float mAnimationSpeed;
	float mCurrentSpeed;
	float mFramesPerSecond;
	bool  mIsLooping;
	bool  mPlayOnce;
};

#endif // SpriteAnimation_h__
