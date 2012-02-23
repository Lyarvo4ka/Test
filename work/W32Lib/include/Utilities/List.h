#pragma once

namespace W32Lib
{
	template <class Type>
	class List
	{
		typedef struct NODE
		{
			NODE *prev;
			Type *item;
			NODE *next;

			NODE() : prev (0), item(0), next(0) {}
			NODE(NODE *prev_node, Type *new_item) : prev (prev_node), item(new_item), next(0) {}
		};

		private:

			unsigned long count;
			NODE *curr_node;
			NODE *first_node;
			NODE *last_node;

		public:

			List(void) : count(0), curr_node(0), first_node(0), last_node(0) {}
			~List(void) {Clear();}

			// Добавляет элемент в конец списка
			bool Add(Type *item);

			// Удаляет элемент из списка по указанному порядковому номеру <index> 
			void Remove(unsigned long index);
			
			// Возвращает указанный элемент
			Type *GetItem(unsigned long index);

			// Возвращает первый элемент списка
			Type *GetFirstItem();

			// Возвращает следующий элемент списка
			Type *GetNextItem();
			
			// Возвращает кол-во элементов в списке
			unsigned long GetCount() {return count;}

			// Удаляет все элементы из списка
			void Clear();
	};

	template<class Type>	//*
	bool List<Type>::Add(Type *item)	
	{
		try
		{
			NODE *new_node = new NODE(last_node, item);
			if (count) {last_node->next = new_node; last_node = new_node;}
			else first_node = last_node = new_node;
			++count;
			return true;
		}
		catch(std::bad_alloc)
		{
			return false;
		}
	}

	template <class Type>
	void List<Type>::Remove(unsigned long index)
	{
		if (index < count)
		{
			NODE *node = first_node;
			for (unsigned int i = 0; i < index; ++i)
				node = node->next;

			if (node->next && node->prev)
			{
				(node->prev)->next = node->next;
				(node->next)->prev = node->prev;
			}
			if (node->next && !node->prev) // node является первой
			{
				(node->next)->prev = 0;
				first_node = node->next;
			}
			if (node->prev && !node->next) // node является последней
			{
				(node->prev)->next = 0;
				last_node = node->prev;
			}
			if (!node->next && !node->prev) // список содержит только одну node
			{
				first_node = last_node = 0;
			}

			--count;
			delete node;
		}
	}

	template <class Type>	//*
	Type *List<Type>::GetItem(unsigned long index)
	{
		if (index < count)
		{
			NODE *node = first_node;
			for (unsigned int i = 0; i < index; ++i)
				node = node->next;
			return node->item;
		}
		else return 0;
	}

	template <class Type>	//*
	Type *List<Type>::GetFirstItem()
	{
		if (first_node) return (curr_node = first_node)->item;
		else return 0;
	}

	template <class Type>	//*
	Type *List<Type>::GetNextItem()
	{
		if (curr_node && curr_node->next) return (curr_node = curr_node->next)->item;
		else return 0;
	}

	template <class Type>	//*
	void List<Type>::Clear()
	{
		while(count)
		{
			if (last_node->prev)
			{
				last_node = last_node->prev;
				delete last_node->next;
			}
			else delete last_node;
			--count;
		}
		first_node = last_node = curr_node = 0;
	}

}