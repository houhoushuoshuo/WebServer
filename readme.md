环境：ubunut18.04、g++ 7.0
使用：
（1）
cd WebServer
sh build.sh
cd ../build/Debug/webServer
./WebServer -p 9999  【-p 指定端口，-l 指定log存储位置】
（2）
之后就可以在网页搜索栏输入127.0.0.1:9999进入投票系统了

特点：
整体使用Reactor模式，epoll的ET模式触发，且非阻塞io，server主线程只负责接受连接，然后发放到其他线程中。
利用多线程充分利用多核cup，支持高并发。
维护一个定时器序列用于关闭短连接。
使用了智能指针维护一些对象，利用RAII机制。
实现了异步日志，同时加入了双缓冲。
可以解析http请求，支持管线化。
可以利用eventfd进行异步唤醒线程。
后台使用redis存储账户密码和投票信息。

