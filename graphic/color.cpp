
#include <graphic/color.h>

// 自分のライブラリの名前空間
namespace yuu
{
// グラフィック描画関連
namespace graphic
{

const Color Color::White   =0xffffffff;
const Color Color::Black   =0xff000000;
const Color Color::Red     =0xffff0000;
const Color Color::Green   =0xff00ff00;
const Color Color::Blue    =0xff0000ff;
const Color Color::Cyan    =0xff00ffff;
const Color Color::Magenta =0xffff00ff;
const Color Color::Yellow  =0xffffff00;
const Color Color::Orange((byte)255,(byte)165,(byte)0,(byte)255);
const Color Color::Gray((byte)128,(byte)128,(byte)128,(byte)255);
const Color Color::Brown((byte)165,(byte)42,(byte)42,(byte)255);
const Color Color::Violet((byte)238,(byte)130,(byte)238,(byte)255);

Color::Color()
	: color(0x00000000)
{
}

Color::Color(byte4 _color) 
	: color(_color)
{
}

Color::Color( byte r, byte g, byte b )
	: color(format(r, g, b, 0xff))
{

}

Color::Color(byte r, byte g, byte b, byte a) 
	: color(format(r, g, b, a))
{
}

Color::Color( float r, float g, float b )
	: color(format(r, g, b, 1))
{

}

Color::Color(float r, float g, float b, float a) 
	: color(format(r, g, b, a))
{
}

void Color::set(byte4 _color)
{
	color =_color;
}

void Color::set( byte r, byte g, byte b )
{
	color = format(r, g, b, 0xff);
}

void Color::set(byte r, byte g, byte b, byte a)
{
	color = format(r, g, b, a);
}

void Color::set( float r, float g, float b )
{
	color = format(r, g, b, 1);
}

void Color::set(float r, float g, float b, float a)
{
	color = format(r, g, b, a);
}

void Color::setAlpha( byte val )
{
	color =((byte4)(val & 0xff) << 24) | (0x00ffffff & color);
}

void Color::setAlpha( float val )
{
	setAlpha((byte)(0xff*val));
}

void Color::setRed( byte val )
{
	color =((byte4)(val & 0xff) << 16) | (0xff00ffff & color);
}

void Color::setRed( float val )
{
	setRed((byte)(0xff*val));
}

void Color::setGreen( byte val )
{
	color =((byte4)(val & 0xff) << 8) | (0xffff00ff & color);
}

void Color::setGreen( float val )
{
	setGreen((byte)(0xff*val));
}

void Color::setBlue( byte val )
{
	color =((byte4)(val & 0xff)) | (0xffffff00 & color);
}

void Color::setBlue( float val )
{
	setBlue((byte)(0xff*val));
}

Color::byte Color::getAlpha() const
{
	return (byte)((color >> 24)&0xff);
}

Color::byte Color::getRed() const
{
	return (byte)((color >> 16)&0xff);
}

Color::byte Color::getGreen() const
{
	return (byte)((color >> 8)&0xff);
}

Color::byte Color::getBlue() const
{
	return (byte)(color&0xff);
}

Color::byte4 Color::format(byte r, byte g, byte b, byte a)
{
	return ((byte4)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff)));
}

Color::byte4 Color::format(float r, float g, float b, float a)
{
	return format((byte)(0xff * r), (byte)(0xff * g), (byte)(0xff * b), (byte)(0xff * a));
}

}
}