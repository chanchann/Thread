/*
https://blog.csdn.net/weixin_39094034/article/details/112434226
*/


#include <iostream>
#include <pthread.h>
using namespace std;
 
//一个用__thread关键字修饰的全局变量
__thread int g_iThreadCount = 0;
// thread_local int g_iThreadCount = 0;
void *pthreadFunc1(void *pArg) {
	g_iThreadCount += 1;
	cout << "pthreadFunc1::g_iThreadCount = " << g_iThreadCount << endl;
	pthread_exit((void *)1);
}
 
void *pthreadFunc2(void *pArg) {
	g_iThreadCount += 2;
	cout << "pthreadFunc2::g_iThreadCount = " << g_iThreadCount << endl;
	pthread_exit((void *)2);
}
 
int main(void) {
	int iRet;
	pthread_t pthreadId1;
	pthread_t pthreadId2;
	
	pthread_create(&pthreadId1, NULL, pthreadFunc1, NULL);
	pthread_create(&pthreadId2, NULL, pthreadFunc2, NULL);
	
	pthread_join(pthreadId1, NULL);
	pthread_join(pthreadId2, NULL);
	
	return 0;
}
