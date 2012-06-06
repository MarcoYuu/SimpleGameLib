#pragma once

// �����̃��C�u�����̖��O���
namespace yuu
{
//------------------------------------------------------------------
//�|�C���^�L���X�g
//------------------------------------------------------------------
// �C�ӂ̌^�̃|�C���^�ւ̃L���X�g
// �L���X�g����|�C���^
// �L���X�g���ꂽ�|�C���^
template <class T>
inline T *pointer_cast(void *p)
{
	return static_cast<T *>(p);
}

//------------------------------------------------------------------
//�����폜�p�p�����N���X
//------------------------------------------------------------------
// �Q�ƃJ�E���g�C���^�t�F�[�X
//
// �Q�ƃJ�E���g�����炪���N���X�ւ̃C���^�t�F�[�X
// �p�����邱�ƂŎ����폜�̃|�C���^���g�p�ł���
//
template<class Derived>
class IRefferenceCount
{
private:
	std::size_t m_ref_count;

public:
	// �R���X�g���N�^
	IRefferenceCount()
		: m_ref_count(0)
	{}

	// ���݂̎Q�Ɛ��̎擾
	// �L���X�g���ꂽ�|�C���^
	inline std::size_t getRef() const
	{
		return m_ref_count;
	}

	// intrusive_ptr�p �Q�Ƃ����������Ă΂�鏈��
	friend void intrusive_ptr_add_ref(Derived *p)
	{
		p->m_ref_count++;
	}

	// intrusive_ptr�p �Q�Ƃ����������Ă΂�鏈�� �Q�Ƃ�0�ɂȂ��delete���Ă΂��B
	friend void intrusive_ptr_release(Derived *p)
	{
		p->m_ref_count--;
		if(p->m_ref_count <= 0)
			delete p;
	}
};
}