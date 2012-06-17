// BasewindowsApp.h
// �A�v���P�[�V�����N���X�w�b�_

// 0.2

#pragma once

#if defined(_WIN32) | defined(_WIN64)
#include <windows.h>
#endif

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include <other/refference_count.h>
#include <other/utility.h>

// �����̃��C�u�����̖��O���
namespace yuu
{
// �A�v���P�[�V����
namespace app
{

class WindowBase;
typedef boost::intrusive_ptr<WindowBase> Window;

//--------------------------------------------------------------------------------
//�E�B���h�E�̊��N���X
//--------------------------------------------------------------------------------
// �E�B���h�E�^�C�v
enum WindowType
{
	WT_DEFAULT,	// �ʏ�
	WT_SIMPLE1,	// �V���v��1
	WT_SIMPLE2	// �V���v��2
};

// �E�B���h�E��b�N���X
class WindowBase
	: public RefferenceCount<WindowBase>
	, boost::noncopyable
{
public:
	static Window create(
		tstring name, 
		int width           = 800, 
		int height          = 600,
		WindowType style    = WT_SIMPLE2, 
		bool consider_frame = true
	);
	virtual ~WindowBase();

	// �E�B���h�E�̔j��
	void destroy();
	// �E�B���h�E�̕\���E��\���̐؂�ւ�
	void show(bool bShow = true);

	// ���̎擾
	int getWidth() const;
	// �����̎擾
	int getHeight() const;
	// �n���h���̎擾
	void* getHandle() const;
	// �E�B���h�E���̎擾
	const tstring& getName() const;

	void setSize(int x, int y, int width, int height);
	void setName(tstring title);

	bool isActive();
	void showCursor(bool show);

protected:
#if defined(_WIN32) | defined(_WIN64)
	// �v���V�[�W��(���������\)
	virtual LRESULT callbackProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
	// �I�[�o�[���C�h����ꍇ�E�B���h�E�N���X�̓o�^�����Ȃ���΂Ȃ�Ȃ�
	virtual bool initWindowClass();
	// ���ۂɃE�B���h�E���쐬���A�n���h����o�^���Ȃ���΂Ȃ�Ȃ�
	virtual bool initWindowInstance(WindowType style, bool consider_frame);
	// �n���h���̓o�^
	void setHandle(void* handle);
#endif

private:
	struct Param;
	Param *param;

	WindowBase(tstring name, int width, int height, WindowType style, bool consider_frame);

#if defined(_WIN32) | defined(_WIN64)
	// �E�B���h�E�v���V�[�W���p�̃O���[�o���֐�
	static LRESULT CALLBACK procedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
#endif	
};
}
}