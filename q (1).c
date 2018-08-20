#include "ecpri.h"
int main()
{
    int count;
    int outputi[MAX+n],outputq[MAX+n];
    short int c,pr,reserved;
    struct packet* recvpacket = malloc(sizeof(struct packet));
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
	sockfd =  socket_e(AF_INET,SOCK_DGRAM,0);

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	// Bind the socket with the server address
	bind_e(sockfd, (const struct sockaddr *)&servaddr,sizeof(servaddr));

	socklen_t len;int  m;
	int ki=((n*8)/(2*BIT_WIDTH));
	float aop=ki;
	int co=ceil(MAX/aop);
	while(count!=co)
	{
		count++;
		m = recvfrom(sockfd, (struct packet *)recvpacket, sizeof(struct packet),MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
		int i[(n*8)/(2*BIT_WIDTH)] = {0},q[(n*8)/(2*BIT_WIDTH)] = {0};
		int remaining=0;
		int j=0;
		int iq,f;
		headdecode((unsigned char)recvpacket->header.first_byte,&c,&reserved,&pr);
		
		/*
		f = (unsigned char)recvpacket->header.first_byte;
		c = f%2;
		reserved = ((f-c)%16)/2;
		pr = (f-(2*reserved +c))/16;
		*/
		for(int r=0;r<((n)*8)/(2*BIT_WIDTH);r++)
		{
			for(int m=0;m<2;m++)
			{
				if(remaining+BIT_WIDTH<=16)
				{
					iq=lb1(recvpacket->payloads.IQ[j],8-remaining,BIT_WIDTH);j++;
					if((BIT_WIDTH+remaining-8)==8)
					{
						iq=iq+fb1(recvpacket->payloads.IQ[j],BIT_WIDTH+remaining-8);
						remaining=(remaining+BIT_WIDTH-8)%8;j++;
					}
					else
					{
						iq=iq+fb1(recvpacket->payloads.IQ[j],BIT_WIDTH+remaining-8);
						remaining=(remaining+BIT_WIDTH-8)%8;
					}
				}
				else if(remaining+BIT_WIDTH<=24)
				{
					iq=lb1(recvpacket->payloads.IQ[j],8-remaining,BIT_WIDTH);j++;
					iq=iq+mb1(recvpacket->payloads.IQ[j],8-remaining,BIT_WIDTH);j++;
					if(BIT_WIDTH+remaining-16==8)
					{
						iq=iq+fb1(recvpacket->payloads.IQ[j],BIT_WIDTH+remaining-16);
						remaining=(BIT_WIDTH+remaining-16)%8;j++;
					}
					else
					{
						iq=iq+fb1(recvpacket->payloads.IQ[j],BIT_WIDTH+remaining-16);
						remaining=(BIT_WIDTH+remaining-16)%8;
					}
				}
				if(m == 0) i[r]=iq;
				else q[r]=iq;
			}
		}
		int ju=(n*8)/(2*BIT_WIDTH);
		for(int hu=0;hu<((n)*8)/(2*BIT_WIDTH);hu++)
		{
			outputi[hu+(recvpacket->payloads.seq_id*ju)]=i[hu];
			outputq[hu+(recvpacket->payloads.seq_id*ju)]=q[hu];
		}
	}
	FILE *fptr;
	fptr=fopen("output.txt","w");
	fprintf(fptr,"The value of ecpri protocol revision: %hd\n",pr);
	fprintf(fptr,"The value of reserved: %hd\n",reserved);
	fprintf(fptr,"The value of c: %hd\n",c);
	for(int ri=0;ri<MAX;ri++) fprintf(fptr,"%d %d\n",outputi[ri],outputq[ri]);
	fclose(fptr);
}
