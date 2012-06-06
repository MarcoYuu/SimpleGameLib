#pragma once

#include <d3d9.h>

#include <boost/intrusive_ptr.hpp>
#include <boost/utility.hpp>

#include "../other/refference_count.h"
#include "graphics_device.h"

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{
//--------------------------------------------------------------------------------------------------
// 頂点バッファ
//--------------------------------------------------------------------------------------------------
class VertexBufferObject;
typedef boost::intrusive_ptr<VertexBufferObject> VertexBuffer;

class VertexBufferObject
	: public IRefferenceCount<VertexBufferObject>
	, boost::noncopyable
{
public:
	virtual size_t write(void *data, size_t size) = 0;
	virtual size_t length() const = 0;
	virtual IDirect3DVertexBuffer9 *get() const = 0;
};

//--------------------------------------------------------------------------------------------------
// 書き込み専用頂点バッファ
//--------------------------------------------------------------------------------------------------
class WriteOnlyVertexBufferObject;
typedef boost::intrusive_ptr<WriteOnlyVertexBufferObject> WriteOnlyVertexBuffer;

class WriteOnlyVertexBufferObject
	: VertexBufferObject
{
public:
	static VertexBuffer create(GraphicDevice device, size_t size, DWORD fvf);
	~WriteOnlyVertexBufferObject() {}

	// 書き込み
	// 戻り値は書き込み時の変更があった場所までの
	size_t write(void *data, size_t size);
	// バッファサイズ
	size_t length() const;
	// ハンドルの取得
	IDirect3DVertexBuffer9 *get() const ;

private:
	GraphicDevice m_device;
	boost::intrusive_ptr<IDirect3DVertexBuffer9> m_vertex_buff;
	size_t m_size;

	WriteOnlyVertexBufferObject(GraphicDevice device, size_t size, DWORD fvf);
};

//--------------------------------------------------------------------------------------------------
// 動的変更用頂点バッファ
//--------------------------------------------------------------------------------------------------
class DynamicVertexBufferObject;
typedef boost::intrusive_ptr<DynamicVertexBufferObject> DynamicVertexBuffer;

class DynamicVertexBufferObject
	: VertexBufferObject
{
public:
	static VertexBuffer create(GraphicDevice device, size_t size, DWORD fvf);
	~DynamicVertexBufferObject() {}

	size_t write(void *data, size_t size);
	size_t length() const;
	IDirect3DVertexBuffer9 *get() const;

private:
	GraphicDevice m_device;
	boost::intrusive_ptr<IDirect3DVertexBuffer9> m_vertex_buff;
	size_t m_max_size;
	size_t m_current_pos;

	DynamicVertexBufferObject(GraphicDevice device, size_t size, DWORD fvf);
};

//--------------------------------------------------------------------------------------------------
// インデックスバッファ
//--------------------------------------------------------------------------------------------------
class IndexBufferObject;
typedef boost::intrusive_ptr<IndexBufferObject> IndexBuffer;

class IndexBufferObject
	: public IRefferenceCount<IndexBufferObject>
	, boost::noncopyable
{
public:
	enum IndexFormat
	{
		SIZE_16BIT = D3DFMT_INDEX16,
		SIZE_32BIT = D3DFMT_INDEX32
	};
	static IndexBuffer create(GraphicDevice device, IndexFormat format, size_t size);
	~IndexBufferObject() {}

	bool write(void *data, size_t size);
	size_t length() const;
	IDirect3DIndexBuffer9 *get() const;

private:
	boost::intrusive_ptr<IDirect3DIndexBuffer9> m_index_buff;
	size_t m_size;

	IndexBufferObject(GraphicDevice device, IndexFormat format, size_t size);
};
}
}