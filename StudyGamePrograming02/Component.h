#pragma once
class Component
{
public:
	// コンストラクタ
	// updateOrderが小さいコンポーネントほど早く更新される
	Component(class Actor* owner, int updateOrder = 100);
	// デストラクタ
	virtual ~Component();
	// このコンポーネントをデルタタイムで更新する
	virtual void Update(float deltaTime);

	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	// 所有アクターへのポインタ
	// コンポーネントから、アクターの位置など必要な情報にアクセスできる。
	class Actor* mOwner;
	// コンポーネントの更新順序
	int mUpdateOrder;
};