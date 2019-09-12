#ifndef __NET_CBASEMODEL_HPP__
#define __NET_CBASEMODEL_HPP__

namespace NET
{
    class CBaseModel : public CThread
    {
    public:// TODO: define const here

    private:// TODO: define const here

    public:// TODO: define embed class or struct or enum here
        
    private:// TODO: define embed class or struct or enum here

    public:// TODO: define your public method here
        // ================ load ======================
        virtual BOOLEAN         load() = 0;
        virtual void            unload() = 0;

        // ============== life circle ===================
        virtual void            start() = 0;
        virtual void            stop( BOOLEAN bForce = FALSE ) = 0;
        virtual void            pause( BOOLEAN bForce = FALSE ) = 0;
        virtual void            resume() = 0;
        
    protected:// TODO: define your protected method here
        CBaseModel();
        virtual ~CBaseModel();

    private:// TODO: define your private method here
        
    protected:// property

    private:// property

    };
}

#endif  // __NET_CBASEMODEL_HPP__
