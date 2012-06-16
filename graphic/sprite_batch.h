
#pragma once

#include <boost/utility.hpp>

#include <other/refference_count.h>
#include <graphic/forward_declaration.h>

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{
//--------------------------------------------------------------------------------------------------
// スプライトバッチ
//--------------------------------------------------------------------------------------------------
class SpriteBatchSystem
	: public IRefferenceCount<SpriteBatchSystem>
	, boost::noncopyable
{
public:
	static SpriteBatch create(GraphicDevice device, size_t size);
	~SpriteBatchSystem() {}

	void begin();
	void end();
	void setTexture(Texture texture);

	void draw(const Point2f &position);
	void draw(const Point2f &position, const Color &color);
	void draw(const Point2f &position, const Color &color, float scale, float rotation);
	void draw(const Point2f &position, const Color &color, float scale, float rotation, float depth);

	enum ColorBlendMode
	{
		BLEND_NOUSE_ALPHA,	// アルファブレンドを切ります
		BLEND_DEFAULT,		// 通常半透明合成します
		BLEND_MODURATE,		// 乗算合成します
		BLEND_INVERSE,		// 白色のテクスチャを上から重ねることで色を反転します
		BLEND_ADDITION1,	// 加算合成します
		BLEND_ADDITION2,	// 加算合成します @attention 非推奨
		BLEND_SUBTRACT1,	// 減算します @attention 非推奨
		BLEND_SUBTRACT2		// 減算します @attention 非推奨
	};
	void blendMode(ColorBlendMode mode);

private:
	SpriteBatchSystem(GraphicDevice device, size_t max_sprite);
	void setIndices();
	void createEffect();

private:
	struct Param;
	Param *param;
};
}
}