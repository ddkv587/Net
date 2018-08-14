#ifndef __CBUFFERHPP__
#define __CBUFFERHPP__

namespace NET
{
	template <class T = CHAR>
	class CBuffer
	{
		public:

		public:
			CBuffer(UINT uiSize, UINT uiAlign = sizeof(T))
				: m_pSource(nullptr)
				, m_uiRef(0)
				, m_uiTotalSize(uiSize)
				, m_uiFreeSize(uiSize)
			{
				m_pRaw 		= new CHAR[sizeof(T) * uiSize];
				m_pSource 	= static_cast<T*>(m_pRaw); 
			}

			virtual ~CBuffer()
			{
				if ( nullptr != m_pRaw ) {
					delete [] m_pRaw;
					m_pRaw 				= nullptr;
					m_pSource 			= nullptr;
					m_uiRef				= 0;
					m_uiTotalSize		= 0;
					m_uiFreeSize		= 0;
				}
			}

			UINT			size()								{ return m_uiTotalSize - m_uiFreeSize; }
			UINT			available()							{ return m_uiFreeSize; }
			UINT			total()								{ return m_uiTotalSize; }
			UINT			ref()								{ return m_uiRef; }
			UINT			align()								{ return m_uiAlign; }
			CHAR*			raw()								{ return m_pRaw; }

			void			addRef()							{ ++m_uiRef; }
			void 			subRef()		
			{
				--m_uiRef; 
				if ( 0 == m_uiRef ) delete this;
			}

			UINT			write(const T* pData, UINT uiSize);
			UINT			writeChar(const CHAR* pChar, UINT uiSize);

			T*				operator->()						{ return m_pSource; }
			explicit		operator T*()						{ return m_pSource; }

		protected:
			CBuffer(CBuffer&) = delete;
			CBuffer(const CBuffer&) = delete;

		private:

		private:
			T*				m_pSource;
			CHAR*			m_pRaw;
			
			UINT			m_uiRef;
			UINT			m_uiAlign;

			UINT			m_uiTotalSize;
			UINT			m_uiFreeSize;
	};
}
#endif
