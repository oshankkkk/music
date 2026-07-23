#include <stdio.h>   
#include <stdlib.h>         
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>         

int serverInit (){
	char *socketpath = "./build/us.socket";

	int server=socket(AF_UNIX,SOCK_STREAM,0);
	if (server==-1){
		perror("sever init");
	}
	struct sockaddr_un address;

	memset(&address,0,sizeof(address));

	address.sun_family = AF_UNIX;

	strcpy(address.sun_path, socketpath);

	unlink(socketpath);

	int bindpr=bind(server, (struct sockaddr *)&address, sizeof(address));

	if (bindpr== -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(server, 5) == -1) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	printf("Waiting for a client...\n");

	int client = accept(server, NULL, NULL);

	if (client == -1) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	printf("Client connected!\n");

	char message[] = "Hello World!";

	write(client, message, sizeof(message));

	close(client);

	close(server);

	unlink(socketpath);

	return 0;
}

