#include <d3dx9.h>
#include "buffer_object.h"

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{
//--------------------------------------------------------------------------------------------------
// 書き込み専用頂点バッファ
//--------------------------------------------------------------------------------------------------
WriteOnlyVertexBufferObject::WriteOnlyVertexBufferObject(GraphicDevice device, size_t size, DWORD fvf)
	: m_device(device)
	, m_size(size)
{
	// 頂点バッファの作成
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
// 動的変更用頂点バッファ
//--------------------------------------------------------------------------------------------------
DynamicVertexBufferObject::DynamicVertexBufferObject(GraphicDevice device, size_t size, DWORD fvf)
	: m_device(device)
	, m_max_size(size)
	, m_current_pos(0)
{
	// 頂点バッファの作成
	// Dynamicの場合D3DPOOL_DEFAULTでなければならないらしい
	// ただし、デバイスロスト時に確実に失われるので復旧が必須
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
	// 現在の末尾
	size_t position = m_current_pos;

	// 残りのスペースに書き込める限り「上書きなし」指定
	DWORD lock_flag = D3DLOCK_NOOVERWRITE;

	// 残りのバッファに収まるか判定
	if(m_max_size <= position + size)
	{
		lock_flag = D3DLOCK_DISCARD;
		position = 0;
	}

	// 書き込み
	void *dest;
	if(FAILED(m_vertex_buff->Lock(position, size, &dest, lock_flag)))
		return false;
	memcpy(dest, data, size);
	m_vertex_buff->Unlock();

	// 書き込んだ分だけずらす
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
// インデックスバッファ
//--------------------------------------------------------------------------------------------------
IndexBufferObject::IndexBufferObject(GraphicDevice device, IndexFormat format, size_t size)
	: m_size(size)
{
	// インデックスバッファの作成
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