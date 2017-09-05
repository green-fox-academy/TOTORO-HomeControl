#ifndef __THREAD_DEF_H
#define __THREAD_DEF_H


osThreadDef(PROJECTOR, projector_client_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
osThreadDef(AC, ac_client_thread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);

#endif /* __THREAD_DEF_H */