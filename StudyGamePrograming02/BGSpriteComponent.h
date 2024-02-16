#pragma once
#include "SpriteComponent.h"
#include <vector>
#include "Math.h"



class BGSpriteComponent : public SpriteComponent		// Component�̎q�N���XSpriteComponent�̎q�N���X
{
public:
	// �w�i�̃R���X�g���N�^�B�w�肳��Ȃ��ꍇ�̕`�揇���̏����l��10�Ƃ���(�����`�悳��邱�ƂŔw�i�ƂȂ�)
	BGSpriteComponent(class Actor* owner, int drawOrder = 10);
	// �X�V�͐e����I�[�o�[���C�h����B�X�N���[����ݒ肷�邽��
	void Update(float deltaTime) override;
	// �`��͐e����I�[�o�[���C�h����B��ʂ̑傫���ɒu�����߁B
	void Draw(SDL_Renderer* renderer) override;
	// �w�i�p�̃e�N�X�`����ݒ肷��
	void SetBGTextures(const std::vector<SDL_Texture*>& textures);
	// ��ʃT�C�Y�ƃX�N���[�����x�̐ݒ�/�擾
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed() const { return mScrollSpeed; }
private:
	// �X�̔w�i�摜�ƃI�t�Z�b�g���J�v�Z��������\����
	struct BGTexture
	{
		SDL_Texture* mTexture;
		Vector2 mOffset;
	};
	std::vector<BGTexture> mBGTextures;
	Vector2 mScreenSize;
	float mScrollSpeed;
};
