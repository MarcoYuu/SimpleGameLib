#include "color.h"

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

Color::Color(byte4 _color) : color(_color)
{
}

Color::Color(float r, float g, float b, float a) : color(format(r, g, b, a))
{
}

Color::Color(byte r, byte g, byte b, byte a) : color(format(r, g, b, a))
{
}

void Color::set(byte4 _color)
{
	color =_color;
}

void Color::set(float r, float g, float b, float a)
{
	color = format(r, g, b, a);
}

void Color::set(byte r, byte g, byte b, byte a)
{
	color = format(r, g, b, a);
}

Color::byte4 Color::format(byte r, byte g, byte b, byte a)
{
	return ((byte4)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff)));
}

Color::byte4 Color::format(float r, float g, float b, float a)
{
	return format((byte)(255 * r), (byte)(255 * g), (byte)(255 * b), (byte)(255 * a));
}
}
}