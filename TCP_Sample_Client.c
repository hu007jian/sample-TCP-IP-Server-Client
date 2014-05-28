#include "headfile.h"

#define		SERVER_PORT		6700
#define		MAX_BUFSIZE		512

void sig_pipe(int sign)
{
	printf("\nClient Catch a SIGINT signal!\n");
	_exit(0);
}

void Client_Process(int sid)
{
	int len = 0;
	int err = 0;
	char sbuf[MAX_BUFSIZE] = {0};
	char rbuf[10] = {0};
	int *disp = NULL;

	disp = (int *)&rbuf[0];

	signal(SIGINT, sig_pipe);

	while(1)
	{
		len = read(0, sbuf, MAX_BUFSIZE);
		if(len < 0)
		{
			perror("client read");
			continue;
		}
		else if(len > 0)
		{
			err = write(sid, sbuf, len);
			if(-1 == err)
			{
				perror("client write");
				continue;
			}
		}
		len = read(sid, rbuf, 10);
		if(len < 0)
		{
			perror("client read");
			continue;
		}
		else if(len > 0)
		{
			printf("send %d bytes success\n", *disp);
		}
	}
}

int main(int argc, char *argv[])
{
	struct sockaddr_in saddr;
	int s_sockid;
	int err;

	s_sockid = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == s_sockid)	
	{
		perror("client socket");
		return -1;
	}
	
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, argv[1], (void *)&saddr.sin_addr);
	bzero(&(saddr.sin_zero), 8);	

	if(-1 ==(err = connect(s_sockid, (struct sockaddr *)&saddr, sizeof(struct sockaddr))))
	{
		perror("client connect");
		return -1;
	}
	Client_Process(s_sockid);
/*	Client_Process_Msg(s_sockid);*/
	close(s_sockid);
	return 0;
}
