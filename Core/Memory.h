#ifndef __MEMORY_H__
#define __MEMORY_H__

template<class T> class CAllocBase_
{
	public:
		CAllocBase_(unsigned int size, unsigned int align)
			: m_pData(NULL), m_uiSize(0), m_uiRef(0), m_uiAlign(0) 
		{}
		virtual ~CAllocBase_(){}

		operator		T* () { return m_pData; }
		operator const 	T* () const { return m_pData; }

		T* alignPoint(void* raw)
		{
			T *p = reinterpet_cast<T*> (
					(reinterpet_cast<uintptr_t>(raw) + m_uiAlign - 1)
					& ~(m_uiAlign - 1)
					);
			return p;
		}

		inline unsigned int size() 	{ return m_uiSize; }
		inline unsigned int ref()	{ return m_uiRef; }
		inline unsigned int align()	{ return m_uiAlign; }

		inline void addRef() 		{ ++m_uiRef; }
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

	private:
		T*		m_pData;
		void*	m_pRaw;
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
			addRef();
		}

		virtual ~CAlloc_() { dealloc(); }

	protected:
		virtual void alloc()
		{
			if ( 0 == m_uiSize ) {
				m_pData = new T[1];
				m_pRaw 	= NULL;
				return;
			}
			if ( m_uiAlign < 2 ) {
				m_pData = new T[m_uiSize];
				m_pRaw 	= NULL;
			} else {
				m_pRaw 	= new void[sizeof(T) * m_uiSize + m_uiAlign - 1];
				m_pData = alignPoint(m_pRaw);
			}
		}

		virtual void dealloc()
		{
			if ( NULL == m_pRaw ) {
				delete [] m_pRaw;
			} else {
				delete [] m_pData;
			}

			m_pRaw 	= NULL;
			m_pData = NULL;
		}

		virtual void dealloc();

	private:
		CAlloc_ (const CAlloc_& src);
		CAlloc_ operator=(const CAlloc_& src);
};

#endif
