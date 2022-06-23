#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
	
char buffer[100];
int len;
int unfinished = 1;
int firstSignal = 1;

int id_secs;
int id_mins;
int id_hours; // variables para los PID de los procesos hijo

int secs = 0;
int mins = 0;
int hours = 0; // contadores para el reloj

int fd_secs[2];
int fd_mins[2];
int fd_hours[2];

void sigusr1(int signo){
	if(firstSignal){
		sprintf(buffer, "Senyal SIGUSR1 rebuda, PID del proces #%d\n", getpid());
		len = strlen(buffer);
		write(1, buffer, len);
		firstSignal = 0;
	}else{
		kill(id_secs, SIGUSR1);
		kill(id_mins, SIGUSR1);
		kill(id_hours, SIGUSR1);
		read(fd_secs[0], &secs, sizeof(int));
		read(fd_mins[0], &mins, sizeof(int));
		read(fd_hours[0], &hours, sizeof(int));
		sprintf(buffer, "%02d:%02d:%02d\n", hours, mins, secs);
  		len = strlen(buffer);
  		write(1, buffer, len);
	}	
}

void sigusr1secs(int signo){
	write(fd_secs[1], &secs, sizeof(int));
}

void sigusr1mins(int signo){
	write(fd_mins[1], &mins, sizeof(int));
}

void sigusr1hours(int signo){
	write(fd_hours[1], &hours, sizeof(int));
}

void sigusr2mins(int signo){
	//sprintf(buffer, "Senyal SIGUSR2 rebuda\n");
	//len = strlen(buffer);
  	//write(1, buffer, len);
	mins++; // incrementar el contador de minutos
}

void sigusr2hours(int signo){
	//sprintf(buffer, "Senyal SIGUSR2 rebuda\n");
	//len = strlen(buffer);
  	//write(1, buffer, len);
	hours++; // incrementar el contador de horas
}

void sigterm(int signo){
	sprintf(buffer,"Programa Finalitzat - Senyal SIGTERM rebuda\n");
	len = strlen(buffer);
  	write(1, buffer, len);
  	kill(id_secs, SIGTERM);
	kill(id_mins, SIGTERM);
	kill(id_hours, SIGTERM);
	unfinished = 0;
}
void sigtermChild(int signo){
	sprintf(buffer, "Senyal SIGTERM rebuda\n");
	len = strlen(buffer);
  	write(1, buffer, len);
}

void sigalrm(int signo){
	//sprintf(buffer, "Senyal SIGALRM rebuda\n");
	//len = strlen(buffer);
  	//write(1, buffer, len);
	secs++;
}

int main(void){
	//char buffer[100];
	//int len;
	
	/*int fd_secs[2];
	int fd_mins[2];
	int fd_hours[2]; // crear los file directory para cada proceso hijo*/
	
	/*int secs = 0;
	int mins = 0;
	int hours = 0; // contadores para el reloj*/
	
	pipe(fd_secs);
	pipe(fd_mins);
	pipe(fd_hours); // hacer pipe con cada fd
	
	sprintf(buffer, "Soc el pare amb PID #%d\n", getpid());
	len = strlen(buffer);
  	write(1, buffer, len);
	
	id_secs = fork(); // crear el primer hijo
	
	if(id_secs == 0){ // proceso segundos
		sprintf(buffer, "Soc el primer fill amb PID #%d\n", getpid());
		len = strlen(buffer);
  		write(1, buffer, len);
		signal(SIGUSR1, sigusr1); // asignar una funcion a la señal SIGUSR1
		signal(SIGALRM, sigalrm); // asignar una funcion a la señal SIGALRM
		
		pause();
		signal(SIGUSR1, sigusr1secs);
		alarm(1);
		
		read(fd_secs[0], &id_mins, sizeof(int)); // leer el PID del proceso minutos
		while(1){
			pause(); // esperar a la señal SIGUSR1 del padre
			alarm(1); // poner una alarma a 1 segundo
			//secs++; // incrementar contador de segundos
			if(secs >= 60){ // si los segundos llegan a 60
				secs = 0; // resetear el contador de segundos
				kill(id_mins, SIGUSR2); // mandar la señal SIGUSR2 al proceso minutos
			}
			sprintf(buffer, "(borrar)Segons: %d\n", secs);
			len = strlen(buffer);
  			//write(1, buffer, len);
		}
		exit(0);
	}
	
	id_mins = fork(); // crear el segundo hijo
	
	if(id_mins == 0){ // proceso minutos
		sprintf(buffer, "Soc el segon fill amb PID #%d\n", getpid());
		len = strlen(buffer);
  		write(1, buffer, len);
		signal(SIGUSR2, sigusr2mins); // asignar una funcion a la señal SIGUSR2
		signal(SIGUSR1, sigusr1mins); // asignar una funcion a la señal SIGUSR1
		read(fd_mins[0], &id_hours, sizeof(int)); // leer el PID del proceso minutos
		while(1){
			pause(); // esperar a la señal SIGUSR2 del proceso segundos
			//mins++; // incrementar contador de minutos
			if(mins >= 60){ // si los minutos llegan a 60
				mins = 0; // resetear el contador de minutos
				kill(id_hours, SIGUSR2); // mandar la señal SIGUSR2 al proceso horas
			}
			sprintf(buffer, "Minuts: %d\n", mins);
			len = strlen(buffer);
  			//write(1, buffer, len);
		}
		exit(0);
	}
	
	id_hours = fork(); // crear el tercer hijo
	
	if(id_hours == 0){ // proceso horas
		sprintf(buffer, "Soc el tercer fill amb PID #%d\n", getpid());
		len = strlen(buffer);
  		write(1, buffer, len);
		signal(SIGUSR2, sigusr2hours); // asignar una funcion a la señal SIGUSR2
		signal(SIGUSR1, sigusr1hours); // asignar una funcion a la señal SIGUSR1
		while(1){
			pause(); // esperar a la señal SIGUSR2 del proceso minutos
			//hours++; // incrementar el contador de horas
			sprintf(buffer, "Hores: %d\n", hours);
			len = strlen(buffer);
  			//write(1, buffer, len);
		}
		exit(0);
	}
	// proceso padre
	
	signal(SIGUSR1, sigusr1); // asignar una fucnion a la señal SIGUSR1
	signal(SIGTERM, sigterm);
	
	write(fd_secs[1], &id_mins, sizeof(int));
	write(fd_mins[1], &id_hours, sizeof(int));
	
	pause(); // esperar a la señal SIGUSR1
	kill(id_secs, SIGUSR1); // enviar la señal SIGUSR1 al proceso segundos
	
	while (unfinished) {
		pause(); // esperar a la señal SIGUSR1
		//kill(id_secs, SIGUSR1);
		//kill(id_mins, SIGUSR1);
		//kill(id_hours, SIGUSR1);
		/*read(fd_secs[0], &secs, sizeof(int));
		read(fd_mins[0], &mins, sizeof(int));
		read(fd_hours[0], &hours, sizeof(int));
		sprintf(buffer, "%02d:%02d:%02d\n", hours, mins, secs);
  		len = strlen(buffer);
  		write(1, buffer, len);*/
  	}
	
	return 0;
}
