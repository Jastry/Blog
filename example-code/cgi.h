#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/stat.h>
#include "processpool.h"

class cgi_conn {
public:
    cgi_conn(){}
    ~cgi_conn(){}

    /* 初始化客户端连接，清空读缓冲区 */
    void init( int epollfd, int sockfd, const struct sockaddr_in& client )
    {
        m_epollfd = epollfd;
        m_sockfd = sockfd;
        m_address = client;
        memset(m_buf, '\0', sizeof(m_buf));
        m_read_idx = 0;
    }

    void process()
    {
        int idx = 0;
        int ret = -1;
        /* 循环读取和分析客户数据 */
        while (true) {
            idx = m_read_idx;
            ret = recv(m_sockfd, m_buf + idx, sizeof(m_buf), 0);
            if (ret < 0) {
                if (errno != EAGAIN) {
                    removefd(m_epollfd, m_sockfd);
                }
                break;
            }
            /* 如果对方关闭连接，则服务器也关闭连接 */
            else if (ret == 0) {
               removefd( m_epollfd, m_sockfd );
               break;
            } else {
                m_read_idx += ret;
                printf("user content is %s\n", m_buf);
                /* 如果遇到字符 "\r\n",则开始处理客户请求 */
                for (; idx < m_read_idx; ++idx) {
                    if ( (idx >= 1) && (m_buf[idx - 1] == '\r') && (m_buf[idx] == '\n') ) {
                        break;
                    }
                }
                /* 防止发生粘包问题，如果没有读到 "\r\n" 表示需要读取更多的客户数据 */
                if ( idx == m_read_idx ) {
                    continue;
                }
                m_buf[idx - 1] = '\0';
                
                char * file_name = m_buf;

                /* 判断客户要运行的程序是否存在 */
                if ( access( file_name, F_OK ) == -1 ) {
                    removefd( m_epollfd, m_sockfd );
                    break;
                }

                /* 创建子进程来执行 cgi 程序 */
                ret = fork();
                if (ret == -1) {
                    removefd( m_epollfd, m_sockfd );
                    break;
                } else if (ret > 0) {
                    //father
                    removefd( m_epollfd, m_sockfd );
                    break;
                } else {
                    //child 将标准输出重定向到 m_sockfd，并执行 cgi 服务
                    close(STDOUT_FILENO);
                    dup( m_sockfd );
                    execl( m_buf, m_buf, NULL );
                    exit(0);
                }
            }
        }
    }

private:
    /* 读缓冲区大小 */
    static const int BUFFER_SIZE = 1024;
    static int m_epollfd;
    static int m_sockfd;
    sockaddr_in m_address;
    char m_buf[BUFFER_SIZE];

    /* 标记读缓冲中已经读入客户数据的最后一个字节的下一个位置 */
    int m_read_idx;
};
