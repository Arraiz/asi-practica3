#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#define SIZE 1024
#define KEY 0x78953960




/*
 * 
 */

int idMem1;
int idMem2;
char *mem1;
char *mem2;
int idSem;
int idSems;
int secreto1;
int offset1;
char secreto2[5];
int secreto8;
int secreto10;

void ej1() {
    idMem1 = shmget(KEY, SIZE, 0666);
    if (idMem1 < 0) {
        puts("error shmget");
    } else {
        mem1 = shmat(idMem1, 0, SHM_RDONLY);
        if (mem1 < 0) {
            puts("error en attach");
        } else {
            //leemos el secreto 1
            secreto1 = *((int *) mem1);
            printf("%d\n", secreto1);
            //leeamos el offset
            offset1 = *((int*) (mem1 + sizeof (int)));
            printf("%d\n", offset1);
            //leemos el secreto 2
            sprintf(secreto2, "%s", mem1 + offset1);
            printf("%s\n", secreto2);
            //quitamos l segmento
            sleep(2);
            if ((shmdt(mem1)) < 0) {
                puts("problema detach");
            } else {
                puts("detached");
            }
        }
    }

}

void ej2() {
    //creamos el segmento de mem
    idMem2 = shmget(KEY, SIZE, 0666 | IPC_CREAT);
    if (idMem2 < 0) {
        perror("shm");

    } else {
        mem2 = shmat(idMem2, 0, 0);
        //escribimos en formato string el secreto 2

        sprintf(mem2, "%s", secreto2);
        printf("escrito %s\n", secreto2);
        int pos = 290 + 16;
        printf("posicion: %d\n", pos);
        sprintf(mem2 + pos, "<%d>", secreto1);
        sleep(1);
        if ((shmdt(mem2)) < 0) {
            puts("problema detach");
        } else {
            puts("detached");
        }

    }
}

void ej3() {
    //creamos el semaforo
    idSem = semget(KEY, 1, 0666 | IPC_CREAT);
    //hacemos la operacion de inicializar
    semctl(idSem, 0, SETVAL, 960);
    int sem = semctl(idSem, 0, GETVAL);
    printf("sem: %d", sem);



}

void ej4() {
    idSems = semget(KEY, 2, 0666 | IPC_CREAT);
    semctl(idSems, 0, SETVAL, 1);
    semctl(idSems, 1, SETVAL, 2);
}

void ej5() {

    //acedemos al segmento de meoria
    idMem1 = shmget(KEY, SIZE, 0666);
    mem1 = shmat(idMem1, 0, 0);

    //acedemos al semaforo
    idSem = semget(KEY, 1, 0666);
    int val = semctl(idMem1, 1, GETVAL);
    printf("%d\n", val);

    //intentamos acceder al area operando sobre el smaforo
    struct sembuf operacion_acceso = {0, -1, 0};
    semop(idSem, &operacion_acceso, 1);
    //leemos el valor dle secreto
    secreto8 = *(int *) (mem1);
    printf("%d\n", secreto8);
    //lo devolvemos
    *(int*) (mem1) = -secreto8;
    sleep(3);
    struct sembuf operacion_salida[3] = {0, 1, 0};
    semop(idSem, &operacion_salida, 1);


}

void ej6() {
    //acedemos al segmento de meoria
    idMem2 = shmget(KEY, SIZE, 0666);
    mem2 = shmat(idMem2, 0, 0);

    //acedemos al semaforo
    idSems = semget(KEY, 2, 0666);
    int val = semctl(idSems, 1, GETVAL);
    printf("sem 1: %d\n", val);
    val = semctl(idSems, 2, GETVAL);
    printf("sem 2: %d\n", val);

    //intentamos acceder al area operando sobre el smaforo
    struct sembuf operacion_acceso[2] = {
        {0, -1, 0},
        {1, -1, 0}
    };
    semop(idSems, operacion_acceso, 2);
    puts("dentro seccion critica");
    //leemos el valor dle secreto
    secreto10 = *(int *) (mem2);
    printf("%d\n", secreto10);
    //lo devolvemos
    *(int*) (mem2) = -secreto10;
    sleep(3);
    struct sembuf operacion_salida[2] = {
        {0, 1, 0},
        {1, 1, 0}
    };
    semop(idSems, operacion_salida, 2);
    puts("fuera seccion critica");
}

ej7() {
    idSem = semget(KEY, 1, 0666 | IPC_CREAT);
    idMem1 = shmget(KEY, SIZE, 0666);
    puts("creando recursos");
    sleep(2);
    mem1 = shmat(idMem1, 0, 0);
    struct sembuf operacion_acceso[] = {0, -1, 0};
    semop(idSem, &operacion_acceso, 1);
    shmdt(mem1);
    shmctl(idMem1, IPC_RMID, NULL);
    semctl(idSem, 0,IPC_RMID, NULL);




}

int main(int argc, char** argv) {


   // puts("ej1");
   // getchar();
   // ej1();
   // puts("ej2");
   // getchar();
   // ej2();
   // getchar();
   // puts("ej3");
   // getchar();
   // ej3();
   // getchar();
    puts("ej4");
    getchar();
    ej4();
    getchar();
    puts("ej5");
    getchar();
    ej5();
    getchar();
    puts("ej6");
    getchar();
    ej6();
    getchar();
    puts("ej7");
    getchar();
    ej7();
    getchar();


    return (EXIT_SUCCESS);
}

