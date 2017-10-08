#include "mainwindow.h"
#include <QApplication>

#include "app_include.h"
#include <can_app.h>


pthread_rwlock_t  lock_bat1, lock_bat2, lock_heater;
sem_t sem_display;
int fd_cansocket = 0;
pthread_t tid_listen;
pthread_t tid_poll;
pthread_t tid000;
pthread_t tid_ui;
uint16_t poll_second = 15;

static void *
DummyData(void *para)
{
    while (1)
    {
        sleep(SAMPLE_INTERVAL);

        pthread_rwlock_wrlock(&lock_bat1);
        Battery_1.voltage++;
        Battery_1.temperature++;
        pthread_rwlock_unlock(&lock_bat1);

        pthread_rwlock_wrlock(&lock_bat2);
        Battery_2.voltage++;
        Battery_2.temperature++;
        pthread_rwlock_unlock(&lock_bat2);
        printf("dummy");
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    pthread_rwlock_init(&lock_bat1, NULL);
    pthread_rwlock_init(&lock_bat2, NULL);
    pthread_rwlock_init(&lock_heater, NULL);
//    sem_init(&sem_display, 0, 0);
//    fd_cansocket = CAN_Init("can0", CAN_BITRATE);

    pthread_create(&tid_listen, NULL, &CAN_Listen, &w);
    pthread_create(&tid_poll, NULL, &CAN_Poll, &poll_second);
    pthread_create(&tid000, NULL, &DummyData, NULL);

    w.show();

//    sem_destroy(&sem_display);
//    pthread_cancel(tid_listen);
//    pthread_cancel(tid_poll);
//    pthread_cancel(tid000);

    return a.exec();
}

