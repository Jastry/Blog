#include <stdio.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


#if 0 //volatile
/* 全局变量加 g_ 为了提高可读性，是代码规范，但全局变量尽量不要使用 */
//int volatile g_flag = 0;
int g_flag = 0;

void handler(int signum) {
	//void(signum);
	g_flag++;
	printf("g_flag = %d\n", g_flag);
}

int main() {
	/* catch 2 signal*/
	signal(SIGINT, handler);
	while (g_flag == 0) {
	}
	return 0;
}

#endif


#if 1
void hander(int signum) { /*	do nothing	*/ }

void MySleep(int second) {
	//	1. 使用 sigaction,注册一个 SIGALRM 处理函数
	struct sigaction new_act, old_act;

	new_act.sa_handler = hander;
	sigemptyset(&new_act.sa_mask);
	new_act.sa_flags = 0;

	sigaction(SIGALRM, &new_act, &old_act);

	//	2. 调用 alarm 函数，N 秒后，系统会自动发送 SIGALRM 函数
	alarm(second);

	//	3. 调用 pause 函数
	pause();
	//	4. 扫尾工作
	sigaction(SIGALRM, &old_act, &new_act);
	int ret = alarm(0);
    
}

int main(int argc, char const *argv[])
{
	/* code */
	while (1) {
		printf("hello world\n");
		MySleep(1);
	}
	return 0;
}
#endif


#if 0 
void hander(int signum) {
	printf("sig = %d\n", signum);
}

void MyPrint(sigset_t* set) {
	int i = 1;
	for (; i <= 31; ++i) {
        if (sigismember(set, i)) {
			printf("1 ");
        }
        else {
		   printf("0 ");
        }
	}
}

int main(int argc, char const *argv[])
{
	signal(SIGINT, hander);
	/*
	 *	how 表示用什么方式修改
	 *	
	 */

	sigset_t new_set;	
	sigset_t old_set;	//以前的信号处理方式
	sigemptyset(&new_set);

	sigaddset(&new_set, SIGINT);	//SIGINT -> ctrl c

	sigprocmask(SIG_BLOCK, &new_set, &old_set);
	

	while (1) {
		sleep(1);		
		MyPrint(&new_set);
        printf("\n");
	}
	return 0;
}
#endif
#if 1

void hander(int signum) {
    (void)signum;
    printf("%d\n", signum);
}

int main() {
    //signal(SIGINT, hander);
    sleep(2);
    //raise(SIGINT);
    abort();

}
#endif
#if 0

//mykill 函数
void hander(int signum) {
	printf("signum is %d\n", signum);
}

int main(int argc, char const *argv[])
{
	if (argc < 3) {
		printf("usage : %s [signum] [pid]", argv[0]);
	}

	/*
	 *	C 语言将字符串转换成整型
	 *	1. atoi() ~ ascii -> integer atol() atoul() 
	 *  2. sprintf();   整数写入字符串中
	 * 	3. sscanf();
	 *	4. stringstream 
	 *	5. boost库	
	 */
	int signum = atoi(argv[1]);
	int pid = atoi(argv[2]);
	int ret = kill(pid, signum);
	if (ret < 0) {
		perror("kill");
		return 1;
	}
	return 0;
}

#endif


#if 0

int g_value = 0;

void hander(int signum) {
    (void)signum;
    printf("%d\n", g_value);
}

int main() {
    signal(SIGALRM, hander);
    alarm(1);
    while (1) {
        g_value++;
    }

    return 0;
}

#endif

#if 0
void hander(int signum) {
	printf("signum is %d\n", signum);
    exit(1);
}

int test() {
    int a = 0;
    int b = 1;
    printf("我是除数为零的异常\n");
    return (a / b);
}

void test2() {
    int *ptr = NULL;
    *ptr = 3;
}

int main(int argc, char const *argv[])
{
	/*	设置新的处理方式使用 signalhander_t 返回值是原来的处理方式
	 *	每一个进程都有自己处理信号的处理方式
	 */
	int signum = 1;
    for (; signum < 32; ++ signum)
        signal(signum, hander);
    
    //test();
    //while (1) {
    //    sleep(1);
    //}
    int a = 0;
    int b = 20;
    int c = b / a;
    //test2();
    printf("%d\n",c);
	return 0;
}

#endif
