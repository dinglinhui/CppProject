#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

pthread_mutex_t mutex;
void print(char* buf) {
	int i;
	pthread_mutex_lock(&mutex);
	for (i = 0; i < strlen(buf); i++) {
		printf("I am in print buf[%d] is %c\r\n", i, buf[i]);

	}
	pthread_mutex_unlock(&mutex);
}

void *thread1(void* arg) {
	char *buf;
	int i;
	pthread_mutex_lock(&mutex);
	buf = (char*) arg;
	printf("before in thread1\r\n");
	sleep(1);
	print(buf);
	for (i = 0; i < strlen(buf); i++) {
		printf("I am from thread1  buf[%d] is %c\r\n", i, buf[i]);
		usleep(100);
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void *thread2(void* arg) {
	char *buf;
	int i;
	printf("before in thread 2\r\n");
	pthread_mutex_lock(&mutex);
	buf = (char*) arg;
	sleep(1);
	for (i = 0; i < strlen(buf); i++) {
		printf("I am from thread2  buf[%d] is %c\r\n", i, buf[i]);
		usleep(100);
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main() {
	pthread_t pid1, pid2;
	char buf[] = "abcdefg";
	pthread_create(&pid1, NULL, thread1, (void*) buf);
	usleep(100);
	pthread_create(&pid2, NULL, thread2, (void*) buf);

	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
	return 0;
}
