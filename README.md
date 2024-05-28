# muduo_c-11
用C++11重写muduo网络库的主要部件

## 开发环境

-   linux kernel version 5.15.0 (ubuntu 20.04 Server)

-   gcc version 9.4.0

-   cmake version 3.28.0

项目编译执行`./aotobuild.sh`，测试用例进入`example/`文件夹，`make clean`清除旧可执行文件，`make`生成新服务器测试用例。

## 功能介绍

  1. 使用C++11重写muduo库的主要部件，取消了对boost库的依赖。

  2. muduo重要组件：Event事件、Reactor反应堆、Demultiplex事件分发器、Eventhandler事件处理器。

  3. `muduo`采用`Reactor`模型和多线程结合的方式，实现了高并发非阻塞网络库。

  4. 用户创建一个main loop，主线程作为main reactor

  5. 给TcpServer设置连接和读写事件回调，TcpServer再给TcpConnection设置回调，这个回调是发生事件用户设置要执行的，TcpConnection再给channel设置回调，在发生事件时，会先执行这个回调，再执行用户设置的回调

  6. TcpServer根据用户设置传入的线程数，去pool中开启几个线程，如果没有设置，那么用户传入的main loop还要承担读写事件的任务。

  7. 当有新连接进来时，创建一个实例对象，然后由Acceptor去轮询唤醒一个sub reactor给它服务

  8. 同时，每个sub reactor在服务时，其所包含的那个Poller如果没有事件就会处于循环阻塞状态，发生事件之后，根据类型再去执行响应的回调操作

## 基本框架

![image](https://github.com/qwert123q/muduo_c-11/assets/53610632/02e93ac2-5e06-4b8b-93a6-8d1464d0f227)


## 处理流程

![image](https://github.com/qwert123q/muduo_c-11/assets/53610632/f513a826-c089-47ee-92d6-6d914d3c3822)
