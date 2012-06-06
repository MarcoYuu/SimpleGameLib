#pragma once

// 自分のライブラリの名前空間
namespace yuu
{
//------------------------------------------------------------------
//ポインタキャスト
//------------------------------------------------------------------
// 任意の型のポインタへのキャスト
// キャストするポインタ
// キャストされたポインタ
template <class T>
inline T *pointer_cast(void *p)
{
	return static_cast<T *>(p);
}

//------------------------------------------------------------------
//自動削除用継承元クラス
//------------------------------------------------------------------
// 参照カウントインタフェース
//
// 参照カウントを自らが持つクラスへのインタフェース
// 継承することで自動削除のポインタが使用できる
//
template<class Derived>
class IRefferenceCount
{
private:
	std::size_t m_ref_count;

public:
	// コンストラクタ
	IRefferenceCount()
		: m_ref_count(0)
	{}

	// 現在の参照数の取得
	// キャストされたポインタ
	inline std::size_t getRef() const
	{
		return m_ref_count;
	}

	// intrusive_ptr用 参照が増えた時呼ばれる処理
	friend void intrusive_ptr_add_ref(Derived *p)
	{
		p->m_ref_count++;
	}

	// intrusive_ptr用 参照が減った時呼ばれる処理 参照が0になるとdeleteが呼ばれる。
	friend void intrusive_ptr_release(Derived *p)
	{
		p->m_ref_count--;
		if(p->m_ref_count <= 0)
			delete p;
	}
};
}