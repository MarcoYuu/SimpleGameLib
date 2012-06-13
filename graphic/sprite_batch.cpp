#include "sprite_batch.h"
#include "sprite.h"

// �����̃��C�u�����̖��O���
namespace yuu
{
// �O���t�B�b�N�`��֘A
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
	//���C�g�ƃJ�����O�I�t
	m_device->device()->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//�e�N�X�`���ϊ�����
	m_device->device()->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);

	//�u�����h�t�@�N�^�[��p����
	m_device->device()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_device->device()->SetTextureStageState(0, D3DTSS_COLOROP,	 D3DTOP_MODULATE);
	m_device->device()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	m_device->device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_device->device()->SetTextureStageState(0, D3DTSS_ALPHAOP,	 D3DTOP_MODULATE);
	m_device->device()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
}
void SpriteBatchSystem::end()
{
	// ���_�̏�������
	size_t offset = m_vertex_buff->write(&m_vertex_list[0], m_current_sprite_num * 4 * sizeof(SpriteVertex));

	// ��Ԑݒ�
	m_effect->SetTechnique("ParameterVertex");
	m_effect->SetFloatArray("ViewportSize", m_device->getBackBufferSize().toAry(), 2);
	if(m_texture!=NULL && m_texture->get()!=NULL){
		m_effect->SetFloatArray("TextureSize", m_texture->getSize().toAry(), 2);
		m_effect->SetTexture("Texture", m_texture->get());
	}

	// �p�X�̊J�n
	UINT num_pass;
	m_effect->Begin(&num_pass, 0);
	m_effect->BeginPass(0);
	{
		// �`�悷�钸�_�̐ݒ�
		m_device->device()->SetStreamSource(0, m_vertex_buff->get(), 0, sizeof(SpriteVertex));
		m_device->device()->SetIndices(m_index_buff->get());
		//m_device->device()->SetTexture(0, m_texture->get());

		// ���_�錾�̐ݒ�
		m_device->device()->SetVertexDeclaration(m_declaration.get());

		// �`��
		m_device->device()->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST, 0,
			offset / sizeof(SpriteVertex),		// �ŏ��C���f�b�N�X�A�o�b�t�@�ɂ��炩���ߊ܂܂�钸�_�̌�
			m_current_sprite_num * 4,				// �ŏ��C���f�b�N�X�ȍ~�Ɋ܂܂�钸�_���B�Ƃ肠�����X�v���C�g�̌����猈�ߑł�
			offset * 6 / (sizeof(SpriteVertex) * 4),	// �`�揈������C���f�b�N�X�܂ł̃I�t�Z�b�g�B�����Ă����X�v���C�g��*6
			m_current_sprite_num * 2				// �`�悳���|���S���̌�
		);
		//m_device->device()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, current_sprite_num*4, 0, current_sprite_num*2);
	}
	// �p�X�̏I��
	m_effect->EndPass();
	m_effect->End();

	//�����_�[�X�e�[�g���f�t�H���g�ɖ߂�
	m_device->device()->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_device->device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	m_device->device()->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

	//�f�t�H���g�̃e�N�X�`���u�����h�ݒ�
	m_device->device()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_device->device()->SetTextureStageState(0, D3DTSS_COLOROP,	 D3DTOP_MODULATE);
	m_device->device()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_device->device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_device->device()->SetTextureStageState(0, D3DTSS_ALPHAOP,	 D3DTOP_MODULATE);
	m_device->device()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// �����I������̂ŃX�v���C�g�̌���������
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
		// ����E�E��E�E���E�����̏��ɒ��_���i�[
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
	// ��̎l�p�`��`�悷��ɂ̓C���f�b�N�X��6�K�v
	// ���_�̕��т͈ȉ��̒ʂ�
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
		//�A���t�@�u�����h�Ȃ�
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		break;
	case BLEND_DEFAULT:
		//�f�t�H���g�̍���
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_device->device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_device->device()->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_ADD);
		break;
	case BLEND_MODURATE:
		//��Z����
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		m_device->device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
		m_device->device()->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_ADD);
		break;
	case BLEND_INVERSE:
		//���]����
		//�����ݒ肵���F�e�N�X�`���𔽓]�����������ɕ`�悵�Ă��
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		m_device->device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		m_device->device()->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_ADD);
		break;
	case BLEND_ADDITION1:
		//���Z����1
		//�����������Z
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_device->device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_device->device()->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_ADD);
		break;
	case BLEND_ADDITION2:
		//���Z����2
		//���S���Z
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		m_device->device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_device->device()->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_ADD);
		break;
	case BLEND_SUBTRACT1:
		//���Z����1	�]���� - �]����
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_device->device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_device->device()->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_SUBTRACT);
		break;
	case BLEND_SUBTRACT2:
		//���Z����2	�]���� - �]����
		m_device->device()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_device->device()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_device->device()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_device->device()->SetRenderState(D3DRS_BLENDOP,  D3DBLENDOP_REVSUBTRACT);
		break;
	}
}
}
}