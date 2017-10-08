#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

extern pthread_t tid_listen;
extern pthread_t tid_poll;
extern pthread_t tid000;
extern pthread_t tid_ui;
extern pthread_rwlock_t lock_bat1, lock_bat2, lock_heater;

#ifdef __cplusplus
}
#endif

#endif // MAIN_H
