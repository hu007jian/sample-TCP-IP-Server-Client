#include "headfile.h"

#define		SERVER_PORT		6700
#define		MAX_CONNECT		10
#define		MAX_BUFSIZE		512

void sig_pipe(int sign)
{
	printf("\nServer Catch a SIGINT signal!\n");
}

void Server_Process(int cid, struct sockaddr_in *caddr)
{
	char rbuf[MAX_BUFSIZE] = {0};
	int res = 0;
	int err = 0;
	char ipbuf[20] = {0};
	
	signal(SIGINT, sig_pipe);

	while(1)
	{
		res = read(cid, rbuf, MAX_BUFSIZE);
		if(-1 == res)
		{
			perror("Server read");
			continue;
		}	
		inet_ntop(AF_INET, (void *)&(caddr->sin_addr), ipbuf, 16);
		printf("receive from %s:%s", ipbuf, rbuf);
		if(-1 == (err = write(cid, (char *)&res, sizeof(int))))
		{
			perror("Server write");
			continue;
		}
	}
}

int main(int argc, char *argv[])
{
	struct sockaddr_in saddr,caddr;
	int s_sockid,c_sockid;
	int clen=0;
	int err;
	pid_t pid;	

	s_sockid = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == s_sockid)
	{
		perror("Server socket");
		return -1;
	}

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(SERVER_PORT);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(saddr.sin_zero), 8);
	memset((char *)&caddr, 0, sizeof(struct sockaddr_in));

	if(-1 == (err = bind(s_sockid, (struct sockaddr *)&saddr, sizeof(struct sockaddr))))
	{
		perror("Server bind");
		return -1;
	}

	
	if(-1 == listen(s_sockid, MAX_CONNECT))
	{
		perror("Server listen");
		return -1;
	}
	printf("Server socket init success, listen......\n");

	while(1)
	{
		clen = sizeof(struct sockaddr);
		if(-1 == (c_sockid = accept(s_sockid, (struct sockaddr *)&caddr, &clen)))
		{
			perror("Server accept");
			continue;
		}		
		
		pid = fork();
		if(0 == pid)
		{
			close(s_sockid);
			Server_Process(c_sockid, &caddr);
		}
		else if(pid > 0)
		{
			close(c_sockid);
		}
		else
		{
			perror("Server fork");
		}
	}
	return 0;
}

