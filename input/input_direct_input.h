// InputAsDirectInput.h
// DirectInput��p�������̓N���X�w�b�_
// �ȈՐ���
// 			DirectInput��p�������̓N���X�̃w�b�_
//
// 0.1
#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800
#endif

#include <dinput.h>
#include "input_interface.h"

// �����̃��C�u�����̖��O���
namespace yuu
{
// ���͊֘A
namespace input
{
//--------------------------------------------------------------------------------------
//DirectInput�̏�����
//--------------------------------------------------------------------------------------
// ���͂̏�����
//
// ���̓f�o�C�X�����������܂��B
// �������Ɏ��s�����ꍇ��O�𓊂��܂��B
// CreateAudio
// �Ȃ�
void InitInput();
Controller CreateDirectInputController(HWND hWnd);

//--------------------------------------------------------------------------------------
//�L�[�{�[�h����DirectInput
//--------------------------------------------------------------------------------------
class DirectInputKeyBoard : public IKeyBoardInput
{
	IDirectInputDevice8 *m_pKeyBoardDev;

	BYTE m_KeyState[256];
	BYTE m_PrevKeyState[256];

public:
	//�R���X�g���N�^��f�X�g���N�^
	DirectInputKeyBoard(HWND hWnd);
	~DirectInputKeyBoard(void);

	// �Q�ƃJ�E���^�t�|�C���^���쐬
	// ���͂��󂯕t����E�B���h�E�̃n���h��
	// �쐬�ł��Ȃ������ꍇNULL
	static KeyBoard Create(HWND hWnd);

	void update();
	bool getKeyState(KEYBOARD_BUTTON key, STATE_TYPE get_type);
};

//--------------------------------------------------------------------------------------
//�Q�[���p�b�h���� by DirectInput
//--------------------------------------------------------------------------------------
class DirectInputGamePad : public IGamePadInput
{
	IDirectInputDevice8 *m_pGamePad;

	DIJOYSTATE m_GamePadState;
	DIJOYSTATE m_PrevGamePadState;

	//�p�b�h�񋓊֐�
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *lpddi, LPVOID lpContext);
	//���̐ݒ�
	static BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID lpvRef);

public:
	//�R���X�g���N�^��f�X�g���N�^
	DirectInputGamePad(HWND hWnd);
	~DirectInputGamePad();

	// �Q�ƃJ�E���^�t�|�C���^���쐬
	// ���͂��󂯕t����E�B���h�E�̃n���h��
	// �쐬�ł��Ȃ������ꍇNULL
	static GamePad Create(HWND hWnd);

	void update();
	bool getButtonState(GAMEPAD_BUTTON button, STATE_TYPE get_type = PRESENT);
	bool getAnalogAxisState(double &x, double &y, double &z);
	bool getAnalogRotateState(double &rx, double &ry, double &rz);
};

//--------------------------------------------------------------------------------------
//�}�E�X���� by DirectInput
//--------------------------------------------------------------------------------------
class DirectInputMouse : public IPointingDeviceInput
{
	IDirectInputDevice8 *m_pMouse;

	DIMOUSESTATE m_MouseState;
	DIMOUSESTATE m_PrevMouseState;

	HWND m_hWnd;

public:
	DirectInputMouse(HWND hWnd);
	~DirectInputMouse();

	// �Q�ƃJ�E���^�t�|�C���^���쐬
	// ���͂��󂯕t����E�B���h�E�̃n���h��
	// �쐬�ł��Ȃ������ꍇNULL
	static PointingDevice Create(HWND hWnd);

	void update();
	bool getButtonState(MOUSE_BUTTON button, STATE_TYPE get_type = PRESENT);
	bool getRelativePos(double &x, double &y);
	bool getAbsScreenPos(double &x, double &y);
	bool getWheelMove(double &z);
};
}
}