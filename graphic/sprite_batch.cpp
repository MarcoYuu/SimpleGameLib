#include "sprite_batch.h"
#include "sprite.h"

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{
SpriteBatchSystem::SpriteBatchSystem(GraphicDevice device, size_t max_sprite)
	: m_device(device)
	, m_texture()
	, m_vertex_buff()
	, m_index_buff()
	, m_vertex_list(max_sprite * 4)
	, m_current_sprite_num(0)
	, m_max_sprite_num(max_sprite)
{
	m_vertex_buff = DynamicVertexBufferObject::create(device, max_sprite * sizeof(SpriteVertex) * 4, FVF);
	m_index_buff  = IndexBufferObject::create(device, IndexBufferObject::SIZE_16BIT, sizeof(short) * 6 * max_sprite);

	setIndices();
	createEffect();
}

SpriteBatch SpriteBatchSystem::create(GraphicDevice device, size_t size)
{
	return SpriteBatch(new SpriteBatchSystem(device, size));
}

void SpriteBatchSystem::begin()
{
	//ライトとカリングオフ
	m_device->device()->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//テクスチャ変換許可
	m_device->device()->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	//ブレンドファクターを用いる
	m_device->device()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_device->device()->SetTextureStageState(0, D3DTSS_COLOROP,	 D3DTOP_MODULATE);
	m_device->device()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	m_device->device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_device->device()->SetTextureStageState(0, D3DTSS_ALPHAOP,	 D3DTOP_MODULATE);
	m_device->device()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
}
void SpriteBatchSystem::end()
{
	// 頂点の書き込み
	size_t offset = m_vertex_buff->write(&m_vertex_list[0], m_current_sprite_num * 4 * sizeof(SpriteVertex));

	// 状態設定
	m_effect->SetTechnique("ParameterVertex");
	m_effect->SetFloatArray("ViewportSize", m_device->getBackBufferSize().toAry(), 2);
	if(m_texture!=NULL && m_texture->get()!=NULL){
		m_effect->SetFloatArray("TextureSize", m_texture->getSize().toAry(), 2);
		m_effect->SetTexture("Texture", m_texture->get());
	}

	// パスの開始
	UINT num_pass;
	m_effect->Begin(&num_pass, 0);
	m_effect->BeginPass(0);
	{
		// 描画する頂点の設定
		m_device->device()->SetStreamSource(0, m_vertex_buff->get(), 0, sizeof(SpriteVertex));
		m_device->device()->SetIndices(m_index_buff->get());
		//m_device->device()->SetTexture(0, m_texture->get());

		// 頂点宣言の設定
		m_device->device()->SetVertexDeclaration(m_declaration.get());

		// 描画
		m_device->device()->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST, 0,
			offset / sizeof(SpriteVertex),		// 最小インデックス、バッファにあらかじめ含まれる頂点の個数
			m_current_sprite_num * 4,				// 最少インデックス以降に含まれる頂点数。とりあえずスプライトの個数から決め打ち
			offset * 6 / (sizeof(SpriteVertex) * 4),	// 描画処理するインデックスまでのオフセット。入っていたスプライト数*6
			m_current_sprite_num * 2				// 描画されるポリゴンの個数
		);
		//m_device->device()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, current_sprite_num*4, 0, current_sprite_num*2);
	}
	// パスの終了
	m_effect->EndPass();
	m_effect->End();

	//レンダーステートをデフォルトに戻す
	m_device->device()->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_device->device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_device->device()->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

	//デフォルトのテクスチャブレンド設定
	m_device->device()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_device->device()->SetTextureStageState(0, D3DTSS_COLOROP,	 D3DTOP_MODULATE);
	m_device->device()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_device->device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_device->device()->SetTextureStageState(0, D3DTSS_ALPHAOP,	 D3DTOP_MODULATE);
	m_device->device()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// 書き終わったのでスプライトの個数を初期化
	this->m_current_sprite_num = 0;
}

void SpriteBatchSystem::setTexture(Texture texture)
{
	m_texture = texture;
}

void SpriteBatchSystem::draw(const Point2f &position)
{
	draw(position, Color::White);
}

void SpriteBatchSystem::draw(const Point2f &position, const Color &color)
{
	draw(position, color, 1.0f, 0.0f);
}

void SpriteBatchSystem::draw(const Point2f &position, const Color &color, float scale, float rotation)
{
	draw(position, color, scale, rotation, 0);
}

void SpriteBatchSystem::draw(const Point2f &position, const Color &color, float scale, float rotation, float depth)
{
	if(m_current_sprite_num < m_max_sprite_num)
	{
		size_t cur_index = m_current_sprite_num * 4;
		// 左上・右上・右下・左下の順に頂点を格納
		for(int i = 0; i < 4; ++i)
		{
			m_vertex_list[cur_index + i].x = position.x;
			m_vertex_list[cur_index + i].y = position.y;
			//m_vertex_list[cur_index + i].z = 0;
			m_vertex_list[cur_index + i].color = color;
			m_vertex_list[cur_index + i].scale = scale;
			m_vertex_list[cur_index + i].rotation = rotation;
			m_vertex_list[cur_index + i].depth = depth;
			m_vertex_list[cur_index + i].index = (float)i;
		}

		++m_current_sprite_num;
	}
}

void SpriteBatchSystem::setIndices()
{
	// 一つの四角形を描画するにはインデックスが6つ必要
	// 頂点の並びは以下の通り
	//   0   1
	//   +---+
	//   |   |
	//   +---+
	//   3   2
	int indexCount = m_max_sprite_num * 6;
	std::vector<short> indices(indexCount);

	short vertexIndex = 0;
	for(int i = 0; i < indexCount; i += 6)
	{
		indices[i + 0] = (short)(vertexIndex + 0);
		indices[i + 1] = (short)(vertexIndex + 1);
		indices[i + 2] = (short)(vertexIndex + 2);

		indices[i + 3] = (short)(vertexIndex + 0);
		indices[i + 4] = (short)(vertexIndex + 2);
		indices[i + 5] = (short)(vertexIndex + 3);

		vertexIndex += 4;
	}
	m_index_buff->write(&indices[0], m_index_buff->length());
}

void SpriteBatchSystem::createEffect()
{
	D3DVERTEXELEMENT9 VtxElem[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 8, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		{0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	IDirect3DVertexDeclaration9 *declaration;
	m_device->device()->CreateVertexDeclaration(VtxElem, &declaration);
	m_declaration = declaration;

	ID3DXEffect *effect;
	LPD3DXBUFFER error = NULL;

	D3DXCreateEffect(
		m_device->device(), g_main, sizeof(g_main), NULL, NULL,
		D3DXSHADER_SKIPVALIDATION, NULL, &effect, &error);

	//D3DXCreateEffectFromFile(
	//	m_device->device(), _T("ParticleVertexEffect.fx"), NULL, NULL,
	//	0, NULL, &effect, &error );

	m_effect = effect;
}

void SpriteBatchSystem::blendMode(COLOR_BLEND mode)
{
	switch(mode)
	{
	case BLEND_NOUSE_ALPHA:
		//アルファブレンドなし
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		break;
	case BLEND_DEFAULT:
		//デフォルトの合成
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_device->device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_device->device()->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_ADD);
		break;
	case BLEND_MODURATE:
		//乗算合成
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		m_device->device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		m_device->device()->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_ADD);
		break;
	case BLEND_INVERSE:
		//反転合成
		//これを設定し白色テクスチャを反転したい部分に描画してやる
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		m_device->device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		m_device->device()->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_ADD);
		break;
	case BLEND_ADDITION1:
		//加算合成1
		//半分だけ加算
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_device->device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_device->device()->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_ADD);
		break;
	case BLEND_ADDITION2:
		//加算合成2
		//完全加算
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		m_device->device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_device->device()->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_ADD);
		break;
	case BLEND_SUBTRACT1:
		//減算合成1	転送元 - 転送先
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_device->device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_device->device()->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_SUBTRACT);
		break;
	case BLEND_SUBTRACT2:
		//減算合成2	転送先 - 転送元
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_device->device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_device->device()->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_REVSUBTRACT);
		break;
	}
}
}
}