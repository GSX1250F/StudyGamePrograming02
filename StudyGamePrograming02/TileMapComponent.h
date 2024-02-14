#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class TileMapComponent : public SpriteComponent			// Componentの子クラスSpriteComponentの子クラス
{
public:
	// 背景のコンストラクタ。指定されない場合の描画順序の初期値は10とする(早く描画されることで背景となる)
	TileMapComponent(class Actor* owner, int drawOrder = 10);
	/// 更新は親からオーバーライドしない。Drawした後は更新しない。
	//void Update(float deltaTime) override;
	/// 描画は親からオーバーライドする
	void Draw(SDL_Renderer* renderer) override;
	// タイルセットからタイルマップを生成
	void SetTileMap(SDL_Texture* tile_texture);
	// タイルセットをcsvから取得
	bool GetMapLayer(const std::vector<std::string>& filenames, const char delimiter = ',');


private:
	SDL_Texture* mTileTexture;
	std::vector< std::vector<int>> mTileSets;

	int mDrawOrder;

	// タイルマップを格納する2次元vector
	std::vector<std::vector<SDL_Rect>> mMapDatas;

	// タイルセットの画像の幅と高さ
	int mTexWidth;
	int mTexHeight;

	// 一つのタイルの幅と高さ
	const int TILE_HEIGHT = 32;
	const int TILE_WIDTH = 32;

	// 描画するマップの幅と高さ
	const int MAP_HEIGHT = 24;
	const int MAP_WIDTH = 32;

};