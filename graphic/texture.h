
#pragma once

#include <boost/utility.hpp>

#include <other/refference_count.h>
#include <other/utility.h>
#include <graphic/forward_declaration.h>

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{
//--------------------------------------------------------------------------------------------------
// テクスチャ
//--------------------------------------------------------------------------------------------------
class TextureManager
	: public IRefferenceCount<TextureManager>
	, boost::noncopyable
{
public:
	static Texture create(GraphicDevice device, const tstring &filename);
	virtual ~TextureManager(){}

	Size getSize();

	void* getHandle();
	const void* getHandle() const;

protected:
	TextureManager(){};
	TextureManager(GraphicDevice device, const tstring &filename);

	struct Param;
	Param *param;
};
}
}