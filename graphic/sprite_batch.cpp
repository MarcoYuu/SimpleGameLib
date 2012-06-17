
#include <d3d9.h>
#include <d3dx9.h>

#include <vector>

#include <graphic/buffer_object.h>
#include <graphic/color.h>
#include <graphic/texture.h>
#include <graphic/sprite.h>
#include <graphic/sprite_batch.h>
#include <graphic/graphics_device.h>

#include <other/com_release.h>

// �����̃��C�u�����̖��O���
namespace yuu
{
// �O���t�B�b�N�`��֘A
namespace graphic
{

const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

struct SpriteVertex
{
	float x, y;//,z	// D3DFVF_XYZ
	DWORD color;	// D3DFVF_DIFFUSE
	float scale;	// D3DFVF_TEX1
	float rotation;	// D3DFVF_TEX1
	float depth;	// D3DFVF_TEX1
	float index;	// D3DFVF_TEX1
};

struct SpriteBatchSystem::Param{
	typedef std::vector<SpriteVertex> SpriteVertexList;

	GraphicDevice device;
	Texture texture;

	VertexBuffer vertex_buff;
	IndexBuffer index_buff;
	SpriteVertexList vertex_list;

	boost::intrusive_ptr<ID3DXEffect> effect;
	boost::intrusive_ptr<IDirect3DVertexDeclaration9> declaration;

	size_t current_sprite_num;
	size_t max_sprite_num;

	Param(GraphicDevice device_, size_t max_sprite_)
		: device(device_)
		, texture()
		, vertex_buff()
		, index_buff()
		, vertex_list(max_sprite_ * 4)
		, effect()
		, declaration()
		, current_sprite_num(0)
		, max_sprite_num(max_sprite_)
	{}
};

SpriteBatchSystem::SpriteBatchSystem(GraphicDevice device, size_t max_sprite)
	: param(new Param(device, max_sprite))
{
	param->vertex_buff = DynamicVertexBufferObject::create(device, max_sprite * sizeof(SpriteVertex) * 4, FVF);
	param->index_buff  = IndexBufferObject::create(device, IndexBufferObject::SIZE_16BIT, sizeof(short) * 6 * max_sprite);

	setIndices();
	createEffect();
}

SpriteBatch SpriteBatchSystem::create(GraphicDevice device, size_t size)
{
	return SpriteBatch(new SpriteBatchSystem(device, size));
}

void SpriteBatchSystem::begin()
{
	//���C�g�ƃJ�����O�I�t
	param->device->enableLighting(false);
	param->device->setCullingMode(NONE);
}
void SpriteBatchSystem::end()
{
	// ���_�̏�������
	size_t offset = param->vertex_buff->write(param->vertex_list.data(), param->current_sprite_num * 4 * sizeof(SpriteVertex));

	// ��Ԑݒ�
	SizeFloat back_buf =param->device->getBackBufferSize().type_cast<float>();
	SizeFloat tex_size =param->texture->getSize().type_cast<float>();
	param->effect->SetTechnique("ParameterVertex");
	param->effect->SetFloatArray("ViewportSize", back_buf.toAry(), 2);
	if( param->texture!=NULL && 
		param->texture->getHandle()!=NULL)
	{
		param->effect->SetFloatArray("TextureSize", tex_size.toAry(), 2);
		param->effect->SetTexture("Texture", (IDirect3DTexture9*)param->texture->getHandle());
	}

	IDirect3DDevice9* dev =(IDirect3DDevice9*)param->device->getHandle();

	// �p�X�̊J�n
	UINT nupass;
	param->effect->Begin(&nupass, 0);
	param->effect->BeginPass(0);
	{
		// �`�悷�钸�_�̐ݒ�
		param->device->setVertexBuffer(param->vertex_buff, 0, 0, sizeof(SpriteVertex));
		param->device->setIndexBuffer(param->index_buff);

		// ���_�錾�̐ݒ�
		dev->SetVertexDeclaration(param->declaration.get());

		// �`��
		dev->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST, 0,
			(UINT)offset / sizeof(SpriteVertex),				// �ŏ��C���f�b�N�X�A�o�b�t�@�ɂ��炩���ߊ܂܂�钸�_�̌�
			(UINT)param->current_sprite_num * 4,					// �ŏ��C���f�b�N�X�ȍ~�Ɋ܂܂�钸�_���B�Ƃ肠�����X�v���C�g�̌����猈�ߑł�
			(UINT)offset * 6 / (sizeof(SpriteVertex) * 4),	// �`�揈������C���f�b�N�X�܂ł̃I�t�Z�b�g�B�����Ă����X�v���C�g��*6
			(UINT)param->current_sprite_num * 2					// �`�悳���|���S���̌�
		);
	}
	// �p�X�̏I��
	param->effect->EndPass();
	param->effect->End();

	//�����_�[�X�e�[�g���f�t�H���g�ɖ߂�
	param->device->enableLighting(true);
	param->device->setCullingMode(RIGHT_HANDED);

	//device->getHandle()->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

	//�f�t�H���g�̃e�N�X�`���u�����h�ݒ�
	dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	dev->SetTextureStageState(0, D3DTSS_COLOROP,	 D3DTOP_MODULATE);
	dev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	dev->SetTextureStageState(0, D3DTSS_ALPHAOP,	 D3DTOP_MODULATE);
	dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// �����I������̂ŃX�v���C�g�̌���������
	param->current_sprite_num = 0;
}

void SpriteBatchSystem::setTexture(Texture texture)
{
	param->texture = texture;
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
	if(param->current_sprite_num < param->max_sprite_num)
	{
		size_t cur_index = param->current_sprite_num * 4;
		// ����E�E��E�E���E�����̏��ɒ��_���i�[
		for(int i = 0; i < 4; ++i)
		{
			param->vertex_list[cur_index + i].x        = position.x;
			param->vertex_list[cur_index + i].y        = position.y;
			param->vertex_list[cur_index + i].color    = color;
			param->vertex_list[cur_index + i].scale    = scale;
			param->vertex_list[cur_index + i].rotation = rotation;
			param->vertex_list[cur_index + i].depth    = depth;
			param->vertex_list[cur_index + i].index    = (float)i;
		}

		++param->current_sprite_num;
	}
}

void SpriteBatchSystem::setIndices()
{
	// ��̎l�p�`��`�悷��ɂ̓C���f�b�N�X��6�K�v
	// ���_�̕��т͈ȉ��̒ʂ�
	//   0   1
	//   +---+
	//   |   |
	//   +---+
	//   3   2
	size_t indexCount = param->max_sprite_num * 6;
	std::vector<short> indices(indexCount);

	short vertexIndex = 0;
	for(size_t i = 0; i < indexCount; i += 6)
	{
		indices[i + 0] = (short)(vertexIndex + 0);
		indices[i + 1] = (short)(vertexIndex + 1);
		indices[i + 2] = (short)(vertexIndex + 2);

		indices[i + 3] = (short)(vertexIndex + 0);
		indices[i + 4] = (short)(vertexIndex + 2);
		indices[i + 5] = (short)(vertexIndex + 3);

		vertexIndex += 4;
	}
	param->index_buff->write(indices.data(), param->index_buff->length());
}

void SpriteBatchSystem::createEffect()
{
	D3DVERTEXELEMENT9 VtxElem[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0,  8, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		{0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	IDirect3DVertexDeclaration9 *declaration;
	IDirect3DDevice9* dev =(IDirect3DDevice9*)param->device->getHandle();
	dev->CreateVertexDeclaration(VtxElem, &declaration);
	param->declaration = declaration;

	ID3DXEffect *effect;
	LPD3DXBUFFER error = NULL;

	D3DXCreateEffect(
		dev, g_main, sizeof(g_main), NULL, NULL,
		D3DXSHADER_SKIPVALIDATION, NULL, &effect, &error);

	param->effect = effect;
}

void SpriteBatchSystem::blendMode(ColorBlendMode mode)
{
	IDirect3DDevice9* dev =(IDirect3DDevice9*)param->device->getHandle();
	switch(mode)
	{
	case BLEND_NOUSE_ALPHA:
		//�A���t�@�u�����h�Ȃ�
		dev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		break;
	case BLEND_DEFAULT:
		//�f�t�H���g�̍���
		dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		dev->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_ADD);
		break;
	case BLEND_MODURATE:
		//��Z����
		dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		dev->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_ADD);
		break;
	case BLEND_INVERSE:
		//���]����
		//�����ݒ肵���F�e�N�X�`���𔽓]�����������ɕ`�悵�Ă��
		dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		dev->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_ADD);
		break;
	case BLEND_ADDITION1:
		//���Z����1
		//�����������Z
		dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		dev->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_ADD);
		break;
	case BLEND_ADDITION2:
		//���Z����2
		//���S���Z
		dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		dev->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_ADD);
		break;
	case BLEND_SUBTRACT1:
		//���Z����1	�]���� - �]����
		dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		dev->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_SUBTRACT);
		break;
	case BLEND_SUBTRACT2:
		//���Z����2	�]���� - �]����
		dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		dev->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_REVSUBTRACT);
		break;
	}
}
}
}