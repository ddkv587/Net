#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <cstdint>

namespace Net
{
	template<class T> class CAllocBase_
	{
		public:
			CAllocBase_(unsigned int size, unsigned int align)
				: m_pData(nullptr), m_uiSize(0), m_uiRef(0), m_uiAlign(0) 
			{}
			virtual ~CAllocBase_(){}

			T operator*() { return m_pData; }
			const T operator*() const { return m_pData; }

			T* alignPoint(char* raw)
			{
				T *p = reinterpret_cast<T*> (
						(reinterpret_cast<uintptr_t>(raw) + m_uiAlign - 1)
						& ~(m_uiAlign - 1)
						);
				return p;
			}

			unsigned int size() 	{ return m_uiSize; }
			unsigned int ref()		{ return m_uiRef; }
			unsigned int align()	{ return m_uiAlign; }

			void addRef() 			{ ++m_uiRef; }
			void subRef() 
			{
				--m_uiRef;
				if ( 0 == m_uiRef ) delete this;
			}

		protected:
			virtual void alloc() 	= 0;
			virtual void dealloc()	= 0;

			CAllocBase_(const CAllocBase_& src);
			CAllocBase_& operator=(const CAllocBase_& src);

		protected:
			T*		m_pData;
			char*	m_pRaw;
			unsigned int m_uiSize;
			unsigned int m_uiRef;
			unsigned int m_uiAlign;
	};

	template<class T> class CAlloc_ : public CAllocBase_<T>
	{
		public:
			explicit CAlloc_(unsigned int size = 0, unsigned int align = 0) 
				: CAllocBase_<T>(size, align)
			{
				alloc();
				CAllocBase_<T>::addRef();
			}

			virtual ~CAlloc_() { dealloc(); }

		protected:
			virtual void alloc()
			{
				if ( 0 == CAllocBase_<T>::m_uiSize ) {
					CAllocBase_<T>::m_pData = new T[1];
					CAllocBase_<T>::m_pRaw 	= nullptr;
					return;
				}
				if ( CAllocBase_<T>::m_uiAlign < 2 ) {
					CAllocBase_<T>::m_pData = new T[CAllocBase_<T>::m_uiSize];
					CAllocBase_<T>::m_pRaw 	= nullptr;
				} else {
					CAllocBase_<T>::m_pRaw 	= new char[sizeof(T) * CAllocBase_<T>::m_uiSize + CAllocBase_<T>::m_uiAlign - 1];
					CAllocBase_<T>::m_pData = CAllocBase_<T>::alignPoint(static_cast<void *>(CAllocBase_<T>::m_pRaw));
				}
			}

			virtual void dealloc()
			{
				if ( nullptr == CAllocBase_<T>::m_pRaw ) {
					delete [] CAllocBase_<T>::m_pRaw;
				} else {
					delete [] CAllocBase_<T>::m_pData;
				}

				CAllocBase_<T>::m_pRaw 	= nullptr;
				CAllocBase_<T>::m_pData = nullptr;
			}

		private:
			CAlloc_ (const CAlloc_& src);
			CAlloc_ operator=(const CAlloc_& src);
	};


	template<class T, class A = CAlloc_<T>>
		class CMemory
		{
			public:
				static CMemory& null();

				CMemory();
				explicit CMemory(unsigned int size, unsigned int align=0);

				CMemory(const CMemory& src);
				CMemory& operator=(const CMemory& src);
				CMemory& operator=(const T& srcData);

				unsigned int 	size()			{ return m_pMemory->m_uiSize; }
				unsigned int 	ref()			{ return m_pMemory->m_uiRef; }
				bool			isNull()		{ return (m_pMemory == null().m_pMemory); }

				const T& 		operator[] (unsigned int index) const;
				T& 				operator[] (unsigned int index);

				virtual A* 		clone();
				void 			duplicate();

			private:
				A*		m_pMemory;
				static	CMemory* s_pNULL;
		};
}
#endif
