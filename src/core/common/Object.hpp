#ifndef __OBJECTHPP__
#define __OBJECTHPP__

namespace NET
{	
	class Object
	{
		public:
			Object()
				: m_size(0)
				, m_data(NULL)
			{
				;
			}

			Object(Object& ob)
			{
				m_size 		= ob.size;
				m_data 		= ob.m_data;

				ob.release(false);
			}

			Object(const Object&) = delete;

			virtual ~Object()
			{
				release();
			}

			void* alloc(::std::size_t size, bool bForceDelete = true)
			{
				if ( m_data != NULL ) {
					if ( bForceDelete ) 
						delete m_data;
					else 
						return m_data;
				}
				
				m_data = new(size);
				m_size = size;

				return m_data;
			}

			void release(bool bDelete = true)		// be careful about memory leaks
			{
				m_size = 0;

				if ( bDelete && NULL != m_data ) {
					delete m_data;
				}
				m_data = NULL;		
			}

			void*			data() const 					{ return m_data; }
			void			setSize(::std::size_t size)		{ m_size = size; }
			::std::size_t 	size()							{ return m_size; }
	
		protected:
			::std::size_t 	m_size;
			void*			m_data;
	}
}
#endif
