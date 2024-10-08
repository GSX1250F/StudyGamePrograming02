#include "Game.h"
#include "SDL_image.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "Character.h"
#include "BGSpriteComponent.h"
#include "TileMapComponent.h"
#include <thread>
#include <chrono>

Game::Game()
	:mWindow(nullptr)
	, mRenderer(nullptr)
	, mIsRunning(true)
	, mIsUpdatingActors(false)
{

}

bool Game::Initialize()
{
	// SDL初期化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("SDLを初期化できません: %s", SDL_GetError());
		return false;
	}
	// SDLウィンドウを作成
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 2)", 100, 100, 1024, 768, 0);
	if (!mWindow)
	{
		SDL_Log("ウィンドウの作成に失敗しました: %s", SDL_GetError());
		return false;
	}
	// SDLレンダラーを作成
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		SDL_Log("レンダラーの作成に失敗しました: %s", SDL_GetError());
		return false;
	}
	// SDL_imageを初期化
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("SDL_imageを初期化できません: %s", SDL_GetError());
		return false;
	}

	// スプライトや背景などのデータを読み込み
	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	// SDLイベントにキューがある間は入力を受け付けない。
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// キーボード入力の受付
	const Uint8* state = SDL_GetKeyboardState(NULL);

	// ESCが押されたとき→Shutdown
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// 宇宙船の操作
	mShip->ProcessKeyboard(state);
	
	/*
	// 人の操作
	mCharacter->ProcessKeyboard(state);
	*/
}

void Game::UpdateGame()
{
	// フレームレート調整（62.5fps)
	if (SDL_GetTicks() - mTicksCount < 16) {
		int sleep = 16 - (SDL_GetTicks() - mTicksCount);
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));    // sleepミリ秒処理を止める
	}

	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));	// 前のフレームから 16ms 経つまで待つ.※sleepしないのでCPU使用率が上がる。
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;		// デルタタイムの計算
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }			// デルタタイムを最大値で制限する
	mTicksCount = SDL_GetTicks();

	// アクターを更新
	mIsUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mIsUpdatingActors = false;

	// 待ちになっていたアクターをmActorsに移動
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// 死んだアクターを一時配列に追加
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// 死んだアクターを削除
	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	// バックバッファ(ゲームのカレントバッファ)を単色でクリアする
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	// バックバッファを現在の描画色でクリアする
	SDL_RenderClear(mRenderer);

	// 全てのスプライトコンポーネントを描画
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}
	// フロントバッファとバックバッファを交換する
	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	// 宇宙船を作成
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, 384.0f));
	mShip->SetScale(1.5f);

	/*
	// プレイヤーであるキャラクターを作成
	mCharacter = new Character(this);
	mCharacter->SetPosition(Vector2(100.0f, 384.0f));
	mCharacter->SetScale(1.5f);
	*/

	// 背景用アクターを作る
	Actor* bgactors = new Actor(this);
	bgactors->SetPosition(Vector2(512.0f, 384.0f));

	// 一番後ろの背景を作成
	BGSpriteComponent* bg = new BGSpriteComponent(bgactors,10);		//描画順序の初期値を省略も可能。
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/Farback01.png")
		,GetTexture("Assets/Farback02.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-10.0f);
	// 手前の背景を作成
	bg = new BGSpriteComponent(bgactors, 20);		//描画順序は一つ大きい値にする
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-50.0f);
	
	
	
	// タイルマップ生成
	Actor* tmactors = new Actor(this);
	tmactors->SetPosition(Vector2(16.0f, 16.0f));

	TileMapComponent* tm = new TileMapComponent(tmactors, 30);
	SDL_Texture* tiletex = GetTexture("Assets/Tiles.png");
	tm->SetTileMap(tiletex);
	
}

void Game::UnloadData()
{
	// actorsを消去
	// ~ActorではRemoveActorが呼び出されるので、別の種類のループを使う
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// テクスチャがすでにマップに入っているのか？
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// ファイルからロード
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("テクスチャファイルのロードに失敗 %s", fileName.c_str());
			return nullptr;
		}

		// サーフェイスからテクスチャを作成
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		// サーフェイスを消去
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("テクスチャへの変換に失敗 %s", fileName.c_str());
			return nullptr;
		}
		// mTexturesに作成したテクスチャを追加
		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	// アクターの更新中ならmPendingActorsに追加
	if (mIsUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// mPendingActorsのvectorからactorを検索し、該当するイテレータを返す
	// actorが見つからない場合は最後のイテレータを返す
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// 該当のイテレータをmPendingActorsの一番最後に移動し、popする
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// mActorsのvectorからactorを検索し、該当するイテレータを返す
	// actorが見つからない場合は最後のイテレータを返す
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// 該当のイテレータをmPendingActorsの一番最後に移動し、popする
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	// ソート済みの配列で挿入点を見つける
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// イテレータの位置の前に要素を挿入する
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// (We can't swap because it ruins ordering)
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}
