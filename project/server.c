// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <pthread.h>
// #include <mqueue.h>
// #include <fcntl.h>
// #include <time.h>
// #include <arpa/inet.h>
// #include "mqtt.h"  // include MQTT publish function

// #define PORT 8080
// #define MQ_NAME "/mqtt_queue"
// #define MQ_MAXMSG 10
// #define MQ_MSGSIZE 1024
// pthread_mutex_t mqtt_mutex = PTHREAD_MUTEX_INITIALIZER;


// void* client_handler(void* arg) {
//     int sock = *(int*)arg;
//     free(arg);

//     char buffer[BUFFER_SIZE];

//     int read_size = recv(sock, buffer, BUFFER_SIZE - 1, 0);
//     if (read_size > 0) {
//         buffer[read_size] = '\0';
//         printf("Server received: %s\n", buffer);

//         // // Protect MQTT publish with mutex
//         pthread_mutex_lock(&mqtt_mutex);
//         mqtt_publish(buffer);
//         pthread_mutex_unlock(&mqtt_mutex);


//         buffer[read_size] = '\0';
//         log_message("Received", buffer, 0);

//         // Push to POSIX message queue with default priority 1
//         mqd_t mq = mq_open(MQ_NAME, O_WRONLY);
//         if (mq == (mqd_t)-1) {
//             perror("mq_open failed in client_handler");
//         } else {
//             if (mq_send(mq, buffer, strlen(buffer), 1) < 0) {
//                 perror("mq_send failed");
//             }
//             mq_close(mq);
//         }
//     }

//     close(sock);
//     return NULL;
// }


// void log_message(const char* prefix, const char* msg, int priority) {
//     time_t now = time(NULL);
//     char ts[64];
//     strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", localtime(&now));
//     printf("[%s] %s (priority %d) -> %s\n", ts, prefix, priority, msg);
// }

// void* mqtt_worker(void* arg) {
//     mqd_t mq = *((mqd_t*)arg);
//     char buffer[MQ_MSGSIZE];
//     unsigned int prio;

//     while (1) {
//         ssize_t bytes = mq_receive(mq, buffer, MQ_MSGSIZE, &prio);
//         if (bytes >= 0) {
//             buffer[bytes] = '\0';
//             log_message("Publishing", buffer, prio);
//             mqtt_publish(buffer);  // Your persistent MQTT client
//         } else {
//             perror("mq_receive failed");
//         }
//     }
//     return NULL;
// }

// int main() {

//     if (mqtt_init() != 0) {
//         exit(EXIT_FAILURE);
//     }
//     struct mq_attr attr;
//     attr.mq_flags = 0;
//     attr.mq_maxmsg = MQ_MAXMSG;
//     attr.mq_msgsize = MQ_MSGSIZE;
//     attr.mq_curmsgs = 0;
//     mqd_t mq = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0644, &attr);
//     if (mq == (mqd_t)-1) {
//         perror("mq_open failed");
//         exit(EXIT_FAILURE);
//     }

//     // pthread_t mqtt_tid;
//     // pthread_create(&mqtt_tid, NULL, mqtt_worker, &mq);
//     // pthread_detach(mqtt_tid); 

//     int server_fd, new_socket;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);
//     char buffer[BUFFER_SIZE] = {0};

//     server_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (server_fd == 0) {
//         perror("socket failed");
//         exit(EXIT_FAILURE);
//     }

//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);

//     if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
//         perror("bind failed");
//         exit(EXIT_FAILURE);
//     }

//     if (listen(server_fd, 5) < 0) {
//         perror("listen failed");
//         exit(EXIT_FAILURE);
//     }

//     printf("Server listening on port %d...\n", PORT);

//     //  this below while loop for without thread 
//     // while (1) {
//     //     new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
//     //     if (new_socket < 0) {
//     //         perror("accept failed");
//     //         continue;
//     //     }

//     //     int read_size = recv(new_socket, buffer, BUFFER_SIZE - 1, 0);
//     //     if (read_size > 0) {
//     //         buffer[read_size] = '\0';
//     //         printf("Server received: %s\n", buffer);

//     //         // Publish to MQTT
//     //         mqtt_publish(buffer);
//     //     }

//     //     close(new_socket);
//     // }

//     //  this below while loop for thread when multiple clinet want to connect 
//     while (1) {
//         int* new_sock = malloc(sizeof(int));
//         *new_sock = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
//         if (*new_sock < 0) {
//             perror("accept failed");
//             free(new_sock);
//             continue;
//         }

//         pthread_t tid;
//         pthread_create(&tid, NULL, client_handler, new_sock);
//         pthread_detach(tid);  // Auto clean thread resources
//     }
//     mq_close(mq);
//     mq_unlink(MQ_NAME);
//     // Cleanup MQTT on exit (never reached in this example)
//     MQTTClient_disconnect(mqtt_client, 10000);
//     MQTTClient_destroy(&mqtt_client);

//     return 0;
// }
























// // server.c
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <pthread.h>
// #include <mqueue.h>
// #include <fcntl.h>
// #include <time.h>
// #include <arpa/inet.h>
// #include "mqtt.h"  // Your mqtt_publish and mqtt_init functions

// #define PORT 8080
// #define MQ_NAME "/mqtt_queue"
// #define MQ_MAXMSG 50
// #define MQ_MSGSIZE 1024

// // ----------------------
// // Logging function
// // ----------------------
// void log_message(const char* prefix, const char* msg, int priority) {
//     time_t now = time(NULL);
//     char ts[64];
//     strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", localtime(&now));
//     printf("[%s] %s (priority %d) -> %s\n", ts, prefix, priority, msg);
// }

// // ----------------------
// // MQTT Worker Thread
// // ----------------------
// void* mqtt_worker(void* arg) {
//     mqd_t mq = *((mqd_t*)arg);
//     char buffer[MQ_MSGSIZE];
//     unsigned int prio;

//     while (1) {
//         ssize_t bytes = mq_receive(mq, buffer, MQ_MSGSIZE, &prio);
//         if (bytes >= 0) {
//             buffer[bytes] = '\0';
//             log_message("Publishing", buffer, prio);
//             mqtt_publish(buffer); // Publish to MQTT broker
//         } else {
//             perror("mq_receive failed");
//             usleep(100000); // Sleep 100ms to avoid busy loop
//         }
//     }
//     return NULL;
// }

// // ----------------------
// // TCP Client Handler
// // ----------------------
// void* client_handler(void* arg) {
//     int sock = *(int*)arg;
//     free(arg);

//     char buffer[MQ_MSGSIZE];

//     int read_size = recv(sock, buffer, MQ_MSGSIZE - 1, 0);
//     if (read_size > 0) {
//         buffer[read_size] = '\0';
//         log_message("Received", buffer, 0);

//         // Push to POSIX message queue with default priority 1
//         mqd_t mq = mq_open(MQ_NAME, O_WRONLY);
//         if (mq == (mqd_t)-1) {
//             perror("mq_open failed in client_handler");
//         } else {
//             if (mq_send(mq, buffer, strlen(buffer), 1) < 0) {
//                 perror("mq_send failed");
//             }
//             mq_close(mq);
//         }
//     }

//     close(sock);
//     return NULL;
// }

// // ----------------------
// // Main Server
// // ----------------------
// int main() {

//     // Initialize MQTT
//     if (mqtt_init() != 0) {
//         exit(EXIT_FAILURE);
//     }

//     // Create POSIX message queue
//     struct mq_attr attr;
//     attr.mq_flags = 0;
//     attr.mq_maxmsg = MQ_MAXMSG;
//     attr.mq_msgsize = MQ_MSGSIZE;
//     attr.mq_curmsgs = 0;
//     mqd_t mq = mq_open(MQ_NAME, O_CREAT | O_RDWR, 0644, &attr);
//     if (mq == (mqd_t)-1) {
//         perror("mq_open failed");
//         exit(EXIT_FAILURE);
//     }

//     // Start MQTT worker thread
//     pthread_t mqtt_tid;
//     pthread_create(&mqtt_tid, NULL, mqtt_worker, &mq);
//     pthread_detach(mqtt_tid);

//     // Setup TCP server
//     int server_fd;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);

//     server_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (server_fd == 0) { perror("socket failed"); exit(EXIT_FAILURE); }

//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);

//     if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) { perror("bind failed"); exit(EXIT_FAILURE); }
//     if (listen(server_fd, 10) < 0) { perror("listen failed"); exit(EXIT_FAILURE); }

//     printf("Server listening on port %d...\n", PORT);

//     // Accept multiple clients in loop
//     while (1) {
//         int* new_sock = malloc(sizeof(int));
//         *new_sock = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
//         if (*new_sock < 0) {
//             perror("accept failed");
//             free(new_sock);
//             continue;
//         }

//         pthread_t tid;
//         pthread_create(&tid, NULL, client_handler, new_sock);
//         pthread_detach(tid); // Auto clean thread resources
//     }

//     // Cleanup (never reached in this example)
//     mq_close(mq);
//     mq_unlink(MQ_NAME);
//     MQTTClient_disconnect(mqtt_client, 10000);
//     MQTTClient_destroy(&mqtt_client);

//     return 0;
// }






// // server.c
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <pthread.h>
// #include <arpa/inet.h>
// #include <queue>

// #include "mqtt.h"  // Your mqtt_publish and mqtt_init functions

// #define PORT 8080
// #define MQ_MSGSIZE 1024

// // ----------------------
// // Logging function
// // ----------------------
// void log_message(const char* prefix, const char* msg, int priority) {
//     time_t now = time(NULL);
//     char ts[64];
//     strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", localtime(&now));
//     printf("[%s] %s (priority %d) -> %s\n", ts, prefix, priority, msg);
// }

// // ----------------------
// // Thread-safe message queue
// // ----------------------
// std::queue<std::string> msg_queue;
// pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t queue_cv = PTHREAD_COND_INITIALIZER;

// // ----------------------
// // MQTT Worker Thread
// // ----------------------
// void* mqtt_worker(void* arg) {
//     while (1) {
//         pthread_mutex_lock(&queue_mutex);
//         while (msg_queue.empty()) {
//             pthread_cond_wait(&queue_cv, &queue_mutex);
//         }
//         std::string msg = msg_queue.front();
//         msg_queue.pop();
//         pthread_mutex_unlock(&queue_mutex);

//         log_message("Publishing", msg.c_str(), 1);
//         mqtt_publish(msg.c_str()); // Publish to MQTT broker
//     }
//     return NULL;
// }

// // ----------------------
// // TCP Client Handler
// // ----------------------
// void* client_handler(void* arg) {
//     int sock = *(int*)arg;
//     free(arg);

//     char buffer[MQ_MSGSIZE];

//     int read_size = recv(sock, buffer, MQ_MSGSIZE - 1, 0);
//     if (read_size > 0) {
//         buffer[read_size] = '\0';
//         log_message("Received", buffer, 0);

//         // Push message to in-memory queue
//         pthread_mutex_lock(&queue_mutex);
//         msg_queue.push(std::string(buffer));
//         pthread_cond_signal(&queue_cv);
//         pthread_mutex_unlock(&queue_mutex);
//     }

//     close(sock);
//     return NULL;
// }

// // ----------------------
// // Main Server
// // ----------------------
// int main() {

//     // Initialize MQTT
//     if (mqtt_init() != 0) {
//         exit(EXIT_FAILURE);
//     }

//     // Start MQTT worker thread
//     pthread_t mqtt_tid;
//     pthread_create(&mqtt_tid, NULL, mqtt_worker, NULL);
//     pthread_detach(mqtt_tid);

//     // Setup TCP server
//     int server_fd;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);

//     server_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (server_fd == 0) { perror("socket failed"); exit(EXIT_FAILURE); }

//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);

//     if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) { perror("bind failed"); exit(EXIT_FAILURE); }
//     if (listen(server_fd, 10) < 0) { perror("listen failed"); exit(EXIT_FAILURE); }

//     printf("Server listening on port %d...\n", PORT);

//     // Accept multiple clients in loop
//     while (1) {
//         int* new_sock = (int*)malloc(sizeof(int));
//         *new_sock = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
//         if (*new_sock < 0) {
//             perror("accept failed");
//             free(new_sock);
//             continue;
//         }

//         pthread_t tid;
//         pthread_create(&tid, NULL, client_handler, new_sock);
//         pthread_detach(tid);
//     }

//     // Cleanup (never reached)
//     MQTTClient_disconnect(mqtt_client, 10000);
//     MQTTClient_destroy(&mqtt_client);

//     return 0;
// }










// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "mqtt.h"  // mqtt_init() and mqtt_publish()

#define PORT 8080
#define MQ_MSGSIZE 1024
#define QUEUE_SIZE 50

// ----------------------
// Logging function
// ----------------------
void log_message(const char* prefix, const char* msg, int priority) {
    time_t now = time(NULL);
    char ts[64];
    strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", localtime(&now));
    printf("[%s] %s (priority %d) -> %s\n", ts, prefix, priority, msg);
}

// ----------------------
// C-style thread-safe message queue
// ----------------------
typedef struct {
    char msgs[QUEUE_SIZE][MQ_MSGSIZE];
    int front;
    int rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} msg_queue_t;

msg_queue_t msg_queue;

void queue_init(msg_queue_t* q) {
    q->front = 0;
    q->rear = 0;
    q->count = 0;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->cond, NULL);
}

int queue_push(msg_queue_t* q, const char* msg) {
    pthread_mutex_lock(&q->mutex);
    if (q->count == QUEUE_SIZE) { // queue full
        pthread_mutex_unlock(&q->mutex);
        return -1;
    }
    strncpy(q->msgs[q->rear], msg, MQ_MSGSIZE-1);
    q->msgs[q->rear][MQ_MSGSIZE-1] = '\0';
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->count++;
    pthread_cond_signal(&q->cond);
    pthread_mutex_unlock(&q->mutex);
    return 0;
}

int queue_pop(msg_queue_t* q, char* out) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == 0) {
        pthread_cond_wait(&q->cond, &q->mutex);
    }
    strncpy(out, q->msgs[q->front], MQ_MSGSIZE);
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->count--;
    pthread_mutex_unlock(&q->mutex);
    return 0;
}

// ----------------------
// MQTT Worker Thread
// ----------------------
void* mqtt_worker(void* arg) {
    char msg[MQ_MSGSIZE];
    while (1) {
        queue_pop(&msg_queue, msg); // wait for a message
        log_message("Publishing", msg, 1);
        mqtt_publish(msg);          // publish to MQTT broker
    }
    return NULL;
}

// ----------------------
// TCP Client Handler
// ----------------------
void* client_handler(void* arg) {
    int sock = *(int*)arg;
    free(arg);

    char buffer[MQ_MSGSIZE];
    int read_size = recv(sock, buffer, MQ_MSGSIZE-1, 0);
    if (read_size > 0) {
        buffer[read_size] = '\0';
        log_message("Received", buffer, 0);

        // push message to in-memory queue
        if (queue_push(&msg_queue, buffer) < 0) {
            log_message("Error", "Queue full, dropping message", 1);
        }
    }

    close(sock);
    return NULL;
}

// ----------------------
// Main Server
// ----------------------
int main() {

    // Initialize MQTT
    if (mqtt_init() != 0) {
        exit(EXIT_FAILURE);
    }

    // Initialize message queue
    queue_init(&msg_queue);

    // Start MQTT worker thread
    pthread_t mqtt_tid;
    pthread_create(&mqtt_tid, NULL, mqtt_worker, NULL);
    pthread_detach(mqtt_tid);

    // Setup TCP server
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket failed"); exit(EXIT_FAILURE); }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed"); exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) {
        perror("listen failed"); exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept multiple clients
    while (1) {
        int* new_sock = (int*)malloc(sizeof(int));
        *new_sock = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (*new_sock < 0) {
            perror("accept failed");
            free(new_sock);
            continue;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, client_handler, new_sock);
        pthread_detach(tid);
    }

    // Cleanup (never reached)
    MQTTClient_disconnect(mqtt_client, 10000);
    MQTTClient_destroy(&mqtt_client);

    return 0;
}
