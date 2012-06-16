

#include <d3dx9.h>

#include <graphic/graphics_device.h>
#include <graphic/buffer_object.h>

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{
//--------------------------------------------------------------------------------------------------
// 書き込み専用頂点バッファ
//--------------------------------------------------------------------------------------------------
struct WriteOnlyVertexBufferObject::Param{
	GraphicDevice device;
	boost::intrusive_ptr<IDirect3DVertexBuffer9> vertex_buff;
	size_t size;

	Param(GraphicDevice device_, size_t size_)
		: device(device_)
		, size(size_)
	{}
};
WriteOnlyVertexBufferObject::WriteOnlyVertexBufferObject(GraphicDevice device, size_t size, DWORD fvf)
	: param(new Param(device, size))
{
	// 頂点バッファの作成
	IDirect3DVertexBuffer9 *buff;
	IDirect3DDevice9* dev =(IDirect3DDevice9*)device->getHandle();
	if(FAILED(dev->CreateVertexBuffer(
				  size, D3DUSAGE_WRITEONLY, fvf, D3DPOOL_MANAGED, &buff, NULL)))
	{
		throw std::runtime_error("error");
	}
	param->vertex_buff = buff;
}

WriteOnlyVertexBufferObject::~WriteOnlyVertexBufferObject()
{
	delete param;
}

VertexBuffer WriteOnlyVertexBufferObject::create(GraphicDevice device, size_t size, DWORD fvf)
{
	return VertexBuffer(new WriteOnlyVertexBufferObject(device, size, fvf));
}

size_t WriteOnlyVertexBufferObject::write(const void *data, size_t size)
{
	void *dest;
	if(FAILED(param->vertex_buff->Lock(0, size, (void **)&dest, 0)))
		return false;

	memcpy(dest, data, size);
	param->vertex_buff->Unlock();

	return size;
}

size_t WriteOnlyVertexBufferObject::length() const
{
	return param->size;
}

void* WriteOnlyVertexBufferObject::getHandle()
{
	return param->vertex_buff.get();
}

const void* WriteOnlyVertexBufferObject::getHandle() const
{
	return param->vertex_buff.get();
}

//--------------------------------------------------------------------------------------------------
// 動的変更用頂点バッファ
//--------------------------------------------------------------------------------------------------
struct DynamicVertexBufferObject::Param{
	GraphicDevice device;
	boost::intrusive_ptr<IDirect3DVertexBuffer9> vertex_buff;
	size_t max_size;
	size_t current_pos;

	Param(GraphicDevice device_, size_t size_)
		: device(device_)
		, max_size(size_)
		, current_pos(0)
	{}
};
DynamicVertexBufferObject::DynamicVertexBufferObject(GraphicDevice device, size_t size, DWORD fvf)
	: param(new Param(device, size))
{
	// 頂点バッファの作成
	// Dynamicの場合D3DPOOL_DEFAULTでなければならないらしい
	// ただし、デバイスロスト時に確実に失われるので復旧が必須
	IDirect3DVertexBuffer9 *buff;
	IDirect3DDevice9* dev =(IDirect3DDevice9*)device->getHandle();
	if(FAILED(dev->CreateVertexBuffer(
				  size, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, fvf, D3DPOOL_DEFAULT, &buff, NULL)))
	{
		throw std::runtime_error("error");
	}
	param->vertex_buff = buff;
}

DynamicVertexBufferObject::~DynamicVertexBufferObject()
{
	delete param;
}

VertexBuffer DynamicVertexBufferObject::create(GraphicDevice device, size_t size, DWORD fvf)
{
	return VertexBuffer(new DynamicVertexBufferObject(device, size, fvf));
}

size_t DynamicVertexBufferObject::write(const void *data, size_t size)
{
	// 現在の末尾
	size_t position = param->current_pos;

	// 残りのスペースに書き込める限り「上書きなし」指定
	DWORD lock_flag = D3DLOCK_NOOVERWRITE;

	// 残りのバッファに収まるか判定
	if(param->max_size <= position + size)
	{
		lock_flag = D3DLOCK_DISCARD;
		position = 0;
	}

	// 書き込み
	void *dest;
	if(FAILED(param->vertex_buff->Lock(position, size, &dest, lock_flag)))
		return false;
	memcpy(dest, data, size);
	param->vertex_buff->Unlock();

	// 書き込んだ分だけずらす
	param->current_pos += size;

	return position;
}

size_t DynamicVertexBufferObject::length() const
{
	return param->max_size;
}

void* DynamicVertexBufferObject::getHandle()
{
	return param->vertex_buff.get();
}

const void* DynamicVertexBufferObject::getHandle() const
{
	return param->vertex_buff.get();
}

//--------------------------------------------------------------------------------------------------
// インデックスバッファ
//--------------------------------------------------------------------------------------------------
struct IndexBufferObject::Param{
	boost::intrusive_ptr<IDirect3DIndexBuffer9> index_buff;
	size_t size;

	Param(size_t size_)
		: size(size_)
	{}
};
IndexBufferObject::IndexBufferObject(GraphicDevice device, IndexFormat format, size_t size)
	: param(new Param(size))
{
	// インデックスバッファの作成
	IDirect3DIndexBuffer9 *buff;
	IDirect3DDevice9* dev =(IDirect3DDevice9*)device->getHandle();
	D3DFORMAT d3dformat;
	switch(format){
	case SIZE_16BIT: d3dformat =D3DFMT_INDEX16; break;
	case SIZE_32BIT: d3dformat =D3DFMT_INDEX32; break;
	}
	if(FAILED(dev->CreateIndexBuffer(
				  size , D3DUSAGE_WRITEONLY , d3dformat, D3DPOOL_MANAGED , &buff, NULL)))
	{
		throw std::runtime_error("error");
	}
	param->index_buff = buff;
}

IndexBufferObject::~IndexBufferObject()
{
	delete param;
}


yuu::graphic::IndexBuffer IndexBufferObject::create(GraphicDevice device, IndexFormat format, size_t size)
{
	return IndexBuffer(new IndexBufferObject(device, format, size));
}

size_t IndexBufferObject::write(const void *data, size_t size)
{
	void *dest;
	if(FAILED(param->index_buff->Lock(0, 0, (void **)&dest, 0)))
		return -1;

	memcpy(dest, data, size);
	param->index_buff->Unlock();

	return size;
}

size_t IndexBufferObject::length() const
{
	return param->size;
}

void* IndexBufferObject::getHandle()
{
	return param->index_buff.get();
}

const void* IndexBufferObject::getHandle() const
{
	return param->index_buff.get();
}

}
}