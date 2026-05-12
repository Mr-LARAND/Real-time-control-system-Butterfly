// Уровень абстракций для Linux (Ubuntu)

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include "abstraction_layer.h"

int can_init(const char *ifname) 
{
    int sock_fd;
    struct sockaddr_can addr;
    struct ifreq ifr;

    // Создаём сокет
    if ((sock_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Ошибка создания сокета");
        return -1;
    }

    // Указываем имя CAN-интерфейса "can0"
    strcpy(ifr.ifr_name, ifname);
    if (ioctl(sock_fd, SIOCGIFINDEX, &ifr) < 0) {
        perror("Ошибка ioctl");
        close(sock_fd);
        return -1;
    }

    // Привязываем сокет к CAN-интерфейсу
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Ошибка привязки сокета");
        close(sock_fd);
        return -1;
    }

    printf("CAN-интерфейс %s успешно инициализирован!\n", ifname);
    return sock_fd;
}

int can_send(int sock_fd, const CAN_Frame *frame) 
{
    // Структура кадра, которую для ядра Linux
    struct can_frame linux_frame;

    // Конвертируем нашу универсальную структуру в структуру SocketCAN
    linux_frame.can_id = frame->id;
    linux_frame.can_dlc = frame->dlc;
    memcpy(linux_frame.data, frame->data, frame->dlc);
    
    // Отправляем кадр
    if (write(sock_fd, &linux_frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
        perror("Ошибка отправки кадра");
        return -1;
    }

    return 0;
}

int can_receive(int sock_fd, CAN_Frame *frame) 
{
    struct can_frame linux_frame;
    ssize_t nbytes;

    // Пытаемся прочитать кадр из сокета
    nbytes = recv(sock_fd, &linux_frame, sizeof(struct can_frame), MSG_DONTWAIT);

    if (nbytes < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) 
            return 0; // Кадров нет
        perror("Ошибка приема кадра (recv)");
        return -1; // Произошла реальная ошибка
    }

    if (nbytes < sizeof(struct can_frame)) {
        fprintf(stderr, "Ошибка: принят неполный CAN-кадр\n");
        return -1;
    }

    // Конвертируем из структуры Linux в нашу универсальную структуру
    frame->id = linux_frame.can_id;
    frame->dlc = linux_frame.can_dlc;
    memcpy(frame->data, linux_frame.data, linux_frame.can_dlc);
    frame->rtr = (linux_frame.can_id & CAN_RTR_FLAG) ? 1 : 0;

    return 1; // Кадр успешно получен
}

void can_close(int sock_fd) {
    close(sock_fd);
}
