#!/usr/bin/python
# -*- coding: UTF-8 -*-
# 文件名：server.py

import socket

s = socket.socket()         # 创建 socket 对象
host = socket.gethostname() # 获取本地主机名
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR,1)
host = "192.168.209.128"
port = 8888                # 设置端口
s.bind((host, port))        # 绑定端口
print "Listening on %s:%d" % (host, port)
s.listen(5)                 # 等待客户端连接
while True:
    c, addr = s.accept()     # 建立客户端连接。
    print '连接地址：', addr
    #c.send('欢迎访问菜鸟教程！')
    while True:
        ping = c.recv(1024)
        print ping
        if ping == 'quit' or ping == "" or ping == -1:
            c.send("Bye Bye")
            print "关闭连接:" + ping
            c.close()                # 关闭连接
            break
        c.send(ping)
