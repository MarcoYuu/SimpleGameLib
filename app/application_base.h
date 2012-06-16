#pragma once

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include <app/windows_window_base.h>
#include <other/refference_count.h>
#include <other/utility.h>

// �����̃��C�u�����̖��O���
namespace yuu
{
// �A�v���P�[�V����
namespace app
{

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