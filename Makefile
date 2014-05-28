all:client server

client:TCP_Sample_Client.o Client_Msg.o
	gcc -o client TCP_Sample_Client.o Client_Msg.o
server:TCP_Sample_Server.o
	gcc -o server TCP_Sample_Server.o

clean:
	rm -f client server *.o
