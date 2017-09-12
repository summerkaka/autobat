#include "mainwindow.h"
#include <QApplication>

#include "app_include.h"

pthread_rwlock_t  lock_canpool;
sem_t sem_display;
int fd_cansocket = 0;

int main(int argc, char *argv[])
{
    pthread_t tid_listen;
    pthread_t tid_poll;
    pthread_t tid_display;
    uint8_t poll_ms = 100;
    QApplication a(argc, argv);
    MainWindow w;
    int ret = -1;

    pthread_rwlock_init(&lock_canpool, NULL);
    ret = sem_init(&sem_display, 0, 0);
    
    // create socketcan thread
    fd_cansocket = CAN_Init("can0", CAN_BITRATE);

    pthread_create(&tid_listen, NULL, &CAN_Listen, NULL);
    pthread_create(&tid_poll, NULL, &CAN_Poll, &poll_ms);
    pthread_create(&tid_display, NULL, &UpdateUi, NULL);

    w.show();

    sem_destroy(&sem_display);
    pthread_rwlock_destroy(&lock_canpool);
    pthread_cancel(tid_listen);
    pthread_cancel(tid_poll);

    return a.exec();
}

