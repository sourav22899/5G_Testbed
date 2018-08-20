/*
	Code for 5G Testbed project
	Header file to include the all the required functions
	Created by V.A.S.N.Y.M
*/

// Include headers BEGIN

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>
#include<math.h>
#include<arpa/inet.h>
#include<string.h>

// Include headers END

// typedefs START

typedef short int si;

// typedefs END

// Constants declaration BEGIN

#define n 1000
#define BIT_WIDTH 12
#define PORT 8080
#define MAXLINE 1024
#define MAX 69362
#define MAX_SIZE_FOR_EVENTS 256

// Constants declaration END

// Functions to encode BEGIN

int mb(int x,int q,int p);
int fb(int x,int q,int p);
int lb(int x,int q,int p);

// Functions to encode END

// Functions to encode header BEGIN

int headencode();

// Functions to encode header END

// Functions to decode BEGIN

int fb1(char c,int q);
int mb1(char c,int r,int o);
int lb1(char c,int r,int o);

// Functions to decode END

//wrapper function description for error handling BEGIN

int socket_e(int family,int type,int protocol);
int bind_e(int sockfd, const struct sockaddr *addr,socklen_t addrlen);

//wrapper function description for error handling END


// Function description to encode BEGIN

int fb(int x,int q,int p)
{
  // q number of first bits from total p bits.they will be at last
	int r;
	r=pow(2,p-q);
	return (x-(x%r))/r;
}
int mb(int x,int q,int p)
{
  //8 number of bits after leaving the fist q bits
	int r;
  	r=pow(2,p-q);
  	return fb(x%r,8,p-q);
}
int lb(int x,int q,int p)
{
  // last q bits of x from total p bits.they will be at first
  	int r,t;
  	t=pow(2,8-q);
  	r=pow(2,q);
  	return ((x%r)*t);
}

// Function description to encode END

// Function description to decode BEGIN

int  fb1(char c,int q)
{
   // first q bits will be stored at last
	int x;
	x=(unsigned char)c;
	// if(x<0)
	//{x += 256;}
	int j=pow(2,8-q);
	return (x-(x%j))/j;
}
int  mb1(char c,int r,int o)
{
   // 8 bits will be stored after the r bits for int
	int x;
	x=(unsigned char)c;
	// if(x<0)
	//{x += 256;}
	int j=pow(2,o-r-8);
	return x*j;
}
int lb1(char c,int r,int o)
{
   //last r bits will stored at first
	int x;
	x=(unsigned char)c;
	// if(x<0)
	//{x += 256;}
	int j=pow(2,r);
	int k=pow(2,o-r);
	return (x%j)*k;
}

// Function description to decode END

// Function description to encode header BEGIN

int headencode()
{
	short int pr, reserved ,c_;
	printf("the value of ecpri protocol revision:");
	scanf("%hd",&pr);
	printf("the value of reserved:");
	scanf("%hd",&reserved);
	printf("the value of C:");
	scanf("%hd",&c_);
	return ((16*pr) + (2*reserved) + c_);
}

// Function description to encode header END

// Function description to decode header BEGIN

void headdecode(int k,si* c,si* reserved,si* pr)
{
	*c = k % 2;
    k /= 2;
    *reserved = k % 8;
    k /= 8; 
    *pr = k;
}

// Function description to decode header END

//required IQ_structures BEGIN

struct IQpayload{
  	short int pc_id;
  	short int seq_id;
  	char IQ[n];
};
struct ecpri_header{
 	char first_byte;
 	char msg_type;
 	short int payloadsize;
};
struct packet{
  	struct ecpri_header header;
  	struct IQpayload payloads;
};

//required IQ_structures END

//wrapper function description for error handling BEGIN

int socket_e(int family,int type,int protocol)
{
	int k;
	if ( (k = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
	{
	    perror("socket creation failed");
	}
	return k;
}
int bind_e(int sockfd, const struct sockaddr *addr,socklen_t addrlen)
{
	int k;
	if ( (k = bind(sockfd, (const struct sockaddr *)&addr,sizeof(addr)))< 0 )
	{
		perror("bind failed");
	}
	return k;
}

//wrapper function description for error handling END
