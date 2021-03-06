#ifndef UTILITIES_COLLECTIONS_STACK_H
#define UTILITIES_COLLECTIONS_STACK_H

#include "../Exceptions/Exceptions.h"
#include "../Memory.h"

namespace Utilities
{
	namespace Collections
	{
		// Vector
		template <class TYPE>
		class Stack
		{
		private:
			// Node
			struct Node
			{
				TYPE data;
				struct Node * previous;
				Node() : previous(NULL) {}
			};

			// Fields
			Node * head;
			int size;

		public:
			// Construction
			Stack();
			~Stack();

			// Properties
			bool IsEmpty() const;
			int Size() const;
			TYPE & Top();
			const TYPE & Top() const;

			// Methods
			void Push(const TYPE & x);
			void Pop();
			void Free();
		};

		// Construction
		template <class TYPE> inline
		Stack<TYPE>::Stack()
			: head(NULL), size(0)
		{
		}

		template <class TYPE> inline
		Stack<TYPE>::~Stack()
		{
			while (this->head != NULL)
				this->Pop();
		}

		// Properties
		template <class TYPE> inline
		bool Stack<TYPE>::IsEmpty() const
		{
			return this->head == NULL;
		}

		template <class TYPE> inline
		int Stack<TYPE>::Size() const
		{
			return this->size;
		}

		template <class TYPE> inline
		TYPE & Stack<TYPE>::Top()
		{
			if (this->head == NULL)
				throw Exceptions::InvalidOperationException();
			return this->head->data;
		}

		template <class TYPE> inline
		const TYPE & Stack<TYPE>::Top() const
		{
			if (this->head == NULL)
				throw Exceptions::InvalidOperationException();
			return this->head->data;
		}

		// Methods
		template <class TYPE>
		void Stack<TYPE>::Push(const TYPE & x)
		{
			Node * node = new(bl) Node;
			node->data = x;
			node->previous = this->head;
			this->head = node;
			++this->size;
		}

		template <class TYPE>
		void Stack<TYPE>::Pop()
		{
			if (this->head == NULL)
			{
				throw Exceptions::StackUnderflowException();
			}
			else
			{
				Node * node = this->head;
				this->head = node->previous;
				delete node;
				--this->size;
			}
		}

		template <class TYPE>
		void Stack<TYPE>::Free()
		{
			while (this->head != NULL)
				this->Pop();
		}
	}
}

#endif
