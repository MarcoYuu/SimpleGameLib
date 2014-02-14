
#pragma once

#include <boost/intrusive_ptr.hpp>
#include "../other/geometry.h"

namespace yuu
{
	namespace graphic
	{

		class GraphicDeviceManager;
		class IBufferObject;
		class VertexBufferObject;
		class WriteOnlyVertexBufferObject;
		class DynamicVertexBufferObject;
		class IndexBufferObject;
		class Color;
		class SpriteBatchSystem;
		class TextureManager;
		class WritableTextureManager;
		class RenderTargetManager;

		typedef boost::intrusive_ptr<GraphicDeviceManager>			GraphicDevice;
		typedef boost::intrusive_ptr<IBufferObject>					Buffer;
		typedef boost::intrusive_ptr<VertexBufferObject>			VertexBuffer;
		typedef boost::intrusive_ptr<WriteOnlyVertexBufferObject>	WriteOnlyVertexBuffer;
		typedef boost::intrusive_ptr<DynamicVertexBufferObject>		DynamicVertexBuffer;
		typedef boost::intrusive_ptr<IndexBufferObject>				IndexBuffer;
		typedef boost::intrusive_ptr<SpriteBatchSystem>				SpriteBatch;
		typedef boost::intrusive_ptr<TextureManager>				Texture;
		typedef boost::intrusive_ptr<WritableTextureManager>		WritableTexture;
		typedef boost::intrusive_ptr<RenderTargetManager>			RenderTerget;

		typedef Vector2f		SizeFloat;
		typedef Vector2<size_t>	Size;
		typedef Vector2<int>	Point;

	}
}