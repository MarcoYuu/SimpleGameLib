// InputAsDirectInput.h
// DirectInput��p�������̓N���X�w�b�_
// �ȈՐ���
// 			DirectInput��p�������̓N���X�̃w�b�_
//
// 0.1
#pragma once

#include <app/windows_window_base.h>
#include <input/input_interface.h>

// �����̃��C�u�����̖��O���
namespace yuu
{
// ���͊֘A
namespace input
{
//--------------------------------------------------------------------------------------
//DirectInput�ɂ��R���g���[���[
//--------------------------------------------------------------------------------------

// �������擾�ł��Ȃ������Ƃ���NULL���Ԃ�
Controller CreateDirectInputController(app::Window window);

}
}