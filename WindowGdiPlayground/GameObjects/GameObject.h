#pragma once
#include "Backend/Graphics.h"
#include "Log.h"
#include <condition_variable>

class GameObject {
public:
	GameObject(const D2D1_POINT_2F& objectPosition, const float_t& opacity, const float_t& rotationAngle);
	virtual ~GameObject();
	virtual void draw() = 0;
	void setPosition(const D2D1_POINT_2F& pos);
	void setPosition(const float_t& x, const float_t& y);
	D2D1_POINT_2F getPosition() const;
	void setRotationAngle(const float_t& Rangle);
	void setScalar(const float_t& scalar);
	void setOpacity(const float_t& opac);
	void runScript(void(*script)(D2D1_POINT_2F&, const float_t&), const float_t& scale);	//TODO: implment scripts and reference here. script is a function pointer
	static void initialize(Graphics* gfx, D2D1_POINT_2F* screencenterVar, const D2D1_POINT_2F& isometricCoeffs);
protected:
	void loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite);
	void releaseSprite(ID2D1Bitmap*& sprite);
protected:
	inline static Graphics* s_pgfx = nullptr;
	inline static std::condition_variable* scriptCV = nullptr;
	D2D1_POINT_2F position;
	bool facingRight = true;
	float_t m_fScalar = 1.0f;
	float_t spriteWidth = 0.0f;
	float_t spriteHeight = 0.0f;
	float_t rotationAngle = 0.0f;
	float_t opacity = 0.0f;
	inline static float_t visibilityRadius = 450.0f * 450.0f;
	inline static D2D1_POINT_2F isometricCoefficients{ 0.0f, 0.0f };
	inline static D2D1_POINT_2F* centerCoordinates = nullptr;
};