#pragma once

#include <boost/utility.hpp>

#include <other/refference_count.h>
#include <graphic/forward_declaration.h>

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{
//--------------------------------------------------------------------------------------------------
// バッファ
//--------------------------------------------------------------------------------------------------
class BufferObject
	: public IRefferenceCount<BufferObject>
	, boost::noncopyable
{
public:
	virtual ~BufferObject(){}

	// 書き込み
	virtual size_t write(const void *data, size_t size) = 0;
	// バッファサイズ
	virtual size_t length() const = 0;
	// ハンドルの取得
	virtual void* getHandle() = 0;
	virtual const void* getHandle() const = 0;
};

// 頂点バッファ
class VertexBufferObject: public BufferObject{
public:
	virtual ~VertexBufferObject(){}
};

//--------------------------------------------------------------------------------------------------
// 書き込み専用頂点バッファ
//--------------------------------------------------------------------------------------------------
class WriteOnlyVertexBufferObject : public VertexBufferObject
{
public:
	static VertexBuffer create(GraphicDevice device, size_t size, DWORD fvf);
	~WriteOnlyVertexBufferObject();

	// 戻り値は書き込めたサイズ、書き込めなかった場合負数
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
// 動的変更用頂点バッファ
//--------------------------------------------------------------------------------------------------
class DynamicVertexBufferObject: public VertexBufferObject
{
public:
	static VertexBuffer create(GraphicDevice device, size_t size, DWORD fvf);
	~DynamicVertexBufferObject();

	// 戻り値は書き込み時の変更があった場所までのオフセット
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
// インデックスバッファ
//--------------------------------------------------------------------------------------------------
class IndexBufferObject: public BufferObject
{
public:
	enum IndexFormat{ SIZE_16BIT, SIZE_32BIT };
	static IndexBuffer create(GraphicDevice device, IndexFormat format, size_t size);
	~IndexBufferObject();

	// 戻り値は書き込めたサイズ、書き込めなかった場合負数
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