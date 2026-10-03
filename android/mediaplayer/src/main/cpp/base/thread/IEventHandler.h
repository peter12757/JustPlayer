//
// Created by Peter Xi on 2022/6/16.
//

#ifndef ANDROIDTEST_IEVENTHANDLER_H
#define ANDROIDTEST_IEVENTHANDLER_H

#include <string>
#include <map>
#include <deque>
#include "../util/Lock.h"
#include "../log/LogUtil.h"
using namespace Lock;

class IBaseEvent
{
public:
    IBaseEvent(uint32_t type)
    :m_type(type)
    {}
    virtual ~IBaseEvent() {}

    // createDup - 分配一个新的对象，内容和当前的对象一样，同时设置 m_bDup 为 true
    // deleteDup - 释放createDup分配出来的对象
    virtual IBaseEvent* createDup() { return nullptr; }
    virtual void deleteDup() {}
    std::string logStr(){ return "no Impl";}

    uint32_t m_type;
};

class IEventHandler {
public:
    IEventHandler(){};
    virtual ~IEventHandler() {};

public:
    int addCall(IBaseEvent* pEvt);
    void clearCall();
    bool isCallEmpty();
    void onEventRun();

private:
    void onCall(IBaseEvent* pEvt);

private:
    Lock::MediaMutex m_queuelock; // 锁m_queue
    //线程调用的handler的queue
    std::deque<IBaseEvent*> m_queue;

    //回调的handler
    typedef void (IEventHandler::*event_handler)(IBaseEvent*);
    typedef std::map<uint32_t, event_handler> handler_map;
    handler_map m_handlers;



};


#endif //ANDROIDTEST_IEVENTHANDLER_H
