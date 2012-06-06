#pragma once

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{
//--------------------------------------------------------------------------------------------------
// 色
//--------------------------------------------------------------------------------------------------
class Color
{
public:
	static const Color White;
	static const Color Black;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Cyan;
	static const Color Magenta;
	static const Color Yellow;
	static const Color Orange;
	static const Color Gray;
	static const Color Brown;
	static const Color Violet;

public:
	typedef unsigned char byte;
	typedef unsigned long byte4;
	
	Color();
	Color(byte4 _color);
	Color(byte r, byte g, byte b, byte a);
	Color(float r, float g, float b, float a);

	void set(byte4 _color);
	void set(byte r, byte g, byte b, byte a);
	void set(float r, float g, float b, float a);
	
	byte4 get() const{return color;};
	operator byte4() const {return color;}

private:
	byte4 color;//ARGB

	static byte4 format(byte r, byte g, byte b, byte a);
	static byte4 format(float r, float g, float b, float a);
};
}
}