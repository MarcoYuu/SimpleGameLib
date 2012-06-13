#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include "../other/refference_count.h"

#include "buffer_object.h"
#include "texture.h"
#include "color.h"

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{
//--------------------------------------------------------------------------------------------------
// スプライトバッチ
//--------------------------------------------------------------------------------------------------
class SpriteBatchSystem;
typedef boost::intrusive_ptr<SpriteBatchSystem> SpriteBatch;

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

	enum COLOR_BLEND
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
	void blendMode(COLOR_BLEND mode);

private:
	SpriteBatchSystem(GraphicDevice device, size_t max_sprite);
	void setIndices();
	void createEffect();

private:
	struct SpriteVertex
	{
		float x, y;//,z	// D3DFVF_XYZ
		DWORD color;	// D3DFVF_DIFFUSE
		float scale;	// D3DFVF_TEX1
		float rotation;	// D3DFVF_TEX1
		float depth;	// D3DFVF_TEX1
		float index;	// D3DFVF_TEX1
	};
	typedef std::vector<SpriteVertex> SpriteVertexList;

private:
	static const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	GraphicDevice m_device;
	Texture m_texture;

	VertexBuffer m_vertex_buff;
	IndexBuffer m_index_buff;
	SpriteVertexList m_vertex_list;

	boost::intrusive_ptr<ID3DXEffect> m_effect;
	boost::intrusive_ptr<IDirect3DVertexDeclaration9> m_declaration;

	int m_current_sprite_num;
	int m_max_sprite_num;
};
}
}