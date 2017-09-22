#include "mainwindow.h"
#include <QApplication>

#include "app_include.h"
#include <can_app.h>

pthread_rwlock_t  lock_canpool;
sem_t sem_display;
int fd_cansocket = 0;


static void *
DummyData(void *para)
{
    while (1)
    {
        sleep(SAMPLE_INTERVAL);

        Battery_1.voltage++;
        Battery_2.voltage++;
        Battery_1.temperature++;
        Battery_2.temperature++;
    }
}

int main(int argc, char *argv[])
{
    pthread_t tid_listen;
    pthread_t tid_poll;
    pthread_t tid000;
    uint16_t poll_second = 15;
    QApplication a(argc, argv);
    MainWindow w;

    pthread_rwlock_init(&lock_canpool, NULL);
    sem_init(&sem_display, 0, 0);
    
//    fd_cansocket = CAN_Init("can0", CAN_BITRATE);

    pthread_create(&tid_listen, NULL, &CAN_Listen, NULL);
    pthread_create(&tid_poll, NULL, &CAN_Poll, &poll_second);
    pthread_create(&tid000, NULL, &DummyData, NULL);

    w.show();

    sem_destroy(&sem_display);
    pthread_rwlock_destroy(&lock_canpool);
    pthread_cancel(tid_listen);
    pthread_cancel(tid_poll);

    return a.exec();
}

