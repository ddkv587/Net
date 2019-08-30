#ifndef __NET_CMODELGROUP_HPP__
#define __NET_CMODELGROUP_HPP__

namespace NET
{
    class CModelGroup : public CBaseModel
    {
    public:// TODO: define const here

    private:// TODO: define const here

    public:// TODO: define embed class or struct or enum here

    private:// TODO: define embed class or struct or enum here

    public:// TODO: define your public method here
        CModelGroup();
        virtual ~CModelGroup();

    protected:// TODO: define your protected method here
        virtual CBaseModel*                 banace();
        
    private:// TODO: define your private method here
        
    protected:// property

    private:// property 
        SLIST<CBaseModel>                   m_lstModel;
    };
}

#endif  // __NET_CMODELGROUP_HPP__
