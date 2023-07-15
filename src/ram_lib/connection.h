#ifndef CONNECTION
#define CONNECTION
#define BUFFER_SIZE 4096

void *receive_thread(void *vargp);
void *send_thread(void *vargp);

#endif
