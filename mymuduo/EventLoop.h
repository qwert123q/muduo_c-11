#pragma once

#include <functional>
#include <vector>
#include <atomic>
#include <memory>
#include <mutex>

#include "noncopyable.h"
#include "Timestamp.h"
#include "CurrentThread.h"

class Channel;
class Poller;

class EventLoop:noncopyable
{
public:
    using Functor=std::function<void()>;

    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    Timestamp  pollRetuenTime() const {return pollReturnTime_;}

    void runInLoop(Functor cb);
    void queueInLoop(Functor cb);

    void wakeup();

    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel *channel);

    // 判断EventLoop对象是否在自己的线程里面
    bool isInLoopThread() const { return threadId_ ==  CurrentThread::tid(); }

private:
    void handleRead();
    void doPendingFunctors();

    using ChannelList=std::vector<Channel*>;

     std::atomic_bool looping_;  // 原子操作，通过CAS实现的
    std::atomic_bool quit_; // 标识退出loop循环
    
    const pid_t threadId_; // 记录当前loop所在线程的id

    Timestamp pollReturnTime_; // poller返回发生事件的channels的时间点
    std::unique_ptr<Poller> poller_;

    int wakeupFd_; 
    std::unique_ptr<Channel> wakeupChannel_;

    ChannelList activeChannels_;

    std::atomic_bool callingPendingFunctors_;
    std::vector<Functor> pendingFunctors_; 
    std::mutex mutex_; 
};