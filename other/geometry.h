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
	Vector2(): x(0), y(0) {}
	Vector2(T _x, T _y): x(_x), y(_y) {}
	Vector2(const Vector2 &rhs): x(rhs.x), y(rhs.y) {}
	Vector2 &operator=(const Vector2 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	~Vector2() {}

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

	T *toAry()
	{
		return &x;
	}
	const T *toAry() const
	{
		return &x;
	}

	const T sumsq() const
	{
		return x * x + y * y;
	}
	const T abs() const
	{
		return sqrt(sumsq());
	}
	const T dot(const Vector2 &rhs) const
	{
		return x * rhs.X + y * rhs.Y;
	}
	const T closs(const Vector2 &rhs) const
	{
		return x * rhs.X - y * rhs.Y;
	}

	bool operator==(const Vector2 &rhs) const
	{
		return (x == rhs.X && y == rhs.Y);
	}
	bool operator!=(const Vector2 &rhs) const
	{
		return !((*this) == rhs);
	}

	const Vector2 operator+()
	{
		return *this;
	}
	const Vector2 operator-()
	{
		return Vector2(-x, -y);
	}
	const Vector2 &operator+=(const Vector2 &rhs)
	{
		x += rhs.X;
		y += rhs.Y;
		return *this;
	}
	const Vector2 &operator-=(const Vector2 &rhs)
	{
		x -= rhs.X;
		y -= rhs.Y;
		return *this;
	}
	const Vector2 &operator*=(const T &rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	const Vector2 &operator/=(const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
};
template<typename T>
const Vector2<T> operator+(const Vector2<T> &lhs, const Vector2<T> &rhs)
{
	return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}
template<typename T>
const Vector2<T> operator-(const Vector2<T> &lhs, const Vector2<T> &rhs)
{
	return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}
template<typename T, typename U>
const Vector2<T> operator*(const U &lhs, const Vector2<T> &rhs)
{
	return Vector2<T>(rhs.x * lhs, rhs.y * lhs);
}
template<typename T, typename U>
const Vector2<T> operator*(const Vector2<T> &lhs, const U &rhs)
{
	return Vector2<T>(lhs.x * rhs, lhs.y * rhs);
}
template<typename T, typename U>
const Vector2<T> operator/(const Vector2<T> &lhs, const U &rhs)
{
	return Vector2<T>(lhs.x / rhs, lhs.y / rhs);
}

//-----------------------------------------------------------------------------------------------
//3次元ベクトル
//-----------------------------------------------------------------------------------------------
template<typename T>
class Vector3
{
public:
	T x, y, z;

public:
	Vector3(): x(0), y(0), z(0) {}
	Vector3(T _x, T _y, T _z): x(_x), y(_y), z(_z) {}
	Vector3(const Vector3 &rhs): x(rhs.x), y(rhs.y), z(rhs.z) {}
	Vector3 &operator=(const Vector3 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	~Vector3() {}

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

	T *toAry()
	{
		return &x;
	}
	const T *toAry() const
	{
		return &x;
	}

	const T sumsq() const
	{
		return x * x + y * y + z * z;
	}
	const T abs() const
	{
		return sqrt(sumsq());
	}
	const T dot(const Vector3 &rhs) const
	{
		return x * rhs.X + y * rhs.Y + z * rhs.Z;
	}
	const Vector3 closs(const Vector3 &rhs) const
	{
		return Vector3(y * rhs.Z - z * rhs.Y, z * rhs.X - x * rhs.Z, x * rhs.Y - y * rhs.X);
	}

	bool operator==(const Vector3 &rhs) const
	{
		return (x == rhs.X && y == rhs.Y && z == rhs.Z);
	}
	bool operator!=(const Vector3 &rhs) const
	{
		return !((*this) == rhs);
	}

	const Vector3 operator+()
	{
		return *this;
	}
	const Vector3 operator-()
	{
		return Vector3(-x, -y, -z);
	}
	const Vector3 &operator+=(const Vector3 &rhs)
	{
		x += rhs.X;
		y += rhs.Y;
		z += rhs.Z;
		return *this;
	}
	const Vector3 &operator-=(const Vector3 &rhs)
	{
		x -= rhs.X;
		y -= rhs.Y;
		z -= rhs.Z;
		return *this;
	}
	const Vector3 &operator*=(const T &rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	const Vector3 &operator/=(const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}
};
template<typename T>
const Vector3<T> operator+(const Vector3<T> &lhs, const Vector3<T> &rhs)
{
	return Vector3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
template<typename T>
const Vector3<T> operator-(const Vector3<T> &lhs, const Vector3<T> &rhs)
{
	return Vector3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
template<typename T, typename U>
const Vector3<T> operator*(const U &lhs, const Vector3<T> &rhs)
{
	return Vector3<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}
template<typename T, typename U>
const Vector3<T> operator*(const Vector3<T> &lhs, const U &rhs)
{
	return Vector3<T>(rhs * lhs.x, rhs * lhs.y, rhs * lhs.z);
}
template<typename T, typename U>
const Vector3<T> operator/(const Vector3<T> &lhs, const U &rhs)
{
	return Vector3<T>(lhs.x / rhs, lhs.y / rhs, lhs.y / rhs);
}

//-----------------------------------------------------------------------------------------------
//4次元ベクトル
//-----------------------------------------------------------------------------------------------
template<typename T>
class Vector4
{
public:
	T X, Y, Z, W;

public:
	Vector4(): X(0), Y(0), Z(0), W(0) {}
	Vector4(T x, T y, T z, T w): X(x), Y(y), Z(z), W(w) {}
	Vector4(const Vector3<T> &rhs): X(rhs.x), Y(rhs.y), Z(rhs.z), W(1.0) {}
	Vector4(const Vector4 &rhs): X(rhs.X), Y(rhs.Y), Z(rhs.Z), W(rhs.W) {}
	Vector4 &operator=(const Vector3<T> &rhs)
	{
		X = rhs.x;
		Y = rhs.y;
		Z = rhs.z;
		W = 1.0;
		return *this;
	}
	Vector4 &operator=(const Vector4 &rhs)
	{
		X = rhs.X;
		Y = rhs.Y;
		Z = rhs.Z;
		W = rhs.W;
		return *this;
	}
	~Vector4() {}

	void set(T x, T y, T z, T w)
	{
		X = x;
		Y = y;
		Z = z;
		W = w;
	}
	void set(const T *ary)
	{
		X = ary[0];
		Y = ary[1];
		Z = ary[2];
		W = ary[3];
	}

	T *toAry()
	{
		return &X;
	}
	const T *toAry() const
	{
		return &X;
	}

	const T sumsq() const
	{
		return X * X + Y * Y + Z * Z + W * W;
	}
	const T sumsq3D() const
	{
		return X * X + Y * Y + Z * Z;
	}
	const T abs() const
	{
		return sqrt(sumsq());
	}
	const T abs3D() const
	{
		return sqrt(sumsq3D());
	}
	const T dot(const Vector4 &rhs) const
	{
		return X * rhs.X + Y * rhs.Y + Z * rhs.Z + W * rhs.W;
	}
	const T dot3D(const Vector4 &rhs) const
	{
		return X * rhs.X + Y * rhs.Y + Z * rhs.Z;
	}
	const Vector3<T> closs3D(const Vector4 &rhs) const
	{
		return Vector3<T>(Y * rhs.Z - Z * rhs.Y, Z * rhs.X - X * rhs.Z, X * rhs.Y - Y * rhs.X);
	}

	bool operator==(const Vector4 &rhs) const
	{
		return (X == rhs.X && Y == rhs.Y && Z == rhs.Z && W == rhs.W);
	}
	bool operator!=(const Vector4 &rhs) const
	{
		return !((*this) == rhs);
	}

	const Vector4 operator+()
	{
		return *this;
	}
	const Vector4 operator-()
	{
		return Vector4(-X, -Y, -Z, -W);
	}
	const Vector4 &operator+=(const Vector4 &rhs)
	{
		X += rhs.X;
		Y += rhs.Y;
		Z += rhs.Z;
		W += rhs.W;
		return *this;
	}
	const Vector4 &operator-=(const Vector4 &rhs)
	{
		X -= rhs.X;
		Y -= rhs.Y;
		Z -= rhs.Z;
		W -= rhs.W;
		return *this;
	}
	const Vector4 &operator*=(const T &rhs)
	{
		X *= rhs;
		Y *= rhs;
		Z *= rhs;
		W *= rhs;
		return *this;
	}
	const Vector4 &operator/=(const T &rhs)
	{
		X /= rhs;
		Y /= rhs;
		Z /= rhs;
		W *= rhs;
		return *this;
	}
};
template<typename T>
const Vector4<T> operator+(const Vector4<T> &lhs, const Vector4<T> &rhs)
{
	return Vector4<T>(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z, lhs.W + rhs.W);
}
template<typename T>
const Vector4<T> operator-(const Vector4<T> &lhs, const Vector4<T> &rhs)
{
	return Vector4<T>(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z, lhs.W - rhs.W);
}
template<typename T, typename U>
const Vector4<T> operator*(const U &lhs, const Vector4<T> &rhs)
{
	return Vector4<T>(lhs * rhs.X, lhs * rhs.Y, lhs * rhs.Z, lhs * rhs.W);
}
template<typename T, typename U>
const Vector4<T> operator*(const Vector4<T> &lhs, const U &rhs)
{
	return Vector4<T>(rhs * lhs.X, rhs * lhs.Y, rhs * lhs.Z, rhs * lhs.W);
}
template<typename T, typename U>
const Vector4<T> operator/(const Vector4<T> &lhs, const U &rhs)
{
	return Vector4<T>(lhs.X / rhs, lhs.Y / rhs, lhs.Y / rhs, lhs.W / rhs);
}

//-----------------------------------------------------------------------------------------------
//クォータニオン
//-----------------------------------------------------------------------------------------------
template<typename T>
class Quaternion
{
public:
	T R, X, Y, Z;

public:
	Quaternion(): R(0), X(0), Y(0), Z(0) {}
	Quaternion(T r, T x, T y, T z): R(r), X(x), Y(y), Z(z) {}
	Quaternion(const Quaternion &rhs): R(rhs.R), X(rhs.X), Y(rhs.Y), Z(rhs.Z) {}
	Quaternion &operator=(const Quaternion &rhs)
	{
		R = rhs.R;
		X = rhs.X;
		Y = rhs.Y;
		Z = rhs.Z;
		return *this;
	}
	~Quaternion() {}

	void set(T r, T x, T y, T z)
	{
		R = r;
		X = x;
		Y = y;
		Z = z;
	}
	void set(const T *ary)
	{
		R = ary[0];
		X = ary[1];
		Y = ary[2];
		Z = ary[3];
	}

	void setConjugation()
	{
		X = -X;
		Y = -Y;
		Z = -Z;
	}
	const Quaternion getConjugation() const
	{
		return Quaternion(R, -X, -Y, -Z);
	}

	void setRotation(T angle, T x, T y, T z)
	{
		T rot = angle / 2.0, _sin = sin(rot);
		R = cos(rot);
		X = x * _sin;
		Y = y * _sin;
		Z = z * _sin;
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
		return &R;
	}
	const T *toAry() const
	{
		return &R;
	}

	const T sumsq() const
	{
		return R * R + X * X + Y * Y + Z * Z;
	}
	const T abs() const
	{
		return sqrt(sumsq());
	}
	const Quaternion inv() const
	{
		T abs = this->abs();
		return Quaternion(R / abs, -X / abs, -Y / abs, -Z / abs);
	}

	bool operator==(const Quaternion &rhs) const
	{
		return (R == rhs.R && X == rhs.X && Y == rhs.Y && Z == rhs.Z);
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
		return Quaternion(-R, -X, -Y, -Z);
	}
	const Quaternion &operator+=(const Quaternion &rhs)
	{
		R += rhs.R;
		X += rhs.X;
		Y += rhs.Y;
		Z += rhs.Z;
		return *this;
	}
	const Quaternion &operator-=(const Quaternion &rhs)
	{
		R -= rhs.R;
		X -= rhs.X;
		Y -= rhs.Y;
		Z -= rhs.Z;
		return *this;
	}
	const Quaternion &operator*=(const T &rhs)
	{
		R *= rhs.R;
		X *= rhs;
		Y *= rhs;
		Z *= rhs;
		return *this;
	}
	const Quaternion &operator/=(const T &rhs)
	{
		R /= rhs.R;
		X /= rhs;
		Y /= rhs;
		Z /= rhs;
		return *this;
	}
};
template<typename T>
const Quaternion<T> operator+(const Quaternion<T> &lhs, const Quaternion<T> &rhs)
{
	return Quaternion<T>(lhs.R + rhs.R, lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z);
}
template<typename T>
const Quaternion<T> operator-(const Quaternion<T> &lhs, const Quaternion<T> &rhs)
{
	return Quaternion<T>(lhs.R - rhs.R, lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z);
}
template<typename T>
const Quaternion<T> operator*(const T &lhs, const Quaternion<T> &rhs)
{
	return Quaternion<T>(lhs * rhs.R, lhs * rhs.X, lhs * rhs.Y, lhs * rhs.Z);
}
template<typename T>
const Quaternion<T> operator*(const Quaternion<T> &lhs, const T &rhs)
{
	return Quaternion<T>(rhs * lhs.R, rhs * lhs.X, rhs * lhs.Y, lhs * rhs.Z);
}
template<typename T>
const Quaternion<T> operator*(const Quaternion<T> &lhs, const Quaternion<T> &rhs)
{
	Vector3<T> l(lhs.X, lhs.Y, lhs.Z), r(rhs.X, rhs.Y, rhs.Z);
	Vector3<T> vec = lhs.R * r + rhs.R * l + l.closs(r);
	return Quaternion<T>(lhs.R * rhs.R - l.dot(r), vec.x, vec.y, vec.z);
}
template<typename T>
const Quaternion<T> operator/(const Quaternion<T> &lhs, const T &rhs)
{
	return Quaternion<T>(lhs.R / rhs, lhs.X / rhs, lhs.Y / rhs, lhs.Y / rhs);
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
		setRotateAxis(angle, axis.X, axis.Y, axis.Z);
	}
	void setRotateFromQuaternion(const Quaternion<T> &q)
	{
		setIdentity();

		m[0][0] = 1.0 - 2.0 * q.Y * q.Y - 2.0 * q.Z * q.Z;
		m[0][1] = 2.0 * q.X * q.Y + 2.0 * q.R * q.Z;
		m[0][2] = 2.0 * q.X * q.Z - 2.0 * q.R * q.Y;
		m[1][0] = 2.0 * q.X * q.Y - 2.0 * q.R * q.Z;
		m[1][1] = 1.0 - 2.0 * q.X * q.X - 2.0 * q.Z * q.Z;
		m[1][2] = 2.0 * q.Y * q.Z + 2.0 * q.R * q.X;
		m[2][0] = 2.0 * q.X * q.Z + 2.0 * q.R * q.Y;
		m[2][1] = 2.0 * q.Y * q.Z - 2.0 * q.R * q.X;
		m[2][2] = 1.0 - 2.0 * q.X * q.X - 2.0 * q.Y * q.Y;
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
			   lhs.X * rhs[0][0] + lhs.Y * rhs[0][1] + lhs.Z * rhs[0][2] + rhs[0][3],
			   lhs.X * rhs[1][0] + lhs.Y * rhs[1][1] + lhs.Z * rhs[1][2] + rhs[1][3],
			   lhs.X * rhs[2][0] + lhs.Y * rhs[2][1] + lhs.Z * rhs[2][2] + rhs[2][3]);
}
template<typename T>
const Matrix4x4<T> operator*(const Vector4<T> &lhs, const Matrix4x4<T> &rhs)
{
	return Vector3<T>(
			   lhs.X * rhs[0][0] + lhs.Y * rhs[1][0] + lhs.Z * rhs[2][0] + lhs.W * rhs[3][0],
			   lhs.X * rhs[0][1] + lhs.Y * rhs[1][1] + lhs.Z * rhs[2][1] + lhs.W * rhs[3][1],
			   lhs.X * rhs[0][2] + lhs.Y * rhs[1][2] + lhs.Z * rhs[2][2] + lhs.W * rhs[3][2]);
}
template<typename T>
const Matrix4x4<T> operator*(const Matrix4x4<T> &lhs, const Vector4<T> &rhs)
{
	return Vector3<T>(
			   lhs.X * rhs[0][0] + lhs.Y * rhs[0][1] + lhs.Z * rhs[0][2] + lhs.W * rhs[0][3],
			   lhs.X * rhs[1][0] + lhs.Y * rhs[1][1] + lhs.Z * rhs[1][2] + lhs.W * rhs[1][3],
			   lhs.X * rhs[2][0] + lhs.Y * rhs[2][1] + lhs.Z * rhs[2][2] + lhs.W * rhs[2][3]);
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
