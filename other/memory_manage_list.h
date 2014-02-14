#pragma once

#include <new>
#include <vector>
#include <cassert>

namespace yuu{

	//--------------------------------------------------------------------------
	// ���̂�����ŕێ�����A���P�[�^+���X�g�ȃR���e�i
	//--------------------------------------------------------------------------
	// �E�����I�ɍŒ� max_num*max_size �̃��������m�ۂ���
	// 
	// �Econstruct ����ƃ������ɋ󂫂�����ꍇ�����ɃI�u�W�F�N�g��z�u����
	// �@�I�u�W�F�N�g�̃T�C�Y�ɂ���Ă̓������̈�͖��ʂɂȂ�
	// 
	// �E�p����̃N���X�ł���Ί��N���X�̃��X�g��ɂ��\�z�ł��邪�A
	// �@max_size�𒴉߂���悤�Ȕh���N���X�͍\�z�ł��Ȃ�
	// 
	// �E���X�g�̃R�s�[���ɓ��e�̓R�s�[����Ȃ�.�R�s�[�����̂�
	// �@�i�[�����^����m�ۂ��ꂽ�������T�C�Y�݂̂ł���
	// 
	// �E���̃N���X���̂��̂̍\�z�̎��Ԃ�std::list���Ƃ͔�r�ɂȂ�Ȃ����炢�傫����
	// �@�ǉ�/�폜���̎��Ԃ͑�̍Œ�1/3���x�ɂ܂ŗ}������
	//
	template<typename T>
	class MemoryManageList
	{
	public:
		class iterator;
		typedef T value_type;
		typedef const iterator const_iterator;

	public:
		// �R���X�g���N�^
		MemoryManageList(size_t max_num, size_t max_size)
			: MAX_ELEMENT_NUM(max_num)
			, MAX_OBJECT_SIZE(max_size)
			, free_num(max_num)
			, memory(MAX_ELEMENT_NUM*MAX_OBJECT_SIZE)
			, storage(max_num)
		{
			// �ő�T�C�Y��T�����Ȃ�A�T�[�g
			assert(MAX_OBJECT_SIZE>=sizeof(T));

			// �X�g���[�W�̊m��
			for (size_t i=0;i<MAX_ELEMENT_NUM;++i){
				storage[i].memory =memory.data()+i*MAX_OBJECT_SIZE;
			}
			// ���X�g�̏�����
			resetList();
		}
		MemoryManageList(const MemoryManageList& rhs)
			: MAX_ELEMENT_NUM(rhs.MAX_ELEMENT_NUM)
			, MAX_OBJECT_SIZE(rhs.MAX_OBJECT_SIZE)
			, free_num(rhs.MAX_ELEMENT_NUM)
			, memory(rhs.MAX_ELEMENT_NUM*rhs.MAX_OBJECT_SIZE)
			, storage(rhs.MAX_ELEMENT_NUM)
		{
			// �X�g���[�W�̊m��
			for (size_t i=0;i<MAX_ELEMENT_NUM;++i){
				storage[i].memory =memory.data()+i*MAX_OBJECT_SIZE;
			}
			resetList();
		}
		MemoryManageList& operator=(const MemoryManageList& rhs){
			MAX_ELEMENT_NUM =rhs.MAX_ELEMENT_NUM;
			MAX_OBJECT_SIZE =rhs.MAX_OBJECT_SIZE;
			free_num        =rhs.MAX_ELEMENT_NUM;

			// �������m�ۂƐ؂�l��
			memory.resize(rhs.MAX_ELEMENT_NUM*rhs.MAX_OBJECT_SIZE);
			std::vector<char>(memory).swap(memory);
			storage.resize(rhs.MAX_ELEMENT_NUM);
			std::vector<Node>(storage).swap(storage);

			// �X�g���[�W�̊m��
			for (size_t i=0;i<MAX_ELEMENT_NUM;++i){
				storage[i].memory =memory.data()+i*MAX_OBJECT_SIZE;
			}
			resetList();
		}

		// �f�X�g���N�^
		~MemoryManageList(){
			clear();
		}

		inline size_t size() const{return MAX_ELEMENT_NUM-free_num;}
		inline size_t capacity() const{return MAX_ELEMENT_NUM;}

		inline iterator begin(){return iterator(head_active.next);}
		inline iterator end(){return iterator(&head_active);}
		inline const_iterator begin() const{return iterator(head_active.next);}
		inline const_iterator end() const{return iterator(&head_active);}

		// ���ׂĔj��
		void clear(){
			// ���ׂĂ̎g�p���I�u�W�F�N�g���J��
			for (Node* n =head_active.next; n!=&head_active; n =n->next)
				node_cast(n)->~value_type();

			// ���X�g�̃��Z�b�g
			resetList();
		}

		void pop_back(){
			if(head_active.next == &head_active)
				return;

			// �g�p�ς݃��X�g����O��
			Node *back       =head_active.prev;
			node_cast(back)->~value_type();
			head_active.prev =back->prev;
			back->prev->next =&head_active;

			// ���g�p���X�g�ɒǉ�
			back->next       =head_free.next;
			back->prev       =&head_free;
			back->next->prev =back;
			head_free.next   =back;
		}

		void pop_front(){
			if(head_active.next == &head_active)
				return;

			// �g�p�ς݃��X�g����O��
			Node *front       =head_active.next;
			node_cast(front)->~value_type();
			head_active.next =front->next;
			front->next->prev =&head_active;

			// ���g�p���X�g�ɒǉ�
			front->next       =head_free.next;
			front->prev       =&head_free;
			front->next->prev =front;
			head_free.next    =front;
		}

		iterator erase(const iterator &it){
			// ���̃C�e���[�^��Ԃ�
			iterator result =it;
			++result;

			// ����g�p��
			++free_num;

			// �g�p�ς݃��X�g����O��
			Node *current       =it.current;
			node_cast(current)->~value_type();
			current->next->prev =current->prev;
			current->prev->next =current->next;

			// ���g�p���X�g�ɒǉ�
			current->next       =head_free.next;
			current->prev       =&head_free;
			current->next->prev =current;
			head_free.next      =current;

			return result;
		}

		// ����0�ň�ǉ�
		template<typename Class>
		Class* construct(){
			// �ő�T�C�Y��T�����Ȃ�A�T�[�g
			assert(MAX_OBJECT_SIZE>=sizeof(Class));

			void* mem =getNextFreeStorage();
			if (mem != NULL){
				value_type *result =new(mem) Class();
				return (Class*)result;
			}
			return NULL;
		}

		// ����1�ň�ǉ�
		template<typename Class, typename Arg>
		Class* construct(Arg arg){
			// �ő�T�C�Y��T�����Ȃ�A�T�[�g
			assert(MAX_OBJECT_SIZE>=sizeof(Class));

			void* mem =getNextFreeStorage();
			if (mem != NULL){
				value_type *result =new(mem) Class(arg);
				return (Class*)result;
			}
			return NULL;
		}

	private:
		struct Node{
			char* memory;
			Node* prev;
			Node* next;
		};
		const size_t MAX_ELEMENT_NUM;
		const size_t MAX_OBJECT_SIZE;
		size_t free_num;

		std::vector<char> memory;
		std::vector<Node> storage;
		Node head_active;
		Node head_free;

	private:
		inline static value_type* node_cast(Node* node){
			return (value_type*)(node->memory);
		}

		void* getNextFreeStorage(){
			if (head_free.next != &head_free){
				//��g�p�ς݂�
				--free_num;

				// ���g�p�m�[�h�����o��
				Node* result       =head_free.next;
				head_free.next     =result->next;
				result->next->prev =&head_free;

				// �g�p�����X�g�̖����ɑ}��
				result->next       =&head_active;
				result->prev       =head_active.prev;
				result->prev->next =result;
				head_active.prev   =result;

				return (void*)node_cast(result);
			}
			return NULL;
		}

		void resetList(){
			// �g�p�����X�g�̏�����
			head_active.next =head_active.prev =&head_active;

			// ���g�p���X�g�̏�����
			head_free.next  =&storage[0];
			head_free.prev  =&storage[MAX_ELEMENT_NUM-1];
			storage[0].prev =&head_free;

			for (size_t i=0;i<MAX_ELEMENT_NUM-1;++i){
				storage[i].next   =&storage[i+1];
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

			// �ԐڎQ�Ɖ��Z�q
			inline value_type& operator*(){return *node_cast(current);}
			inline const value_type& operator*() const{return *node_cast(current);}
			inline value_type* operator->(){return node_cast(current);}
			inline const value_type* operator->() const{return node_cast(current);}

			// �C�e���[�^����
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
}

