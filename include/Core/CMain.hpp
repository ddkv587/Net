#ifndef __NET_CMAIN_HPP__
#define __NET_CMAIN_HPP__

namespace NET
{
    class CMain final : public CThread 
    {
    public:// TODO: define const here

    private:// TODO: define const here

    public:// TODO: define embed class or struct or enum here

    private:// TODO: define embed class or struct or enum here

    public:// TODO: define your public method here
        static CMain*                   getInstance();
        static void                     delInstance();

        void                            initialize();
        void                            uninitialize();
        BOOLEAN                         isInitialized()         { return m_bInitialized; }

        void                            pushMessage( CBaseMessage* );
        CBaseMessage*                   popMessage();

        // ================== module =======================
        void                            registerModule( CBaseModule* pModule, STRING strName = STRING_NULL );
        
    protected:// TODO: define your protected method here
        CMain();
        ~CMain();

    private:// TODO: define your private method here
        void                            mainLoop( void* arg );

        void                            operator delete( void* ) = delete;
        void                            operator delete[]( void* ) = delete;
        
    protected:// property

    private:// property
        static CMain*                       s_pInstance;

        BOOLEAN                             m_bInitialized;

        SMAP< STRING, CBaseModule >         m_mapModules;

        SQUEUE< CBaseMessage >              m_sysMessage;
        SQUEUE< CBaseMessage >              m_userMessage;
        SQUEUE< CBaseMessage >              m_timerMessage;
    };
}

#endif  // __NET_CXXX_HPP__
