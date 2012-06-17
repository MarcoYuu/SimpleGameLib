#pragma once

#include <boost/utility.hpp>

#include <app/windows_window_base.h>
#include <other/refference_count.h>
#include <other/utility.h>
#include <graphic/forward_declaration.h>

// �����̃��C�u�����̖��O���
namespace yuu
{
// �O���t�B�b�N�`��֘A
namespace graphic
{
//--------------------------------------------------------------------------------------------------
//DirectX9�f�o�C�X�Ǘ��N���X
//--------------------------------------------------------------------------------------------------
struct Rectangle{
	long x1, y1;
	long x2, y2;

	Rectangle();
	Rectangle(const Point &p1, const Point &p2);
	Rectangle(long x1_, long y1_, long x2_, long y2_);

	long width() const{return x2-x1;}
	long height() const{return y2-y1;}
};

enum BackBufferSize
{
	WS_320x240,
	WS_640x480,
	WS_800x600,
	WS_1024x768,
	WS_1280x960,
	WS_1920x1080
};
Size GetSize(BackBufferSize);

enum CullingMode
{
	NONE, 
	RIGHT_HANDED, 
	LEFT_HANDED
};

enum TextureTransformMode{
	DISABLE,
	COORD_1D,
	COORD_2D,
	COORD_3D,
	COORD_4D,
	PROJECTED
};

enum TextureAddressingMode{
	WRAP,
	MIRROR,
	CLAMP,
	BORDER,
	MIRROR_ONCE
};

enum ClearTarget{
	RENDERING_TARGET =0x01,
	STENCIL_BUFFER   =0x02,
	Z_BUFFER         =0x04,

	RENDER_STENCIL   =RENDERING_TARGET|STENCIL_BUFFER,
	RENDER_Z         =RENDERING_TARGET|Z_BUFFER,
	Z_STENCIL        =Z_BUFFER|STENCIL_BUFFER,
	ALL_TARGET       =RENDERING_TARGET|Z_STENCIL
};

class GraphicDeviceManager
	: public RefferenceCount<GraphicDeviceManager>
	, boost::noncopyable
{
public:
	static GraphicDevice create(
		app::Window window, 
		BackBufferSize size = WS_800x600, 
		bool bWindowed      = true, 
		bool bVSync         = true
	);
	~GraphicDeviceManager();

	// �o�b�N�o�b�t�@�̃T�C�Y���擾
	const Size& getBackBufferSize() const;

	// ���C�e�B���O��L����
	void enableLighting(bool enable);
	// �e�N�X�`���ϊ���L����
	void enableTextureTransform(int stage, TextureTransformMode);
	
	// �J�����O�̐ݒ�
	void setCullingMode(CullingMode mode);
	// �e�N�X�`���A�h���b�V���O�̐ݒ�
	void setTextureAddressingMode(int stage, TextureAddressingMode u, TextureAddressingMode v);
	// ���_�o�b�t�@�̐ݒ�
	void setVertexBuffer(VertexBuffer buff, int stream, int offset, int stride);
	// ���_�C���f�b�N�X��ݒ�
	void setIndexBuffer(IndexBuffer buff);

	// �`��̊J�n
	void begin();
	// �`��̏I��
	void end();
	// ��ʂ̃t���b�v
	int present();

	// �����_�[�^�[�Q�b�g�̕ύX
	void setRenderTarget(RenderTerget target, int index);
	void resetRenderTargetToBackBuffer();

	// �o�b�t�@���N���A
	void clear();
	void clear(Color color);
	void clear(ClearTarget target, Color color);
	void clear(ClearTarget target, Color color, const Rectangle *area, int rect_num);
	void clear(ClearTarget target, Color color, const Rectangle *area, int rect_num, float z, int stencil);
	
	// �����̃n���h�����擾
	void* getHandle();
	const void* getHandle() const;

private:
	struct Param;
	Param *param;

	GraphicDeviceManager();
	void Init(app::Window window, 
		int WindowWidth, int WindowHeight, 
		bool bWindowed, bool bVSync);
};
}
}