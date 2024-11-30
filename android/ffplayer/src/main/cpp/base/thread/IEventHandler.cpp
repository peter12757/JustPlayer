//
// Created by Peter Xi on 2022/6/16.
//

#include "IEventHandler.h"

int IEventHandler::addCall(IBaseEvent* pEvt)
{
    IBaseEvent* dup = pEvt->createDup();
    if (dup == nullptr)
    {
        LOGW("addCall failed evt: %u", pEvt->logStr().c_str());
        return -1;
    }
    MutexStackLock lock(m_queuelock);
    m_queue.push_back(dup);

    return 0;
}

void IEventHandler::clearCall()
{
    MutexStackLock lock(m_queuelock);
//  for (std::deque<IBaseEvent*>::iterator it = m_queue.begin(); it != m_queue.end(); ++it)
    for (auto it : m_queue)
    {
        it->deleteDup();
    }
    LOGI("clearCall clear m_queue size %u", m_queue.size());
    m_queue.clear();
}

bool IEventHandler::isCallEmpty()
{
    MutexStackLock lock(m_queuelock);
    return m_queue.empty();
}

void IEventHandler::onEventRun()
{
    std::deque<IBaseEvent*> linkCall;
    {
        MutexStackLock lock(m_queuelock);
        linkCall.swap(m_queue);
        m_queue.clear();
    }

    for (auto it : linkCall)
    {
        onCall(it);
        it->deleteDup();
    }
}

void IEventHandler::onCall(IBaseEvent* pEvt)
{
    if (!pEvt)
    {
        LOGW("onCall failed null");
        return;
    }

    handler_map::iterator it = m_handlers.find(pEvt->m_type);

    if (it != m_handlers.end())
    {
        (this->*it->second)(pEvt);
    }
    else
    {
        LOGI("failed to find request handler for %u", pEvt->m_type);
    }
}