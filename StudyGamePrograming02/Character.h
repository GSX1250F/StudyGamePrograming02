#pragma once
#include "Actor.h"

class Character : public Actor		// Actorの子クラス
{
public:
	Character(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ProcessKeyboard(const uint8_t* state);
	float GetRightSpeed() const { return mRightSpeed; }
	float GetDownSpeed() const { return mDownSpeed; }
private:
	class AnimSpriteComponent* mAnimComponent;
	float mRightSpeed;
	float mDownSpeed;
};