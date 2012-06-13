#pragma once

#include <new>
#include <vector>
#include <cassert>
#include <boost/utility.hpp>

template<typename T>
class MemoryManageList
{
public:
	class iterator;
	typedef T value_type;
	typedef const iterator const_iterator;

public:
	// コンストラクタ
	MemoryManageList(size_t max_num, size_t max_size)
		: MAX_ELEMENT_NUM(max_num)
		, MAX_OBJECT_SIZE(max_size)
		, free_num(max_num)
		, storage(max_num)
	{
		// 最大サイズがT未満ならアサート
		assert(MAX_OBJECT_SIZE>=sizeof(T));

		// ストレージの確保
		for (size_t i=0;i<MAX_ELEMENT_NUM;++i){
			storage[i].memory.resize(MAX_OBJECT_SIZE);
		}
		// リストの初期化
		resetList();
	}
	MemoryManageList(const MemoryManageList& rhs)
		: MAX_ELEMENT_NUM(rhs.MAX_ELEMENT_NUM)
		, MAX_OBJECT_SIZE(rhs.MAX_OBJECT_SIZE)
		, free_num(rhs.MAX_ELEMENT_NUM)
		, storage(rhs.storage)
	{
		// リストの初期化
		resetList();
	}
	MemoryManageList& operator=(const MemoryManageList& rhs){
		MAX_ELEMENT_NUM =rhs.MAX_ELEMENT_NUM;
		MAX_OBJECT_SIZE =rhs.MAX_OBJECT_SIZE;
		free_num =rhs.MAX_ELEMENT_NUM;
		storage =rhs.storage;
		resetList();
	}

	// デストラクタ
	~MemoryManageList(){
		clear();
	}

	inline size_t size() const{return MAX_ELEMENT_NUM-free_num;}
	inline size_t capacity() const{return MAX_ELEMENT_NUM;}

	inline iterator begin(){return iterator(head_active.next);}
	inline iterator end(){return iterator(&head_active);}
	inline const_iterator begin() const{return iterator(head_active.next);}
	inline const_iterator end() const{return iterator(&head_active);}

	// すべて破棄
	void clear(){
		// すべての使用中オブジェクトを開放
		for (Node* n =head_active.next; n!=&head_active; n =n->next)
			node_cast(n)->~value_type();
		// リストのリセット
		resetList();
	}

	void pop_back(){
		if(head_active.next == &head_active)
			return;

		// 使用済みリストから外す
		Node *back       =head_active.prev;
		node_cast(back)->~value_type();
		head_active.prev =back->prev;
		back->prev->next =&head_active;

		// 未使用リストに追加
		back->next       =head_free.next;
		back->prev       =&head_free;
		back->next->prev =back;
		head_free.next   =back;
	}

	void pop_front(){
		if(head_active.next == &head_active)
			return;

		// 使用済みリストから外す
		Node *front       =head_active.next;
		node_cast(front)->~value_type();
		head_active.next =front->next;
		front->next->prev =&head_active;

		// 未使用リストに追加
		front->next       =head_free.next;
		front->prev       =&head_free;
		front->next->prev =front;
		head_free.next    =front;
	}

	iterator erase(const iterator &it){
		// 一つ先のイテレータを返す
		iterator result =it;
		++result;

		// 一つ未使用に
		++free_num;

		// 使用済みリストから外す
		Node *current       =it.current;
		node_cast(current)->~value_type();
		current->next->prev =current->prev;
		current->prev->next =current->next;

		// 未使用リストに追加
		current->next       =head_free.next;
		current->prev       =&head_free;
		current->next->prev =current;
		head_free.next      =current;

		return result;
	}

	// 引数0で一つ追加
	template<typename Class>
	Class* construct(){
		// 最大サイズがT未満ならアサート
		assert(MAX_OBJECT_SIZE>=sizeof(Class));

		void* mem =getNextFreeStorage();
		if (mem != NULL){
			return new(mem) Class();
		}
		return NULL;
	}

	// 引数1で一つ追加
	template<typename Class, typename Arg>
	Class* construct(Arg arg){
		// 最大サイズがT未満ならアサート
		assert(MAX_OBJECT_SIZE>=sizeof(Class));

		void* mem =getNextFreeStorage();
		if (mem != NULL){
			return new(mem) Class(arg);
		}
		return NULL;
	}

private:
	struct Node{
		std::vector<char> memory;
		Node* prev;
		Node* next;
	};
	const size_t MAX_ELEMENT_NUM;
	const size_t MAX_OBJECT_SIZE;
	size_t free_num;

	std::vector<Node> storage;
	Node head_active;
	Node head_free;

private:
	inline static value_type* node_cast(Node* node){
		return (value_type*)(node->memory.data());
	}

	void* getNextFreeStorage(){
		if (head_free.next != &head_free){
			//一つ使用済みに
			--free_num;

			// 未使用ノードを一つ取出し
			Node* result       =head_free.next;
			head_free.next     =result->next;
			result->next->prev =&head_free;

			// 使用中リストの末尾に挿入
			result->next       =&head_active;
			result->prev       =head_active.prev;
			result->prev->next =result;
			head_active.prev   =result;

			return (void*)node_cast(result);
		}
		return (void*)NULL;
	}

	void resetList(){
		// 使用中リストの初期化
		head_active.next =head_active.prev =&head_active;

		// 未使用リストの初期化
		head_free.next =&storage[0];
		head_free.prev =&storage[MAX_ELEMENT_NUM-1];
		storage[0].prev =&head_free;
		for (size_t i=0;i<MAX_ELEMENT_NUM-1;++i){
			storage[i].next =&storage[i+1];
			storage[i+1].prev =&storage[i];
		}
		storage[MAX_ELEMENT_NUM-1].next =&head_free;

		free_num =MAX_ELEMENT_NUM;
	}

public:
	class iterator {
		friend class MemoryManageList;
	public:
		typedef T value_type;
		inline iterator():current(NULL){}
		
		// 間接参照演算子
		inline value_type& operator*(){return *node_cast(current);}
		inline const value_type& operator*() const{return *node_cast(current);}
		inline value_type* operator->(){return node_cast(current);}
		inline const value_type* operator->() const{return node_cast(current);}

		// イテレータ操作
		inline iterator operator++(){
			current = current->next;
			return *this;
		}
		inline iterator operator--(){
			current = current->prev;
			return *this;
		}
		inline iterator operator++(int){
			iterator result(*this);
			current = current->next
			return result;
		}
		inline iterator operator--(int){
			iterator result(*this);
			current = current->prev;
			return result;
		}
		inline bool operator==(const iterator& rhs) const{return current==rhs.current;}
		inline bool operator!=(const iterator& rhs) const{return !(*this==rhs);}

	private:
		inline iterator(Node* n):current(n){}
		Node *current;
	};

};

