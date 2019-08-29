#ifndef __NET_CBASEMESSAGE_HPP__
#define __NET_CBASEMESSAGE_HPP__

namespace OpenHMI
{
	class CBaseMessage : public CObject
	{
	public:// TODO: define const here

	private:// TODO: define const here

	public:// TODO: define embed class or struct or enum here
        virtual void handleMessage( CObject* data ) = 0;

	private:// TODO: define embed class or struct or enum here

	public:// TODO: define your public method here

	protected:// TODO: define your protected method here
		CBaseMessage();
		virtual ~CBaseMessage();

	private:// TODO: define your private method here
		
	protected:// property

	private:// property

	};
}

#endif  // __NET_CBASEMESSAGE_HPP__
