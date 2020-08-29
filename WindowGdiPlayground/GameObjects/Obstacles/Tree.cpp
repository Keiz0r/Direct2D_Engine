#include "GameObjects/Obstacles/Tree.h"

Tree::Tree(const D2D1_POINT_2F& position, const float_t& scale, const float_t& opacity, const float_t& rotationAngle, const bool& is_destrucable)
	:
	Obstacle(position, scale, opacity, rotationAngle, is_destrucable)
{
	objectcounter++;
	randomizeTree();
}

Tree::~Tree() {
	objectcounter--;
	if (objectcounter == 0) {
		for (size_t i = 0; i < to_underlying(Tree::Treetype::Count); i++) {
			releaseSprite(s_pSprite[i]);
		}
	}
}

void Tree::draw() {
	if (s_pSprite != nullptr) {
		float_t halfwidth = (spriteSize.width * m_fScalar) / 2.0f;
		float_t halfheight = (spriteSize.height * m_fScalar) / 2.0f;
		static D2D1_POINT_2F screenCenter{ s_pgfx->getScreenSize().width / 2.0f, s_pgfx->getScreenSize().height / 2.0f };
		D2D1_POINT_2F shift = { screenCenter.x - (centerCoordinates->x + centerCoordinates->y - position.x - position.y) * 1.6f , screenCenter.y - (centerCoordinates->x - centerCoordinates->y - position.x + position.y) * 0.8f };
		//no draw if beyond screen
		float_t radiusFromScreenCenter = (position.x - centerCoordinates->x) * (position.x - centerCoordinates->x) + (position.y - centerCoordinates->y) * (position.y - centerCoordinates->y);
		if (visibilityRadius > radiusFromScreenCenter) {
			s_pgfx->transformTRSM(spriteOffset.x * m_fScalar * m_fScalar, spriteOffset.y * m_fScalar * m_fScalar, rotationAngle, { shift.x, shift.y }, m_fScalar, m_fScalar, false);
			s_pgfx->drawBitmap(s_pSprite[to_underlying(type)], { shift.x - halfwidth, shift.y - halfheight, shift.x + halfwidth, shift.y + halfheight }, opacity, { 0.0f, 0.0f, static_cast<float>(spriteSize.width), static_cast<float>(spriteSize.height) });
			s_pgfx->restoreDefaultDrawingParameters();
		}
	}
}

void Tree::randomizeTree() {
	//randomize sprites
	static std::random_device seed;
	static std::mt19937 rng(seed());
	static std::uniform_int_distribution<std::mt19937::result_type> distribution(to_underlying(Tree::Treetype::Regular), to_underlying(Tree::Treetype::Count) - 1);

	type = static_cast<Tree::Treetype>(distribution(rng));
//	type = Tree::Treetype::Type12;
	switch (type) {
	case Tree::Treetype::Regular:
		if (s_pSprite[0] == nullptr) {
			loadSprite(GAMESPRITE(SPRITE_TREE1), s_pSprite[0]);
		}
		spriteSize = s_pSprite[0]->GetPixelSize();
		spriteOffset.y = spriteSize.height / -3.0f;
		break;
	case Tree::Treetype::Spruce:
		if (s_pSprite[1] == nullptr) {
			loadSprite(GAMESPRITE(SPRITE_TREE2), s_pSprite[1]);
		}
		spriteSize = s_pSprite[1]->GetPixelSize();
		spriteOffset.y = spriteSize.height / -2.5f;
		break;
	case Tree::Treetype::Palm1:
		if (s_pSprite[2] == nullptr) {
			loadSprite(GAMESPRITE(SPRITE_TREE3), s_pSprite[2]);
		}
		spriteSize = s_pSprite[2]->GetPixelSize();
		spriteOffset.y = spriteSize.height / -2.3f;
		spriteOffset.x = spriteSize.width / 13.0f;
		break;
	case Tree::Treetype::Palm2:
		if (s_pSprite[3] == nullptr) {
			loadSprite(GAMESPRITE(SPRITE_TREE4), s_pSprite[3]);
		}
		spriteSize = s_pSprite[3]->GetPixelSize();
		spriteOffset.y = spriteSize.height / -2.45f;
		spriteOffset.x = spriteSize.width / 19.0f;
		break;
	case Tree::Treetype::Willow1:
		if (s_pSprite[4] == nullptr) {
			loadSprite(GAMESPRITE(SPRITE_TREE5), s_pSprite[4]);
		}
		spriteSize = s_pSprite[4]->GetPixelSize();
		spriteOffset.y = spriteSize.height / -3.4f;
		spriteOffset.x = spriteSize.width / 50.0f;
		break;
	case Tree::Treetype::Willow2:
		if (s_pSprite[5] == nullptr) {
			loadSprite(GAMESPRITE(SPRITE_TREE6), s_pSprite[5]);
		}
		spriteSize = s_pSprite[5]->GetPixelSize();
		spriteOffset.y = spriteSize.height / -2.95f;
		spriteOffset.x = spriteSize.width / -150.0f;
		break;
	case Tree::Treetype::Pine:
		if (s_pSprite[6] == nullptr) {
			loadSprite(GAMESPRITE(SPRITE_TREE7), s_pSprite[6]);
		}
		spriteSize = s_pSprite[6]->GetPixelSize();
		spriteOffset.y = spriteSize.height / -2.3f;
		spriteOffset.x = spriteSize.width / 50.0f;
		break;
	case Tree::Treetype::Aspen:
		if (s_pSprite[7] == nullptr) {
			loadSprite(GAMESPRITE(SPRITE_TREE8), s_pSprite[7]);
		}
		spriteSize = s_pSprite[7]->GetPixelSize();
		spriteOffset.y = spriteSize.height / -2.55f;
		spriteOffset.x = spriteSize.width / 28.0f;
		break;
	case Tree::Treetype::Type9:
		if (s_pSprite[8] == nullptr) {
			loadSprite(GAMESPRITE(SPRITE_TREE9), s_pSprite[8]);
		}
		spriteSize = s_pSprite[8]->GetPixelSize();
		spriteOffset.y = spriteSize.height / -2.7f;
		spriteOffset.x = spriteSize.width / 55.0f;
		break;
	case Tree::Treetype::Dried:
		if (s_pSprite[9] == nullptr) {
			loadSprite(GAMESPRITE(SPRITE_TREE10), s_pSprite[9]);
		}
		spriteSize = s_pSprite[9]->GetPixelSize();
		spriteOffset.y = spriteSize.height / -2.5f;
		spriteOffset.x = spriteSize.width / 45.0f;
		break;
	case Tree::Treetype::Type11:
		if (s_pSprite[10] == nullptr) {
			loadSprite(GAMESPRITE(SPRITE_TREE11), s_pSprite[10]);
		}
		spriteSize = s_pSprite[10]->GetPixelSize();
		spriteOffset.y = spriteSize.height / -4.0f;
		break;
	case Tree::Treetype::Type12:
		if (s_pSprite[11] == nullptr) {
			loadSprite(GAMESPRITE(SPRITE_TREE12), s_pSprite[11]);
		}
		spriteSize = s_pSprite[11]->GetPixelSize();
		spriteOffset.y = spriteSize.height / -3.8f;
		spriteOffset.x = spriteSize.width / 120.0f;
		break;
	case Tree::Treetype::Type13:
		if (s_pSprite[12] == nullptr) {
			loadSprite(GAMESPRITE(SPRITE_TREE13), s_pSprite[12]);
		}
		spriteSize = s_pSprite[12]->GetPixelSize();
		spriteOffset.y = spriteSize.height / -2.35f;
		spriteOffset.x = spriteSize.width / 30.0f;
		break;
	case Tree::Treetype::Type14:
		if (s_pSprite[13] == nullptr) {
			loadSprite(GAMESPRITE(SPRITE_TREE14), s_pSprite[13]);
		}
		spriteSize = s_pSprite[13]->GetPixelSize();
		spriteOffset.y = spriteSize.height / -2.6f;
		spriteOffset.x = spriteSize.width / 40.0f;
		break;
	}
}
