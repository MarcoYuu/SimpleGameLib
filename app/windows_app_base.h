// BasewindowsApp.h
// �A�v���P�[�V�����N���X�w�b�_

// 0.2

#pragma once

#pragma warning(disable:4290)

#include <windows.h>

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include "../other/refference_count.h"
#include "../other/utility.h"

// �����̃��C�u�����̖��O���
namespace yuu
{
// �A�v���P�[�V����
namespace app
{
class WindowBase;
class IApplication;
class WindowsAppBase;
class WindowsGameBase;

// �E�B���h�E�N���X�ւ̃|�C���^
//
// ���̃N���X�ւ̎Q�Ƃ�0�ɂȂ�Ǝ����I�Ƀ������ォ��폜�����B
// �t�Ɍ����΁A�C���X�^���X�ւ̎Q�Ƃ�������胁������ɑ��݂���B
typedef boost::intrusive_ptr<WindowBase> Window;

//--------------------------------------------------------------------------------
//�E�B���h�E�̊��N���X
//--------------------------------------------------------------------------------
// �E�B���h�E�^�C�v
enum WINDOW_TYPE
{
	DEFAULT = WS_OVERLAPPEDWINDOW,	// <�ʏ�
	SIMPLE1 = WS_POPUP | WS_DLGFRAME | WS_SYSMENU | WS_CAPTION,	// <�V���v��1
	SIMPLE2 = WS_OVERLAPPEDWINDOW &~WS_THICKFRAME &~WS_MAXIMIZEBOX	// <�V���v��2
};

// �E�B���h�E��b�N���X
// �p�����ăv���V�[�W����ύX�ł���\n
class WindowBase
	: public IRefferenceCount<WindowBase>, boost::noncopyable
{
public:
	static Window create(
		tstring name, int width = 800, int height = 600,
		DWORD style = SIMPLE2, bool consider_frame = true);
	virtual ~WindowBase() {}

	void destroy();
	void show(bool bShow = true);

	int getWidth() const;
	int getHeight() const;
	HWND getHandle() const;

	void setSize(int x, int y, int width, int height);
	void setTitle(tstring title);

	void showCursor(bool show);

protected:
	virtual LRESULT windowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

private:
	HWND m_hWnd;
	std::wstring m_name;
	std::wstring m_title;
	int m_width;
	int m_height;

	virtual bool initWindowClass();
	virtual bool initWindowInstance(DWORD style, bool consider_frame);

	static LRESULT CALLBACK dummyProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

	WindowBase(
		tstring name, int width = 800, int height = 600,
		DWORD style = SIMPLE2, bool consider_frame = true);
};
//--------------------------------------------------------------------------------
//�A�v���P�[�V�������[�v�̊�b�N���X
//--------------------------------------------------------------------------------
// �A�v���P�[�V�����C���^�t�F�[�X
class IApplication :boost::noncopyable
{
public:
	virtual ~IApplication() {}
	void run();

protected:
	virtual void initialize() = 0;
	virtual void startMainLoop() = 0;
};

//--------------------------------------------------------------------------------
//�ʏ�E�B���h�E�Y�A�v���C���^�t�F�[�X
//--------------------------------------------------------------------------------
// �E�B���h�E�Y�A�v���C���^�t�F�[�X
class WindowsAppBase : public IApplication
{
	typedef IApplication base;
public:
	WindowsAppBase();
	virtual ~WindowsAppBase() {}

protected:
	virtual void initialize();
	virtual void startMainLoop();

	void setWindow(Window window);
	Window getWindow();
	const Window getWindow() const;

private:
	Window m_Window;
};
//--------------------------------------------------------------------------------
//�ʏ�Q�[���A�v���C���^�t�F�[�X
//--------------------------------------------------------------------------------
// �E�B���h�E�Y�Q�[���A�v���C���^�t�F�[�X
class WindowsGameBase : public WindowsAppBase
{
	typedef WindowsAppBase base;
public:
	WindowsGameBase();
	virtual ~WindowsGameBase() {}

protected:
	void startMainLoop();
	virtual void initialize();
	virtual void mainProcess();
};
}
}