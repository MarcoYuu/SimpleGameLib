

//-----------------------------------------------------------------------------
//
// �V�F�[�_�̎��O�R���p�C���ɂ���
//
// fxc /T fx_2_0 /Fo Sample.fxo Sample.fx
// 
// /T �I�v�V�����́A�V�F�[�_�[�̃o�[�W�������w�肵�܂��B 
// DirectX 9.0 SDK December 2004�ɓY�t����Ă���R���p�C��(fxc.exe)��
// �R���p�C�������ꍇfx_2_0�����g�p�ł��܂���B 
// �ł���SM3.0�ŋL�q���ꂽfx�t�@�C�����g�p���Ă����Ȃ����삵�܂��B
// �C�}�C�`�Ӗ��s�������Afx_2_0�Ƃ��Ƃ��΂����Ǝv���܂��B 
// /Fo �I�v�V�������w�肷��ƃI�u�W�F�N�g�t�@�C�����쐬���܂��B
// �p�����[�^�͂Q�ō�����R���p�C�����č쐬���ꂽ�t�@�C�����A
// �R���p�C������fx�t�@�C�����ƂȂ�܂��B /Fo �I�v�V������ /Fh�ƕύX�����
// �w�b�_�t�@�C�����o�͂��܂��B
// ���̏ꍇ�o�͂���t�@�C�����̊g���q��fxo����h�ɕύX���܂��B
//
//
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// VertexDemo.fx
// ���I���_�o�b�t�@���g�����p�[�e�B�N���`��p�̃V�F�[�_�[
//=============================================================================

// �r���[�|�[�g���
float2 ViewportSize;	// {width, height }

// �e�N�X�`���T�C�Y
float2 TextureSize;

// �l�p�`���
// xy�ɒ��_�ʒu�Azw��UV���W���i�[����
float4 QuadInfo[] = 
{
	{ -0.5,-0.5, 0, 0 }, // ����
	{  0.5,-0.5, 1, 0 }, // �E��
	{  0.5, 0.5, 1, 1 }, // �E��
	{ -0.5, 0.5, 0, 1 }, // ����
};

// �e�N�X�`���T���v���ݒ�
texture Texture;

sampler TextureSampler = sampler_state
{
	Texture = (Texture);
};

//-----------------------------------------------------------------------------
// �X�N���[�����W����r���[�|�[�g���W�֕ϊ�����
//=============================================================================
float2 ScreenToViewport( float2 screenPos )
{
	// DirectX�̃X�N���[�����W��0.5�s�N�Z���̂���𒼂�
	screenPos -= 0.5f;

	float2 result = screenPos.xy / ViewportSize.xy * 2 - 1;
	result.y = -result.y;
	return result;
	
}

//-----------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[
//=============================================================================
void PS( inout float4 color : COLOR0, float2 texCoord : TEXCOORD0 )
{
	color *= tex2D(TextureSampler, texCoord);
}

//-----------------------------------------------------------------------------
// ���_�f�[�^���Ƀp�[�e�B�N���p�����[�^�[���i�[��
// ���_�V�F�[�_�[���ŉ�]�g��Ȃǂ̏��������钸�_�V�F�[�_�[
//=============================================================================
void ParameterVertexVS(
	float2 position : POSITION0,
	float4 color : COLOR0,
	float4 param : TEXCOORD0,	// {x: Scale, y:Rotation, z:Depth, w:cornerIndex }
	
	out float4 outputPosition : POSITION0,
	out float2 outputTexCoord : TEXCOORD0,
	out float4 outputColor    : COLOR0 )
{
	// �p�����[�^�[���W�X�^��W�J
	float scale = param.x;
	float rotation = param.y;
	float depth = param.z;
	int cornerIndex = param.w;

	// ��]�ƃX�P�[������
	float cs = cos(rotation);
	float sn = sin(rotation);
	
	float2 pos = position + mul( QuadInfo[cornerIndex].xy * TextureSize * scale,
														float2x2(cs, sn, -sn, cs) );
	
	pos = ScreenToViewport(pos);
	
	outputPosition = float4( pos.x, pos.y, depth, 1 );
	
	// �e�N�X�`�����W�̐ݒ�
	outputTexCoord = QuadInfo[cornerIndex].zw;

	// �J���[�͂��̂܂܃X���[	
	outputColor = color;
}

technique ParameterVertex
{
	pass Pass1
	{
		VertexShader = compile vs_2_0 ParameterVertexVS();
		PixelShader = compile ps_2_0 PS();
	}
}