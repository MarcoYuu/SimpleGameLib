// InputInterface.h
// ���̓C���^�t�F�[�X�w�b�_
// �ȈՐ���
// 			���̓C���^�t�F�[�X�̃w�b�_
//
// 0.1
#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800
#endif

#include <dinput.h>
#include <stdexcept>

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include "../other/refference_count.h"

// �����̃��C�u�����̖��O���
namespace yuu
{
// ���͊֘A
namespace input
{
//--------------------------------------------------------------------------------------
//���̓C���^�t�F�[�X
//--------------------------------------------------------------------------------------

class IController;
class IKeyBoardInput;
class IGamePadInput;
class IPointingDeviceInput;

// _IController�N���X�ւ̃|�C���^
typedef boost::intrusive_ptr<IController> Controller;
// _IKeyBoardInput�N���X�ւ̃|�C���^
typedef boost::intrusive_ptr<IKeyBoardInput> KeyBoard;
// _IGamePadInput�N���X�ւ̃|�C���^
typedef boost::intrusive_ptr<IGamePadInput> GamePad;
// _IPointingDeviceInput�N���X�ւ̃|�C���^
typedef boost::intrusive_ptr<IPointingDeviceInput> PointingDevice;

//-----------------------------------------------------------------------------------
//�{�^���񋓎q
//-----------------------------------------------------------------------------------
// �{�^���̎擾���@
enum STATE_TYPE
{
	PRESENT,	// <���݂̏�Ԃ��擾����
	JUST_DOWN,	// <�����ꂽ���ォ���擾����
	JUST_UP		// <�����ꂽ���ォ���擾����
};
// �R���g���[���[�̃{�^��
enum CONTROL_BUTTON
{
	CB_BUTTON_0,
	CB_BUTTON_1,
	CB_BUTTON_2,
	CB_BUTTON_3,
	CB_BUTTON_4,
	CB_BUTTON_5,
	CB_BUTTON_6,
	CB_BUTTON_7,
	CB_BUTTON_8,
	CB_BUTTON_9,
	CB_BUTTON_10,
	CB_BUTTON_11,
	CB_BUTTON_12,
	CB_BUTTON_13,
	CB_BUTTON_14,
	CB_BUTTON_15,
	CB_BUTTON_LEFT,
	CB_BUTTON_RIGHT,
	CB_BUTTON_UP,
	CB_BUTTON_DOWN,
	CB_BUTTON_NUM,
	BUTTON_NONE = 0xffffffff
};
// �L�[�{�[�h�̃{�^��
enum KEYBOARD_BUTTON
{
	KB_BUTTON_A         = DIK_A,
	KB_BUTTON_B         = DIK_B,
	KB_BUTTON_C         = DIK_C,
	KB_BUTTON_D         = DIK_D,
	KB_BUTTON_E         = DIK_E,
	KB_BUTTON_F         = DIK_F,
	KB_BUTTON_G         = DIK_G,
	KB_BUTTON_H         = DIK_H,
	KB_BUTTON_I         = DIK_I,
	KB_BUTTON_J         = DIK_J,
	KB_BUTTON_K         = DIK_K,
	KB_BUTTON_L         = DIK_L,
	KB_BUTTON_M         = DIK_M,
	KB_BUTTON_N         = DIK_N,
	KB_BUTTON_O         = DIK_O,
	KB_BUTTON_P         = DIK_P,
	KB_BUTTON_Q         = DIK_Q,
	KB_BUTTON_R         = DIK_R,
	KB_BUTTON_S         = DIK_S,
	KB_BUTTON_T         = DIK_T,
	KB_BUTTON_U         = DIK_U,
	KB_BUTTON_V         = DIK_V,
	KB_BUTTON_W         = DIK_W,
	KB_BUTTON_X         = DIK_X,
	KB_BUTTON_Y         = DIK_Y,
	KB_BUTTON_Z         = DIK_Z,
	KB_BUTTON_SHIFT		= DIK_LSHIFT,
	KB_BUTTON_CTRL		= DIK_LCONTROL,
	KB_BUTTON_ESCAPE	= DIK_ESCAPE,
	KB_BUTTON_DELETE	= DIK_DELETE,
	KB_BUTTON_LEFT		= DIK_LEFT,
	KB_BUTTON_RIGHT		= DIK_RIGHT,
	KB_BUTTON_UP		= DIK_UP,
	KB_BUTTON_DOWN		= DIK_DOWN,
	KB_BUTTON_1         = DIK_1,
	KB_BUTTON_2         = DIK_2,
	KB_BUTTON_3         = DIK_3,
	KB_BUTTON_4         = DIK_4,
	KB_BUTTON_5         = DIK_5,
	KB_BUTTON_6         = DIK_6,
	KB_BUTTON_7         = DIK_7,
	KB_BUTTON_8         = DIK_8,
	KB_BUTTON_9         = DIK_9,
	KB_BUTTON_0         = DIK_0,
	KB_BUTTON_NUM
};
// �}�E�X�̃{�^��
enum MOUSE_BUTTON
{
	MB_BUTTON_0,
	MB_BUTTON_1,
	MB_BUTTON_2,
	MB_BUTTON_3,
	MB_BUTTON_NUM
};
// �Q�[���p�b�h�̃{�^��
enum GAMEPAD_BUTTON
{
	GB_BUTTON_0,
	GB_BUTTON_1,
	GB_BUTTON_2,
	GB_BUTTON_3,
	GB_BUTTON_4,
	GB_BUTTON_5,
	GB_BUTTON_6,
	GB_BUTTON_7,
	GB_BUTTON_8,
	GB_BUTTON_9,
	GB_BUTTON_10,
	GB_BUTTON_11,
	GB_BUTTON_12,
	GB_BUTTON_13,
	GB_BUTTON_14,
	GB_BUTTON_15,
	GB_BUTTON_16,
	GB_BUTTON_17,
	GB_BUTTON_18,
	GB_BUTTON_19,
	GB_BUTTON_20,
	GB_BUTTON_21,
	GB_BUTTON_22,
	GB_BUTTON_23,
	GB_BUTTON_24,
	GB_BUTTON_25,
	GB_BUTTON_26,
	GB_BUTTON_27,
	GB_BUTTON_28,
	GB_BUTTON_29,
	GB_BUTTON_30,
	GB_BUTTON_31,
	GB_BUTTON_LEFT,
	GB_BUTTON_RIGHT,
	GB_BUTTON_UP,
	GB_BUTTON_DOWN,
	GB_BUTTON_NUM,
	GB_ANALOG_X,
	GB_ANALOG_Y,
	GB_ANALOG_Z
};

//-----------------------------------------------------------------//
//�R���g���[���[�C���^�t�F�[�X
//-----------------------------------------------------------------//
// �R���g���[���C���^�t�F�[�X
//
// ���z�I�ȃQ�[���R���g���[���N���X
//
class IController
	: public yuu::IRefferenceCount<IController>
	, boost::noncopyable
{
public:
	virtual ~IController() {}

	// ���͍X�V
	// �Ȃ�
	virtual void update() = 0;
	// �{�^���̏�Ԃ��擾
	// STATE_TYPE
	// �擾����{�^��
	// �擾���
	// �{�^���̏��
	virtual bool getButtonState(CONTROL_BUTTON ctrl, STATE_TYPE get_type = PRESENT) = 0;
	// ���̏�Ԃ��擾
	// STATE_TYPE
	// ���������@:-1.0�`1.0�Œl��Ԃ�����
	// ���������@:-1.0�`1.0�Œl��Ԃ�����
	// ���������@:-1.0�`1.0�Œl��Ԃ�����
	// �Ȃ�
	virtual void getAnalogAxisState(double &x, double &y, double &z) = 0;
	// ���̉�]���擾
	// STATE_TYPE
	// ��������
	// ��������
	// ��������
	// �Ȃ�
	virtual void getAnalogRotateState(double &rx, double &ry, double &rz) = 0;
	// �ω��ʂ��擾
	// ���s����
	// ��������
	// ���
	virtual void getRelativePos(double &x, double &y) = 0;
	// ��Έʒu���擾
	// ���s����
	// ��������
	// ���
	virtual void getAbsScreenPos(double &x, double &y) = 0;
	// �z�C�[���̕ω��ʂ��擾
	// �ω���
	// ���
	virtual void getWheelMove(double &z) = 0;

	// �{�^���ݒ��������Ԃɖ߂�
	// �Ȃ�
	virtual void setDefault() = 0;
	// �L�[�{�[�h�̓��͂��{�^���ɐݒ肷��
	// STATE_TYPE
	// �ݒ肳���{�^��
	// �ݒ肷��L�[�{�[�h�̃L�[
	// �Ȃ�
	virtual void setConfigKeyBoard(CONTROL_BUTTON ctrl, KEYBOARD_BUTTON key) = 0;
	// �|�C���e�B���O�f�o�C�X�̓��͂��{�^���ɐݒ肷��
	// STATE_TYPE
	// �ݒ肳���{�^��
	// �ݒ肷��}�E�X�̃{�^��
	// �Ȃ�
	virtual void setConfigMouse(CONTROL_BUTTON ctrl, MOUSE_BUTTON key) = 0;
	// �Q�[���p�b�h�̓��͂��{�^���ɐݒ肷��
	// STATE_TYPE
	// �ݒ肳���{�^��
	// �ݒ肷��Q�[���p�b�h�̂̃{�^��
	// �Ȃ�
	virtual void setConfigGamepad(CONTROL_BUTTON ctrl, GAMEPAD_BUTTON key) = 0;
};

//-----------------------------------------------------------------//
//�L�[�{�[�h���͏����C���^�t�F�[�X
//-----------------------------------------------------------------//
// �L�[�{�[�h�C���^�t�F�[�X
//
// ���z�I�ȃL�[�{�[�h�N���X
//
class IKeyBoardInput
	: public yuu::IRefferenceCount<IKeyBoardInput>
	, boost::noncopyable
{
public:
	virtual ~IKeyBoardInput() {}

	// ���͏����X�V
	// �Ȃ�
	virtual void update() = 0;

	// �L�[�̏����擾
	// �擾����L�[
	// �擾���
	// ���
	virtual bool getKeyState(KEYBOARD_BUTTON key, STATE_TYPE get_type = PRESENT) = 0;
};

//-----------------------------------------------------------------//
//�Q�[���p�b�h���͏����C���^�t�F�[�X
//-----------------------------------------------------------------//
// �Q�[���p�b�h�C���^�t�F�[�X
//
// ���z�I�ȃQ�[���p�b�h�N���X
//
class IGamePadInput
	: public yuu::IRefferenceCount<IGamePadInput>
	, boost::noncopyable
{
public:
	virtual ~IGamePadInput() {}

	// ���͏����X�V
	// �Ȃ�
	virtual void update() = 0;
	// �{�^���̏����擾
	// �擾����{�^��
	// �擾���
	// ���
	virtual bool getButtonState(GAMEPAD_BUTTON button, STATE_TYPE get_type = PRESENT) = 0;
	// ���̏�Ԃ��擾
	// STATE_TYPE
	// ���������@:-1.0�`1.0�Œl��Ԃ�����
	// ���������@:-1.0�`1.0�Œl��Ԃ�����
	// ���������@:-1.0�`1.0�Œl��Ԃ�����
	// �Ȃ�
	virtual bool getAnalogAxisState(double &x, double &y, double &z) = 0;
	// ���̏�Ԃ��擾
	// STATE_TYPE
	// ��������
	// ��������
	// ��������
	// �Ȃ�
	virtual bool getAnalogRotateState(double &rx, double &ry, double &rz) = 0;
};

//-----------------------------------------------------------------//
//�|�C���e�B���O�f�o�C�X�̓��͏����C���^�t�F�[�X
//-----------------------------------------------------------------//
// �|�C���e�B���O�f�o�C�X�C���^�t�F�[�X
//
// ���z�I�ȃ|�C���e�B���O�f�o�C�X�N���X
//
class IPointingDeviceInput
	: public yuu::IRefferenceCount<IPointingDeviceInput>
	, boost::noncopyable
{
public:
	virtual ~IPointingDeviceInput() {}

	// ���͏����X�V
	// �Ȃ�
	virtual void update() = 0;
	// �{�^���̏����擾
	// �擾����{�^��
	// �擾���
	// ���
	virtual bool getButtonState(MOUSE_BUTTON button, STATE_TYPE get_type = PRESENT) = 0;
	// �ω��ʂ��擾
	// ���s����
	// ��������
	// ���
	virtual bool getRelativePos(double &x, double &y) = 0;
	// ��Έʒu���擾
	// ���s����
	// ��������
	// ���
	virtual bool getAbsScreenPos(double &x, double &y) = 0;
	// �z�C�[���̕ω��ʂ��擾
	// �ω���
	// ���
	virtual bool getWheelMove(double &z) = 0;
};
}
}