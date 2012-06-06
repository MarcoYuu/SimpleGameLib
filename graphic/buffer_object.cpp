#include <d3dx9.h>
#include "buffer_object.h"

// �����̃��C�u�����̖��O���
namespace yuu
{
// �O���t�B�b�N�`��֘A
namespace graphic
{
//--------------------------------------------------------------------------------------------------
// �������ݐ�p���_�o�b�t�@
//--------------------------------------------------------------------------------------------------
WriteOnlyVertexBufferObject::WriteOnlyVertexBufferObject(GraphicDevice device, size_t size, DWORD fvf)
	: m_device(device)
	, m_size(size)
{
	// ���_�o�b�t�@�̍쐬
	IDirect3DVertexBuffer9 *buff;
	if(FAILED(device->device()->CreateVertexBuffer(
				  size, D3DUSAGE_WRITEONLY, fvf, D3DPOOL_MANAGED, &buff, NULL)))
	{
		throw std::runtime_error("error");
	}
	m_vertex_buff = buff;
}

yuu::graphic::VertexBuffer WriteOnlyVertexBufferObject::create(GraphicDevice device, size_t size, DWORD fvf)
{
	return VertexBuffer(new WriteOnlyVertexBufferObject(device, size, fvf));
}

size_t WriteOnlyVertexBufferObject::write(void *data, size_t size)
{
	void *dest;
	if(FAILED(m_vertex_buff->Lock(0, size, (void **)&dest, 0)))
		return false;

	memcpy(dest, data, size);
	m_vertex_buff->Unlock();

	return 0;
}

size_t WriteOnlyVertexBufferObject::length() const
{
	return m_size;
}

IDirect3DVertexBuffer9 *WriteOnlyVertexBufferObject::get() const
{
	return m_vertex_buff.get();
}

//--------------------------------------------------------------------------------------------------
// ���I�ύX�p���_�o�b�t�@
//--------------------------------------------------------------------------------------------------
DynamicVertexBufferObject::DynamicVertexBufferObject(GraphicDevice device, size_t size, DWORD fvf)
	: m_device(device)
	, m_max_size(size)
	, m_current_pos(0)
{
	// ���_�o�b�t�@�̍쐬
	// Dynamic�̏ꍇD3DPOOL_DEFAULT�łȂ���΂Ȃ�Ȃ��炵��
	// �������A�f�o�C�X���X�g���Ɋm���Ɏ�����̂ŕ������K�{
	IDirect3DVertexBuffer9 *buff;
	if(FAILED(device->device()->CreateVertexBuffer(
				  size, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, fvf, D3DPOOL_DEFAULT, &buff, NULL)))
	{
		throw std::runtime_error("error");
	}
	m_vertex_buff = buff;
}

yuu::graphic::VertexBuffer DynamicVertexBufferObject::create(GraphicDevice device, size_t size, DWORD fvf)
{
	return VertexBuffer(new DynamicVertexBufferObject(device, size, fvf));
}

size_t DynamicVertexBufferObject::write(void *data, size_t size)
{
	// ���݂̖���
	size_t position = m_current_pos;

	// �c��̃X�y�[�X�ɏ������߂����u�㏑���Ȃ��v�w��
	DWORD lock_flag = D3DLOCK_NOOVERWRITE;

	// �c��̃o�b�t�@�Ɏ��܂邩����
	if(m_max_size <= position + size)
	{
		lock_flag = D3DLOCK_DISCARD;
		position = 0;
	}

	// ��������
	void *dest;
	if(FAILED(m_vertex_buff->Lock(position, size, &dest, lock_flag)))
		return false;
	memcpy(dest, data, size);
	m_vertex_buff->Unlock();

	// �������񂾕��������炷
	m_current_pos += size;

	return position;
}

size_t DynamicVertexBufferObject::length() const
{
	return m_max_size;
}

IDirect3DVertexBuffer9 *DynamicVertexBufferObject::get() const
{
	return m_vertex_buff.get();
}

//--------------------------------------------------------------------------------------------------
// �C���f�b�N�X�o�b�t�@
//--------------------------------------------------------------------------------------------------
IndexBufferObject::IndexBufferObject(GraphicDevice device, IndexFormat format, size_t size)
	: m_size(size)
{
	// �C���f�b�N�X�o�b�t�@�̍쐬
	IDirect3DIndexBuffer9 *buff;
	if(FAILED(device->device()->CreateIndexBuffer(
				  size , D3DUSAGE_WRITEONLY , (D3DFORMAT)format , D3DPOOL_MANAGED , &buff, NULL)))
	{
		throw std::runtime_error("error");
	}
	m_index_buff = buff;
}

yuu::graphic::IndexBuffer IndexBufferObject::create(GraphicDevice device, IndexFormat format, size_t size)
{
	return IndexBuffer(new IndexBufferObject(device, format, size));
}

bool IndexBufferObject::write(void *data, size_t size)
{
	void *dest;
	if(FAILED(m_index_buff->Lock(0, 0, (void **)&dest, 0)))
		return false;

	memcpy(dest, data, size);
	m_index_buff->Unlock();

	return true;
}

size_t IndexBufferObject::length() const
{
	return m_size;
}

IDirect3DIndexBuffer9 *IndexBufferObject::get() const
{
	return m_index_buff.get();
}
}
}