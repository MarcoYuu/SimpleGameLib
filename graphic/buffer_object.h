#pragma once

#include <boost/utility.hpp>

#include <other/refference_count.h>
#include <graphic/forward_declaration.h>

// �����̃��C�u�����̖��O���
namespace yuu
{
// �O���t�B�b�N�`��֘A
namespace graphic
{
//--------------------------------------------------------------------------------------------------
// �o�b�t�@
//--------------------------------------------------------------------------------------------------
class BufferObject
	: public IRefferenceCount<BufferObject>
	, boost::noncopyable
{
public:
	virtual ~BufferObject(){}

	// ��������
	virtual size_t write(const void *data, size_t size) = 0;
	// �o�b�t�@�T�C�Y
	virtual size_t length() const = 0;
	// �n���h���̎擾
	virtual void* getHandle() = 0;
	virtual const void* getHandle() const = 0;
};

// ���_�o�b�t�@
class VertexBufferObject: public BufferObject{
public:
	virtual ~VertexBufferObject(){}
};

//--------------------------------------------------------------------------------------------------
// �������ݐ�p���_�o�b�t�@
//--------------------------------------------------------------------------------------------------
class WriteOnlyVertexBufferObject : public VertexBufferObject
{
public:
	static VertexBuffer create(GraphicDevice device, size_t size, DWORD fvf);
	~WriteOnlyVertexBufferObject();

	// �߂�l�͏������߂��T�C�Y�A�������߂Ȃ������ꍇ����
	size_t write(const void *data, size_t size);

	size_t length() const;
	void* getHandle();
	const void* getHandle() const;

private:
	struct Param;
	Param *param;

	WriteOnlyVertexBufferObject(GraphicDevice device, size_t size, DWORD fvf);
};

//--------------------------------------------------------------------------------------------------
// ���I�ύX�p���_�o�b�t�@
//--------------------------------------------------------------------------------------------------
class DynamicVertexBufferObject: public VertexBufferObject
{
public:
	static VertexBuffer create(GraphicDevice device, size_t size, DWORD fvf);
	~DynamicVertexBufferObject();

	// �߂�l�͏������ݎ��̕ύX���������ꏊ�܂ł̃I�t�Z�b�g
	size_t write(const void *data, size_t size);
	
	size_t length() const;
	void* getHandle();
	const void* getHandle() const;

private:
	struct Param;
	Param *param;

	DynamicVertexBufferObject(GraphicDevice device, size_t size, DWORD fvf);
};

//--------------------------------------------------------------------------------------------------
// �C���f�b�N�X�o�b�t�@
//--------------------------------------------------------------------------------------------------
class IndexBufferObject: public BufferObject
{
public:
	enum IndexFormat{ SIZE_16BIT, SIZE_32BIT };
	static IndexBuffer create(GraphicDevice device, IndexFormat format, size_t size);
	~IndexBufferObject();

	// �߂�l�͏������߂��T�C�Y�A�������߂Ȃ������ꍇ����
	size_t write(const void *data, size_t size);

	size_t length() const;
	void* getHandle();
	const void* getHandle() const;

private:
	struct Param;
	Param *param;

	IndexBufferObject(GraphicDevice device, IndexFormat format, size_t size);
};
}
}