#include "NET_Core.hpp"

CModuleManager* 	CModuleManager::s_pInstance = nullptr;

namespace NET
{
    CModuleManager* CModuleManager::getInstance()
    {
        if ( nullptr == s_pInstance ) {
            s_pInstance = new CModuleManager();
        }
        return s_pInstance;
    }

     CModuleManager::CModuleManager()
     {
         ;
     }

    CModuleManager::~CModuleManager()
    {
        ::std::lock_guard<SMUTEX> lock(m_mutex);
        
        m_mapModules.clear();
    } 

    void CModuleManager::registerModule(CBaseModule* pModule)
    {
        if ( pModule != nullptr && !isExist(pModule->strName) ) {
            ::std::lock_guard<SMUTEX> lock(m_mutex);

            m_mapModules.insert( ::std::pair<STRING, CBaseModule*>( pModule->name(), pModule ) );
        }
    }

    void CModuleManager::removeModule(STRING strName)
    {
        ::std::lock_guard<SMUTEX> lock(m_mutex);

        m_mapModules.erase(strName);
    }

    void CModuleManager::removeModule(CBaseModule* pModule)
    {
        CHECK(NULL != pModule);
        ::std::lock_guard<SMUTEX> lock(m_mutex);

        for ( auto it = m_mapModules.begin(); it != m_mapModules.end(); ) {
            if ( pModule == it->second ) {
                it = c.erase(it);
            } else {
                ++it;
            }
        }
    } 

    BOOLEAN CModuleManager::isExist(STRING strName)
    {
        ::std::lock_guard<SMUTEX> lock(m_mutex);

        return m_mapModules.contains(strName);
    }

    BOOLEAN CModuleManager::isExist(CBaseModule* pModule)
    {
        CHECK_R(NULL != pModule, FALSE);        
        ::std::lock_guard<SMUTEX> lock(m_mutex);
        
        for ( auto it = m_mapModules.begin(); it != m_mapModules.end(); ++it ) {
            if ( pModule == it->second ) {
                return TRUE;
            }
        }
    }

    CBaseModule* CModuleManager::getModule(STRING strName)
    {
        ::std::lock_guard<SMUTEX> lock(m_mutex);

        auto find = m_mapModules.find(strName);
        if ( find != m_mapModules.end() ) {
            return find->second;
        } else {
            return nullptr;
        } 
    } 
}