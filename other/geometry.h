#pragma once

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef TO_RADIANS
#define TO_RADIANS(deg) (M_PI*deg/180.0)
#endif

#ifndef TO_DEGREES
#define TO_DEGREES(rad) (180.0*rad/M_PI)
#endif

namespace yuu{

template<typename T> class Vector2;
template<typename T> class Vector3;
template<typename T> class Vector4;
template<typename T> class Quaternion;
template<typename T> class Matrix4x4;

typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

typedef Vector2<float> Point2f;
typedef Vector2<double> Point2d;
typedef Vector3<float> Point3f;
typedef Vector3<double> Point3d;

typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;
typedef Quaternion<float> Quaternionf;
typedef Quaternion<double> Quaterniond;
typedef Matrix4x4<float> Matrix4x4f;
typedef Matrix4x4<double> Matrix4x4d;

//-----------------------------------------------------------------------------------------------
//二次元ベクトル
//-----------------------------------------------------------------------------------------------
template<typename T>
class Vector2
{
public:
	T x, y;

public:
	//---------------------------
	// Constructors
	//---------------------------
	Vector2()
		: x(0)
		, y(0) 
	{}
	Vector2(T _x, T _y)
		: x(_x)
		, y(_y) 
	{}
	Vector2(const T *ary)
		: x(ary[0])
		, y(ary[1]) 
	{}

	Vector2(const Vector2 &rhs)
		: x(rhs.x)
		, y(rhs.y) 
	{}
	template<typename U>
	Vector2(const Vector2<U> &rhs)
		: x(rhs.x)
		, y(rhs.y)
	{}

	Vector2 &operator=(const Vector2 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	template<typename U>
	Vector2 &operator=(const Vector2<U> &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	//---------------------------
	// set value
	//---------------------------
	void set(T x, T y)
	{
		this->x = x;
		this->y = y;
	}
	void set(const T *ary)
	{
		x = ary[0];
		y = ary[1];
	}

	//---------------------------
	// to array
	//---------------------------
	T *toAry()
	{
		return &x;
	}
	const T *toAry() const
	{
		return &x;
	}

	//---------------------------
	// utilities
	//---------------------------
	T sumsq() const
	{
		return x * x + y * y;
	}
	T abs() const
	{
		return sqrt(sumsq());
	}
	T dot(const Vector2 &rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}
	T closs(const Vector2 &rhs) const
	{
		return x * rhs.x - y * rhs.y;
	}
	template<typename U>
	Vector2<U> type_cast() const{
		return Vector2<U>(static_cast<U>(x), static_cast<U>(y));
	}

	//---------------------------
	// operators
	//---------------------------
	bool operator==(const Vector2 &rhs) const
	{
		return (x == rhs.x && y == rhs.y);
	}
	bool operator!=(const Vector2 &rhs) const
	{
		return !((*this) == rhs);
	}

	Vector2 operator+() const
	{
		return *this;
	}
	Vector2 operator-() const
	{
		return Vector2(-x, -y);
	}

	const Vector2& operator+=(const Vector2 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	const Vector2& operator-=(const Vector2 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	const Vector2& operator*=(const T &rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	const Vector2& operator/=(const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}

	friend Vector2<T> operator+(const Vector2<T> &lhs, const Vector2<T> &rhs)
	{
		return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
	}
	friend Vector2<T> operator-(const Vector2<T> &lhs, const Vector2<T> &rhs)
	{
		return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
	}
	friend Vector2<T> operator*(const T &lhs, const Vector2<T> &rhs)
	{
		return Vector2<T>(rhs.x * lhs, rhs.y * lhs);
	}
	friend Vector2<T> operator*(const Vector2<T> &lhs, const T &rhs)
	{
		return Vector2<T>(lhs.x * rhs, lhs.y * rhs);
	}
	friend Vector2<T> operator/(const Vector2<T> &lhs, const T &rhs)
	{
		return Vector2<T>(lhs.x / rhs, lhs.y / rhs);
	}
};

//-----------------------------------------------------------------------------------------------
//3次元ベクトル
//-----------------------------------------------------------------------------------------------
template<typename T>
class Vector3
{
public:
	T x, y, z;

public:
	//---------------------------
	// Constructors
	//---------------------------
	Vector3()
		: x(0)
		, y(0)
		, z(0) 
	{}
	Vector3(T _x, T _y, T _z)
		: x(_x)
		, y(_y)
		, z(_z) 
	{}
	Vector3(const T *ary)
		: x(ary[0])
		, y(ary[1])
		, z(ary[2]) 
	{}

	Vector3(const Vector3 &rhs)
		: x(rhs.x)
		, y(rhs.y)
		, z(rhs.z) 
	{}
	template<typename U>
	Vector3(const Vector3<U> &rhs)
		: x(rhs.x)
		, y(rhs.y)
		, z(rhs.z) 
	{}

	Vector3 &operator=(const Vector3 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	template<typename U>
	Vector3 &operator=(const Vector3<U> &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	//---------------------------
	// set value
	//---------------------------
	void set(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void set(const T *ary)
	{
		x = ary[0];
		y = ary[1];
		z = ary[2];
	}

	//---------------------------
	// to array
	//---------------------------
	T *toAry()
	{
		return &x;
	}
	const T *toAry() const
	{
		return &x;
	}

	//---------------------------
	// utilities
	//---------------------------
	T sumsq() const
	{
		return x * x + y * y + z * z;
	}
	T abs() const
	{
		return sqrt(sumsq());
	}
	T dot(const Vector3 &rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}
	Vector3 closs(const Vector3 &rhs) const
	{
		return Vector3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
	}
	template<typename U>
	Vector3<U> type_cast() const{
		return Vector3<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z));
	}

	//---------------------------
	// operators
	//---------------------------
	bool operator==(const Vector3 &rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}
	bool operator!=(const Vector3 &rhs) const
	{
		return !((*this) == rhs);
	}

	Vector3 operator+()
	{
		return *this;
	}
	Vector3 operator-()
	{
		return Vector3(-x, -y, -z);
	}

	const Vector3& operator+=(const Vector3 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	const Vector3& operator-=(const Vector3 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	const Vector3& operator*=(const T &rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	const Vector3& operator/=(const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	friend Vector3<T> operator+(const Vector3<T> &lhs, const Vector3<T> &rhs)
	{
		return Vector3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}
	friend Vector3<T> operator-(const Vector3<T> &lhs, const Vector3<T> &rhs)
	{
		return Vector3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}
	friend Vector3<T> operator*(const T &lhs, const Vector3<T> &rhs)
	{
		return Vector3<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
	}
	friend Vector3<T> operator*(const Vector3<T> &lhs, const T &rhs)
	{
		return Vector3<T>(rhs * lhs.x, rhs * lhs.y, rhs * lhs.z);
	}
	friend Vector3<T> operator/(const Vector3<T> &lhs, const T &rhs)
	{
		return Vector3<T>(lhs.x / rhs, lhs.y / rhs, lhs.y / rhs);
	}
};


//-----------------------------------------------------------------------------------------------
//4次元ベクトル
//-----------------------------------------------------------------------------------------------
template<typename T>
class Vector4
{
public:
	T x, y, z, w;

public:
	//---------------------------
	// Constructors
	//---------------------------
	Vector4()
		: x(0)
		, y(0)
		, z(0)
		, w(0) 
	{}
	Vector4(T x, T y, T z, T w)
		: x(x)
		, y(y)
		, z(z)
		, w(w) 
	{}
	Vector4(const T *ary)
		: x(ary[0])
		, y(ary[1])
		, z(ary[2]) 
		, w(ary[3])
	{}

	Vector4(const Vector3<T> &rhs)
		: x(rhs.x)
		, y(rhs.y)
		, z(rhs.z)
		, w(1.0) 
	{}
	template<typename U>
	Vector4(const Vector3<U> &rhs)
		: x(rhs.x)
		, y(rhs.y)
		, z(rhs.z)
		, w(1.0) 
	{}

	Vector4(const Vector4 &rhs)
		: x(rhs.x)
		, y(rhs.y)
		, z(rhs.z)
		, w(rhs.w) 
	{}
	template<typename U>
	Vector4(const Vector4<U> &rhs)
		: x(rhs.x)
		, y(rhs.y)
		, z(rhs.z)
		, w(rhs.w) 
	{}

	Vector4 &operator=(const Vector3<T> &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = 1.0;
		return *this;
	}
	template<typename U>
	Vector4 &operator=(const Vector3<U> &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = 1.0;
		return *this;
	}

	Vector4 &operator=(const Vector4 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}
	template<typename U>
	Vector4 &operator=(const Vector4<U> &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	//---------------------------
	// set value
	//---------------------------
	void set(T x, T y, T z, T w)
	{
		x = x;
		y = y;
		z = z;
		w = w;
	}
	void set(const T *ary)
	{
		x = ary[0];
		y = ary[1];
		z = ary[2];
		w = ary[3];
	}

	//---------------------------
	// to array
	//---------------------------
	T *toAry()
	{
		return &x;
	}
	const T *toAry() const
	{
		return &x;
	}

	//---------------------------
	// utilities
	//---------------------------
	T sumsq() const
	{
		return x * x + y * y + z * z + w * w;
	}
	T sumsq3D() const
	{
		return x * x + y * y + z * z;
	}
	T abs() const
	{
		return sqrt(sumsq());
	}
	T abs3D() const
	{
		return sqrt(sumsq3D());
	}
	T dot(const Vector4 &rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	}
	T dot3D(const Vector4 &rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}
	Vector3<T> closs3D(const Vector4 &rhs) const
	{
		return Vector3<T>(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
	}
	template<typename U>
	Vector4<U> type_cast() const{
		return Vector4<U>(
			static_cast<U>(x), static_cast<U>(y), 
			static_cast<U>(z), static_cast<U>(w));
	}

	//---------------------------
	// operators
	//---------------------------
	bool operator==(const Vector4 &rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
	}
	bool operator!=(const Vector4 &rhs) const
	{
		return !((*this) == rhs);
	}

	Vector4 operator+()
	{
		return *this;
	}
	Vector4 operator-()
	{
		return Vector4(-x, -y, -z, -w);
	}

	const Vector4& operator+=(const Vector4 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}
	const Vector4& operator+=(const Vector3 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	const Vector4& operator-=(const Vector4 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}
	const Vector4& operator-=(const Vector3 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	const Vector4& operator*=(const T &rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}
	const Vector4& operator/=(const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w *= rhs;
		return *this;
	}

	friend Vector4<T> operator+(const Vector4<T> &lhs, const Vector4<T> &rhs)
	{
		return Vector4<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
	}
	friend Vector4<T> operator-(const Vector4<T> &lhs, const Vector4<T> &rhs)
	{
		return Vector4<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
	}
	friend Vector4<T> operator*(const T &lhs, const Vector4<T> &rhs)
	{
		return Vector4<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
	}
	friend Vector4<T> operator*(const Vector4<T> &lhs, const T &rhs)
	{
		return Vector4<T>(rhs * lhs.x, rhs * lhs.y, rhs * lhs.z, rhs * lhs.w);
	}
	friend Vector4<T> operator/(const Vector4<T> &lhs, const T &rhs)
	{
		return Vector4<T>(lhs.x / rhs, lhs.y / rhs, lhs.y / rhs, lhs.w / rhs);
	}
};


//-----------------------------------------------------------------------------------------------
//クォータニオン
//-----------------------------------------------------------------------------------------------
template<typename T>
class Quaternion
{
public:
	T r, x, y, z;

public:
	Quaternion(): r(0), x(0), y(0), z(0) {}
	Quaternion(T r, T x, T y, T z): r(r), x(x), y(y), z(z) {}
	Quaternion(const Quaternion &rhs): r(rhs.r), x(rhs.x), y(rhs.y), z(rhs.z) {}
	Quaternion &operator=(const Quaternion &rhs)
	{
		r = rhs.r;
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	~Quaternion() {}

	void set(T r, T x, T y, T z)
	{
		r = r;
		x = x;
		y = y;
		z = z;
	}
	void set(const T *ary)
	{
		r = ary[0];
		x = ary[1];
		y = ary[2];
		z = ary[3];
	}

	void setConjugation()
	{
		x = -x;
		y = -y;
		z = -z;
	}
	const Quaternion getConjugation() const
	{
		return Quaternion(r, -x, -y, -z);
	}

	void setRotation(T angle, T x, T y, T z)
	{
		T rot = angle / 2.0, _sin = sin(rot);
		r = cos(rot);
		x = x * _sin;
		y = y * _sin;
		z = z * _sin;
	}
	void setRotation(T angle, const Vector3<T> &axis)
	{
		setRotation(angle, axis.x, axis.y, axis.z)
	}

	static const Quaternion getRotation(T angle, T x, T y, T z)
	{
		T rot = angle / 2.0, _sin = sin(rot);
		return Quaternion(cos(rot), x * _sin, y * _sin, z * _sin);
	}
	static const Quaternion getRotation(T angle, const Vector3<T> &axis)
	{
		return getRotation(cos(rot), axis.x, axis.y, axis.z);
	}

	T *toAry()
	{
		return &r;
	}
	const T *toAry() const
	{
		return &r;
	}

	const T sumsq() const
	{
		return r * r + x * x + y * y + z * z;
	}
	const T abs() const
	{
		return sqrt(sumsq());
	}
	const Quaternion inv() const
	{
		T abs = this->abs();
		return Quaternion(r / abs, -x / abs, -y / abs, -z / abs);
	}

	bool operator==(const Quaternion &rhs) const
	{
		return (r == rhs.r && x == rhs.x && y == rhs.y && z == rhs.z);
	}
	bool operator!=(const Quaternion &rhs) const
	{
		return !((*this) == rhs);
	}

	const Quaternion operator+()
	{
		return *this;
	}
	const Quaternion operator-()
	{
		return Quaternion(-r, -x, -y, -z);
	}
	const Quaternion &operator+=(const Quaternion &rhs)
	{
		r += rhs.r;
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	const Quaternion &operator-=(const Quaternion &rhs)
	{
		r -= rhs.r;
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	const Quaternion &operator*=(const T &rhs)
	{
		r *= rhs.r;
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	const Quaternion &operator/=(const T &rhs)
	{
		r /= rhs.r;
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}
};
template<typename T>
const Quaternion<T> operator+(const Quaternion<T> &lhs, const Quaternion<T> &rhs)
{
	return Quaternion<T>(lhs.r + rhs.r, lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
template<typename T>
const Quaternion<T> operator-(const Quaternion<T> &lhs, const Quaternion<T> &rhs)
{
	return Quaternion<T>(lhs.r - rhs.r, lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
template<typename T>
const Quaternion<T> operator*(const T &lhs, const Quaternion<T> &rhs)
{
	return Quaternion<T>(lhs * rhs.r, lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}
template<typename T>
const Quaternion<T> operator*(const Quaternion<T> &lhs, const T &rhs)
{
	return Quaternion<T>(rhs * lhs.r, rhs * lhs.x, rhs * lhs.y, lhs * rhs.z);
}
template<typename T>
const Quaternion<T> operator*(const Quaternion<T> &lhs, const Quaternion<T> &rhs)
{
	Vector3<T> l(lhs.x, lhs.y, lhs.z), r(rhs.x, rhs.y, rhs.z);
	Vector3<T> vec = lhs.r * r + rhs.r * l + l.closs(r);
	return Quaternion<T>(lhs.r * rhs.r - l.dot(r), vec.x, vec.y, vec.z);
}
template<typename T>
const Quaternion<T> operator/(const Quaternion<T> &lhs, const T &rhs)
{
	return Quaternion<T>(lhs.r / rhs, lhs.x / rhs, lhs.y / rhs, lhs.y / rhs);
}
template<typename T>
const Quaternion<T> operator/(const T &lhs, const Quaternion<T> &rhs)
{
	return lhs * rhs.inv();
}
template<typename T>
const Quaternion<T> operator/(const Quaternion<T> &lhs, const Quaternion<T> &rhs)
{
	return lhs * rhs.inv();
}

//-----------------------------------------------------------------------------------------------
//行列
//-----------------------------------------------------------------------------------------------
template<typename T>
class Matrix4x4
{
private:
	T m[4][4];

public:
	Matrix4x4()
	{
		setIdentity();
	}
	Matrix4x4(const T *ary)
	{
		set(ary);
	}
	Matrix4x4 &operator=(const Matrix4x4 &rhs)
	{
		set(rhs.toAry());
		return *this;
	}
	Matrix4x4(T _m11, T _m12, T _m13, T _m14,
			  T _m21, T _m22, T _m23, T _m24,
			  T _m31, T _m32, T _m33, T _m34,
			  T _m41, T _m42, T _m43, T _m44)
	{
		m[0][0] = _m11;
		m[0][1] = _m12;
		m[0][2] = _m13;
		m[0][3] = _m14;
		m[1][0] = _m21;
		m[1][1] = _m22;
		m[1][2] = _m23;
		m[1][3] = _m24;
		m[2][0] = _m31;
		m[2][1] = _m32;
		m[2][2] = _m33;
		m[2][3] = _m34;
		m[3][0] = _m41;
		m[3][1] = _m42;
		m[3][2] = _m43;
		m[3][3] = _m44;
	}

	void set(T _m11, T _m12, T _m13, T _m14,
			 T _m21, T _m22, T _m23, T _m24,
			 T _m31, T _m32, T _m33, T _m34,
			 T _m41, T _m42, T _m43, T _m44)
	{
		m[0][0] = _m11;
		m[0][1] = _m12;
		m[0][2] = _m13;
		m[0][3] = _m14;
		m[1][0] = _m11;
		m[1][1] = _m22;
		m[1][2] = _m23;
		m[1][3] = _m24;
		m[2][0] = _m31;
		m[2][1] = _m32;
		m[2][2] = _m33;
		m[2][3] = _m34;
		m[3][0] = _m41;
		m[3][1] = _m42;
		m[3][2] = _m43;
		m[3][3] = _m44;
	}
	void set(const T *ary)
	{
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				m[i][j] = ary[i * 4 + j];
	}

	T *toAry()
	{
		return &m11;
	}
	const T *toAry() const
	{
		return &m11;
	}

	const T *operator[](int i) const
	{
		return m[i];
	}
	T *operator[](int i)
	{
		return m[i];
	}

	void setIdentity()
	{
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				m[i][j] = 0.0;
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0;
	}
	void setTranspose()
	{
		*this = getTranspose();
	}
	void setInverse()
	{
		*this = getInverse();
	}

	const T det() const
	{
		Matrix4x4 mat(*this);
		//三角行列の作成
		for(int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < 4; j++)
			{
				if(i < j)
				{
					T rate = mat[j][i] / mat[i][i];
					for(int k = 0; k < 4; k++)
						mat[j][k] -= mat[i][k] * rate;
				}
			}
		}
		//対角成分の積=行列式の値
		T det = 1.0f;;
		for(int i = 0; i < 4; i++)
			det *= mat[i][i];
		return det;
	}
	const Matrix4x4 getTranspose() const
	{
		Matrix4x4 mat;
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				mat[i][j] = m[j][i];
		return mat;
	}
	const Matrix4x4 getInverse() const
	{
		Matrix4x4 mat(*this), inv;

		//掃き出し法
		T rate;
		for(int i = 0; i < 4; i++)
		{
			rate = 1 / mat[i][i];
			for(int j = 0; j < 4; j++)
			{
				mat[i][j] *= rate;
				inv[i][j] *= rate;
			}
			for(int j = 0; j < 4; j++)
			{
				if(i != j)
				{
					rate = mat[j][i];
					for(int k = 0; k < 4; k++)
					{
						mat[j][k] -= mat[i][k] * rate;
						inv[j][k] -= inv[i][k] * rate;
					}
				}
			}
		}
		return inv;
	}

	void setTransrate(T x, T y, T z)
	{
		setIdentity();
		m[3][0] = x;
		m[3][1] = y;
		m[3][2] = z;
	}
	void setScale(T x, T y, T z)
	{
		setIdentity();
		m[0][0] = x;
		m[1][1] = y;
		m[2][2] = z;
	}
	void setRotateX(T angle)
	{
		setIdentity();
		m[1][1] = cos(angle);
		m[1][2] = sin(angle);
		m[2][1] = -m[1][2];
		m[2][2] = m[1][1];
	}
	void setRotateY(T angle)
	{
		setIdentity();
		m[0][0] = cos(angle);
		m[0][2] = -sin(angle);
		m[2][0] = -m[0][2];
		m[2][2] = m[0][0];
	}
	void setRotateZ(T angle)
	{
		setIdentity();
		m[0][0] = cos(angle);
		m[0][1] = sin(angle);
		m[1][0] = -m[0][1];
		m[1][1] = m[0][0];
	}
	void setRotateAxis(T angle, T x, T y, T z)
	{
		float _cos = cos(angle), _sin = sin(angle), one_cos = 1.0 - _cos;
		setIdentity();

		m[0][0] = x * x * one_cos + _cos;
		m[0][1] = x * y * one_cos + z * _sin;
		m[0][2] = x * z * one_cos - y * _sin;
		m[1][0] = x * y * one_cos - z * _sin;
		m[1][1] = y * y * one_cos + _cos;
		m[1][2] = y * z * one_cos + x * _sin;
		m[2][0] = x * z * one_cos + y * _sin;
		m[2][1] = y * z * one_cos - x * _sin;
		m[2][2] = z * z * one_cos + _cos;
	}
	void setRotateAxis(T angle, const Vector3<T> &axis)
	{
		setRotateAxis(angle, axis.x, axis.y, axis.z);
	}
	void setRotateFromQuaternion(const Quaternion<T> &q)
	{
		setIdentity();

		m[0][0] = 1.0 - 2.0 * q.y * q.y - 2.0 * q.z * q.z;
		m[0][1] = 2.0 * q.x * q.y + 2.0 * q.r * q.z;
		m[0][2] = 2.0 * q.x * q.z - 2.0 * q.r * q.y;
		m[1][0] = 2.0 * q.x * q.y - 2.0 * q.r * q.z;
		m[1][1] = 1.0 - 2.0 * q.x * q.x - 2.0 * q.z * q.z;
		m[1][2] = 2.0 * q.y * q.z + 2.0 * q.r * q.x;
		m[2][0] = 2.0 * q.x * q.z + 2.0 * q.r * q.y;
		m[2][1] = 2.0 * q.y * q.z - 2.0 * q.r * q.x;
		m[2][2] = 1.0 - 2.0 * q.x * q.x - 2.0 * q.y * q.y;
	}

	bool operator==(const Matrix4x4 &rhs) const
	{
		for(int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < 4; ++j)
			{
				if(m[i][j] != rhs[i][j])
					return false;
			}
		}
		return true;
	}
	bool operator!=(const Matrix4x4 &rhs) const
	{
		return !((*this) == rhs);
	}

	Matrix4x4 operator+() const
	{
		return *this;
	}
	Matrix4x4 operator-() const
	{
		Matrix4x4 t;
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				t.m[i][j] = -m[i][j];
		return t;
	}
	const Matrix4x4 &operator+=(const Matrix4x4 &rhs)
	{
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				m[i][j] += rhs[i][j];
		return *this;
	}
	const Matrix4x4 &operator-=(const Matrix4x4 &rhs)
	{
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				m[i][j] -= rhs[i][j];
		return *this;
	}
	const Matrix4x4 &operator*=(const Matrix4x4 &rhs)
	{
		Matrix4x4 mat(*this);

		for(int i = 0; i < 4; ++i)
		{
			for(int j = 0; j < 4; ++j)
			{
				m[i][j] = 0.0;
				for(int k = 0; k < 4; ++k)
					m[i][j] += mat[i][k] * rhs[k][i];
			}
		}
		return *this;
	}

	const Matrix4x4 &operator*=(const T &rhs)
	{
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				m[i][j] *= rhs;
		return *this;
	}
	const Matrix4x4 &operator/=(const T &rhs)
	{
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				m[i][j] /= rhs;
		return *this;
	}
};
template<typename T>
const Matrix4x4<T> operator+(const Matrix4x4<T> &lhs, const Matrix4x4<T> &rhs)
{
	return Matrix4x4<T>(lhs) += rhs;
}
template<typename T>
const Matrix4x4<T> operator-(const Matrix4x4<T> &lhs, const Matrix4x4<T> &rhs)
{
	return Matrix4x4<T>(lhs) -= rhs;
}

template<typename T>
const Matrix4x4<T> operator*(const T &lhs, const Matrix4x4<T> &rhs)
{
	return Matrix4x4<T>(rhs) *= lhs;
}
template<typename T>
const Matrix4x4<T> operator*(const Matrix4x4<T> &lhs, const T &rhs)
{
	return Matrix4x4<T>(lhs) *= rhs;
}
template<typename T>
const Matrix4x4<T> operator*(const Vector3<T> &lhs, const Matrix4x4<T> &rhs)
{
	return Vector3<T>(
			   lhs.x * rhs[0][0] + lhs.y * rhs[1][0] + lhs.z * rhs[2][0] + rhs[3][0],
			   lhs.x * rhs[0][1] + lhs.y * rhs[1][1] + lhs.z * rhs[2][1] + rhs[3][1],
			   lhs.x * rhs[0][2] + lhs.y * rhs[1][2] + lhs.z * rhs[2][2] + rhs[3][2]);
}
template<typename T>
const Matrix4x4<T> operator*(const Matrix4x4<T> &lhs, const Vector3<T> &rhs)
{
	return Vector3<T>(
			   lhs.x * rhs[0][0] + lhs.y * rhs[0][1] + lhs.z * rhs[0][2] + rhs[0][3],
			   lhs.x * rhs[1][0] + lhs.y * rhs[1][1] + lhs.z * rhs[1][2] + rhs[1][3],
			   lhs.x * rhs[2][0] + lhs.y * rhs[2][1] + lhs.z * rhs[2][2] + rhs[2][3]);
}
template<typename T>
const Matrix4x4<T> operator*(const Vector4<T> &lhs, const Matrix4x4<T> &rhs)
{
	return Vector3<T>(
			   lhs.x * rhs[0][0] + lhs.y * rhs[1][0] + lhs.z * rhs[2][0] + lhs.w * rhs[3][0],
			   lhs.x * rhs[0][1] + lhs.y * rhs[1][1] + lhs.z * rhs[2][1] + lhs.w * rhs[3][1],
			   lhs.x * rhs[0][2] + lhs.y * rhs[1][2] + lhs.z * rhs[2][2] + lhs.w * rhs[3][2]);
}
template<typename T>
const Matrix4x4<T> operator*(const Matrix4x4<T> &lhs, const Vector4<T> &rhs)
{
	return Vector3<T>(
			   lhs.x * rhs[0][0] + lhs.y * rhs[0][1] + lhs.z * rhs[0][2] + lhs.w * rhs[0][3],
			   lhs.x * rhs[1][0] + lhs.y * rhs[1][1] + lhs.z * rhs[1][2] + lhs.w * rhs[1][3],
			   lhs.x * rhs[2][0] + lhs.y * rhs[2][1] + lhs.z * rhs[2][2] + lhs.w * rhs[2][3]);
}
template<typename T>
const Matrix4x4<T> operator*(const Matrix4x4<T> &lhs, const Matrix4x4<T> &rhs)
{
	return Matrix4x4<T>(lhs) *= rhs;
}

template<typename T>
const Matrix4x4<T> operator/(const Matrix4x4<T> &lhs, const T &rhs)
{
	return Matrix4x4<T>(lhs) /= rhs;
}

}