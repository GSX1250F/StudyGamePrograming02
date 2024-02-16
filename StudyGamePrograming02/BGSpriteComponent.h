#pragma once
#include "SpriteComponent.h"
#include <vector>
#include "Math.h"



class BGSpriteComponent : public SpriteComponent		// Componentの子クラスSpriteComponentの子クラス
{
public:
	// 背景のコンストラクタ。指定されない場合の描画順序の初期値は10とする(早く描画されることで背景となる)
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);
	// 更新は親からオーバーライドする。スクロールを設定するため
	void Update(float deltaTime) override;
	// 描画は親からオーバーライドする。画面の大きさに置くため。
	void Draw(SDL_Renderer* renderer) override;
	// 背景用のテクスチャを設定する
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);
	// 画面サイズとスクロール速度の設定/取得
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed() const { return mScrollSpeed; }
private:
	// 個々の背景画像とオフセットをカプセル化する構造体
	struct BGTexture
	{
		SDL_Texture* mTexture;
		Vector2 mOffset;
	};
	std::vector<BGTexture> mBGTextures;
	Vector2 mScreenSize;
	float mScrollSpeed;
};
