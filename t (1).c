#include "ecpri.h"
int main()
{
	int pcount = 0;
	struct packet *sending_msg = malloc(sizeof(struct packet));
	int in[2*((n*8)/(2*a))] = {0};
	for(int nu = 0;nu < n;nu++)
	{
		sending_msg->payloads.IQ[nu] = '\0';
	}
	sending_msg->header.payloadsize = n+4;
	int f = headencode() ;
	sending_msg->header.first_byte = f;
	sending_msg->header.msg_type = '0';
	sending_msg->payloads.seq_id = 0;
	int sockfd;
	char buffer[MAXLINE];
	struct sockaddr_in servaddr;

	// Creating socket file descriptor
	sockfd =  socket_e(AF_INET,SOCK_DGRAM,0);

	memset(&servaddr, 0, sizeof(servaddr));

	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	int n1, len;
	FILE *fpt;
	fpt = fopen("input.txt","r");
	int t = 0;
	while(!feof(fpt))
	{
		if((t%2 == 0) && (t == 2*((n*8)/(2*a))))
		{
			int iq;
			int j = 0;
			int remaining = 0;
			for(int hu = 0;hu < 2*((n*8)/(2*a));hu+=2)
			{
				printf("%d %d \n",in[hu],in[hu+1]);
			}
			for(int r = 0;r < 2*((n*8)/(2*a));r += 2)
			{
				iq = in[r];
				for(int m = 0;m < 2;m++)
				{
					if(remaining+a <= 16)
					{
						sending_msg->payloads.IQ[j] = sending_msg->payloads.IQ[j]+fb(iq,8-remaining,a);
						j++;
						if((a+remaining-8) == 8)
						{
							sending_msg->payloads.IQ[j] = lb(iq,a+remaining-8,a);
							remaining = (a+remaining-8)%8;
							j++;
						}
						else
						{	sending_msg->payloads.IQ[j] = lb(iq,a+remaining-8,a);
							remaining = (a+remaining-8)%8;
						}
					}
					else if(a+remaining<=24)
					{
						sending_msg->payloads.IQ[j]=sending_msg->payloads.IQ[j]+fb(iq,8-remaining,a);
						j++;
						sending_msg->payloads.IQ[j]=mb(iq,8-remaining,a);
						j++;
						if((a+remaining-16)==8)
						{
							sending_msg->payloads.IQ[j]=lb(iq,a+remaining-16,a);
							remaining=(a+remaining-16)%8;
							j++;
						}
						else
						{	sending_msg->payloads.IQ[j]=lb(iq,a+remaining-16,a);
							remaining=(a+remaining-16)%8;
						}
					}
				iq=in[r+1];
				}
			}
			sendto(sockfd,(struct packet *)sending_msg, sizeof(struct packet),
				0, (const struct sockaddr *) &servaddr,sizeof(servaddr));
			printf("packet sent successfully\n");
			pcount++;
			for(int nu=0;nu<n;nu++)
			{
				sending_msg->payloads.IQ[nu]='\0';
			}
			sending_msg->payloads.seq_id++;
			t = 0;
			for(int g = 0;g < 2*((n*8)/(2*a));g++)
			{
				in[g] = 0;
			}
		}
		fscanf(fpt,"%d",&in[t]);
		t++;
	}
	if(t!=1)
	{
		int iq;
		int j = 0;
		int remaining = 0;
		for(int hug=0;hug<2*((n*8)/(2*a));hug+=2)
		{
			printf("%d %d \n",in[hug],in[hug+1]);
		}
		float fo = 8.0;
		int ker = ceil((t*2*a)/f);
		sending_msg->header.payloadsize = ker+4;
		/*
		f = ((16*pr) + (2*reserved) + c_); // Why f is being reassigned again???
		*/
		sending_msg->header.first_byte= f;
		for(int r=0;r<(t-1);r+=2)
		{
			iq=in[r];
			for(int m=0;m<2;m++)
			{
				if(remaining+a<=16)
				{
					sending_msg->payloads.IQ[j]=sending_msg->payloads.IQ[j]+fb(iq,8-remaining,a);j++;
					if((a+remaining-8)==8)
					{
						sending_msg->payloads.IQ[j]=lb(iq,a+remaining-8,a);
						remaining=(a+remaining-8)%8;j++;
					}
					else
					{
						sending_msg->payloads.IQ[j]=lb(iq,a+remaining-8,a);
						remaining=(a+remaining-8)%8;
					}
				}
				else if(a+remaining<=24)
				{
					sending_msg->payloads.IQ[j] = sending_msg->payloads.IQ[j]+fb(iq,8-remaining,a);j++;
					sending_msg->payloads.IQ[j] = mb(iq,8-remaining,a);j++;
					if((a+remaining-16)==8)
					{
						sending_msg->payloads.IQ[j]=lb(iq,a+remaining-16,a);
					 	remaining=(a+remaining-16)%8;j++;
					}
					else
					{
						sending_msg->payloads.IQ[j]=lb(iq,a+remaining-16,a);
					 	remaining=(a+remaining-16)%8;
					}
				}
				iq=in[r+1];
			}
		}
		sendto(sockfd,(struct packet *)sending_msg, sizeof(struct packet),
				0, (const struct sockaddr *) &servaddr,
						sizeof(servaddr));
		printf("packet sent successfully\n");
		pcount++;
	}
	for(int nu=0;nu<n;nu++)
	{
		sending_msg->payloads.IQ[nu]='\0';
	}
	close(sockfd);
	fclose(fpt);
	printf("the total number of packets sent %d\n",pcount);
}
