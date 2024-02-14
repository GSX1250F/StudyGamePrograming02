#include "Character.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

Character::Character(Game* game) : Actor(game)		//Actorの子クラス
{
	mRightSpeed = 0.0f;
	mDownSpeed = 0.0f;
	AnimSpriteComponent* asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> anims = {
		game->GetTexture("Assets/Character01.png"),
		game->GetTexture("Assets/Character02.png"),
		game->GetTexture("Assets/Character03.png"),
		game->GetTexture("Assets/Character04.png"),
		game->GetTexture("Assets/Character05.png"),
		game->GetTexture("Assets/Character06.png"),
		game->GetTexture("Assets/Character07.png"),
		game->GetTexture("Assets/Character08.png"),
		game->GetTexture("Assets/Character09.png"),
		game->GetTexture("Assets/Character10.png"),
		game->GetTexture("Assets/Character11.png"),
		game->GetTexture("Assets/Character12.png"),
		game->GetTexture("Assets/Character13.png"),
		game->GetTexture("Assets/Character14.png"),
		game->GetTexture("Assets/Character15.png"),
		game->GetTexture("Assets/Character16.png"),
		game->GetTexture("Assets/Character17.png"),
		game->GetTexture("Assets/Character18.png"),
	};
	asc->SetAnimTextures(anims, 1, 6,true);
	mAnimComponent = asc;
}
void Character::UpdateActor(float deltaTime) 
{
	Actor::UpdateActor(deltaTime);
	// 速度とデルタタイムに基づいて位置を更新する
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	pos.y += mDownSpeed * deltaTime;
	// 位置を画面の左半分に制限する
	if (pos.x < 70.0f)
	{
		pos.x = 70.0f;
	}
	else if (pos.x > 1040.0f)
	{
		pos.x = 1040.0f;
	}
	if (pos.y < 40.0f)
	{
		pos.y = 40.0f;
	}
	else if (pos.y > 670.0f)
	{
		pos.y = 670.0f;
	}
	SetPosition(pos);
}

void Character::ProcessKeyboard(const uint8_t* state)
{
	// 入力に基づいて速度を変更
	// 一旦スピードを 0 にリセットすることで、キーを押し続けていても加速しない。
	mRightSpeed = 0.0f;
	mDownSpeed = 0.0f;

	// right/left
	if (state[SDL_SCANCODE_RIGHT])
	{
		mRightSpeed += 250.0f;
		if(mAnimComponent->mIsAnimating == false){ mAnimComponent->SetAnimNum(1, 6, true); }
	}
	else if (state[SDL_SCANCODE_LEFT])
	{
		mRightSpeed -= 250.0f;
		if (mAnimComponent->mIsAnimating == false) { mAnimComponent->SetAnimNum(1, 6, true); }
	}
	// up/down
	else if (state[SDL_SCANCODE_DOWN])
	{
		mDownSpeed += 300.0f;
		if (mAnimComponent->mIsAnimating == false) { mAnimComponent->SetAnimNum(1, 6, true); }
	}
	else if (state[SDL_SCANCODE_UP])
	{
		mDownSpeed -= 300.0f;
		if (mAnimComponent->mIsAnimating == false) { mAnimComponent->SetAnimNum(1, 6, true); }
	}
	// punch
	else if (state[SDL_SCANCODE_SPACE])
	{
		if (mAnimComponent->mLoopFlag == true){ mAnimComponent->SetAnimNum(16, 18, false); }
	}
	

}