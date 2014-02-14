
#pragma once

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>
#include "../other/refference_count.h"

namespace yuu
{
namespace input
{
//--------------------------------------------------------------------------------------
//���̓C���^�t�F�[�X
//--------------------------------------------------------------------------------------

class IController;
class IKeyBoardInput;
class IGamePadInput;
class IPointingDeviceInput;

typedef boost::intrusive_ptr<IController> Controller;
typedef boost::intrusive_ptr<IKeyBoardInput> KeyBoard;
typedef boost::intrusive_ptr<IGamePadInput> GamePad;
typedef boost::intrusive_ptr<IPointingDeviceInput> PointingDevice;

//-----------------------------------------------------------------------------------
//�{�^���񋓎q
//-----------------------------------------------------------------------------------
//
// �{�^���̎擾���@
//
enum ButtonState
{
	PRESENT,	// ���݂̏�Ԃ��擾����
	JUST_DOWN,	// �����ꂽ���ォ���擾����
	JUST_UP		// �����ꂽ���ォ���擾����
};

//-----------------------------------------------------------------------------------
// 
// �R���g���[���[�̃{�^��
// ����{�^���ɑ΂����͊��蓖�ĂȂ��ݒ������ꍇ
// BUTTON_NONE���e���̓f�o�C�X�̗񋓌^�ɃL���X�g����
// �n���Ȃ���΂Ȃ�Ȃ�
// 
enum ControllerButton
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
//-----------------------------------------------------------------------------------
//
// �L�[�{�[�h�̃{�^��
// 
enum KeyboardButton
{
	KB_BUTTON_A,
	KB_BUTTON_B,
	KB_BUTTON_C,
	KB_BUTTON_D,
	KB_BUTTON_E,
	KB_BUTTON_F,
	KB_BUTTON_G,
	KB_BUTTON_H,
	KB_BUTTON_I,
	KB_BUTTON_J,
	KB_BUTTON_K,
	KB_BUTTON_L,
	KB_BUTTON_M,
	KB_BUTTON_N,
	KB_BUTTON_O,
	KB_BUTTON_P,
	KB_BUTTON_Q,
	KB_BUTTON_R,
	KB_BUTTON_S,
	KB_BUTTON_T,
	KB_BUTTON_U,
	KB_BUTTON_V,
	KB_BUTTON_W,
	KB_BUTTON_X,
	KB_BUTTON_Y,
	KB_BUTTON_Z,
	KB_BUTTON_SHIFT,
	KB_BUTTON_CTRL,
	KB_BUTTON_ESCAPE,
	KB_BUTTON_DELETE,
	KB_BUTTON_LEFT,
	KB_BUTTON_RIGHT,
	KB_BUTTON_UP,
	KB_BUTTON_DOWN,
	KB_BUTTON_1,
	KB_BUTTON_2,
	KB_BUTTON_3,
	KB_BUTTON_4,
	KB_BUTTON_5,
	KB_BUTTON_6,
	KB_BUTTON_7,
	KB_BUTTON_8,
	KB_BUTTON_9,
	KB_BUTTON_0,
	KB_BUTTON_NUM
};
//-----------------------------------------------------------------------------------
//
// �}�E�X�̃{�^��
// 
enum MouseButton
{
	MB_BUTTON_0,
	MB_BUTTON_1,
	MB_BUTTON_2,
	MB_BUTTON_3,
	MB_BUTTON_NUM
};
//-----------------------------------------------------------------------------------
//
// �Q�[���p�b�h�̃{�^��
// 
enum GamepadButton
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
	: public yuu::RefferenceCount<IController>
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
	virtual bool getButtonState(ControllerButton ctrl, ButtonState get_type = PRESENT) = 0;
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
	virtual void setConfigKeyBoard(ControllerButton ctrl, KeyboardButton key) = 0;
	// �|�C���e�B���O�f�o�C�X�̓��͂��{�^���ɐݒ肷��
	// STATE_TYPE
	// �ݒ肳���{�^��
	// �ݒ肷��}�E�X�̃{�^��
	// �Ȃ�
	virtual void setConfigMouse(ControllerButton ctrl, MouseButton key) = 0;
	// �Q�[���p�b�h�̓��͂��{�^���ɐݒ肷��
	// STATE_TYPE
	// �ݒ肳���{�^��
	// �ݒ肷��Q�[���p�b�h�̂̃{�^��
	// �Ȃ�
	virtual void setConfigGamepad(ControllerButton ctrl, GamepadButton key) = 0;
};

//-----------------------------------------------------------------//
//�L�[�{�[�h���͏����C���^�t�F�[�X
//-----------------------------------------------------------------//
// �L�[�{�[�h�C���^�t�F�[�X
//
// ���z�I�ȃL�[�{�[�h�N���X
//
class IKeyBoardInput
	: public yuu::RefferenceCount<IKeyBoardInput>
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
	virtual bool getKeyState(KeyboardButton key, ButtonState get_type = PRESENT) = 0;
};

//-----------------------------------------------------------------//
//�Q�[���p�b�h���͏����C���^�t�F�[�X
//-----------------------------------------------------------------//
// �Q�[���p�b�h�C���^�t�F�[�X
//
// ���z�I�ȃQ�[���p�b�h�N���X
//
class IGamePadInput
	: public yuu::RefferenceCount<IGamePadInput>
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
	virtual bool getButtonState(GamepadButton button, ButtonState get_type = PRESENT) = 0;
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
	: public yuu::RefferenceCount<IPointingDeviceInput>
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
	virtual bool getButtonState(MouseButton button, ButtonState get_type = PRESENT) = 0;
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