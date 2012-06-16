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
	typedef unsigned int byte4;
	
	Color();
	Color(byte4 color);
	Color(byte r, byte g, byte b);
	Color(byte r, byte g, byte b, byte a);
	Color(float r, float g, float b);
	Color(float r, float g, float b, float a);

	void set(byte4 color);
	void set(byte r, byte g, byte b);
	void set(byte r, byte g, byte b, byte a);
	void set(float r, float g, float b);
	void set(float r, float g, float b, float a);

	void setAlpha(byte val);
	void setAlpha(float val);
	void setRed(byte val);
	void setRed(float val);
	void setGreen(byte val);
	void setGreen(float val);
	void setBlue(byte val);
	void setBlue(float val);

	byte getAlpha() const;
	byte getRed() const;
	byte getGreen() const;
	byte getBlue() const;
	
	byte4 get() const {return color;};
	operator byte4() const {return color;}
	Color& operator=(const Color &rhs){color =rhs.color;} 
	Color& operator=(const byte4 &rhs){color =rhs;}

private:
	byte4 color;//ARGB

	static byte4 format(byte r, byte g, byte b, byte a);
	static byte4 format(float r, float g, float b, float a);
};
}
}