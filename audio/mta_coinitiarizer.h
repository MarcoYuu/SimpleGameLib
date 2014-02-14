// MTA_CoInitiarizer.h
// COM�������N���X�w�b�_
// �ȈՐ���
//
// 			Init()���Ăяo����MTA�ɏ��������A�v���O�����I������COM���I��������B
// 			�}���`�X���b�h�͈��S�ł͂Ȃ���������Ȃ�
//
// 0.2
#pragma once

#if defined(_WIN32) | defined(_WIN64)
#include <ObjBase.h>
#include <cstdlib>

// �����̃��C�u�����̖��O���
namespace yuu
{
	// COM�������N���X
	//
	// Init()���Ăяo����MTA�ɏ��������A�v���O�����I������COM���I��������B
	class MTA_CoInitiarizer
	{
		friend int ::atexit(void ( *)(void));

	public:
		// COM�̏�����
		// �Ȃ�
		static void Init()
		{
			static bool initialized = false;

			//����������Ă��Ȃ�������COM��������
			if(!initialized)
			{
				initialized = true;
				CoInitializeEx(0, COINIT_MULTITHREADED);

				//�v���O�����I�����ɏI������
				std::atexit(MTA_CoInitiarizer::UnInit);
			}
		}
	private:
		static void UnInit()
		{
			//�I������
			CoUninitialize();
		}
	};
}
#endif
