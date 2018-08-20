#include "ecpri.h"
struct single_fault{
    int single_fault_header;
    int additional;
};
struct eve_ind{
    int eve_header;
    struct single_fault fault[MAX_SIZE_FOR_EVENTS];
};
/*
    encoding the first eve_id...etc into a integer
*/
int eve_head_encode(si eve_id,si eve_type,si seq_no,si no_of_faults)
{
    return (eve_id<<24)+(eve_type<<16)+(seq_no<<8)+no_of_faults;
}
void eve_head_decode(int k,si* eve_id,si* eve_type,si* seq_no,si* no_of_faults)
{
    *no_of_faults = k % MAX_SIZE_FOR_EVENTS;
    k /= MAX_SIZE_FOR_EVENTS;
    *seq_no = k % MAX_SIZE_FOR_EVENTS;
    k /= MAX_SIZE_FOR_EVENTS;
    *eve_type = k % MAX_SIZE_FOR_EVENTS;
    k /= MAX_SIZE_FOR_EVENTS; 
    *eve_id = k;
}
int single_fault_head_encode(si ele_id,si raise,si fault)
{
    return (ele_id<<16)+(raise<<12)+fault;
}
void single_fault_head_decode(int k,si* ele_id,si* raise,si* fault)
{
    *fault = k % (MAX_SIZE_FOR_EVENTS*16);
    k /= (MAX_SIZE_FOR_EVENTS*16);
    *raise = k % 16;
    k /= 16; 
    *ele_id = k;
}
int main()
{
    si eve_id,eve_type,no_of_faults,seq_no,ele_id,raise,fault;
    int additional;
    scanf("%hd %hd",&eve_id,&eve_type);
    no_of_faults = 0;
    if(eve_type < 1 && eve_type > 4) // Types of event types that have non zero #faults
    {
        scanf("%hd",&no_of_faults);
    }
    struct eve_ind *event = malloc(sizeof(struct eve_ind));
    memset(&event,0,sizeof(event));
    seq_no = 0;
    event->eve_header = eve_head_encode(eve_id,eve_type,seq_no,no_of_faults);
    /*
    si w,x,y,z;
    eve_head_decode(event->eve_header,&w,&x,&y,&z);
    printf("%hd %hd %hd %hd\n",w,x,y,z);  
    */
    int i = 0;
    while(i < no_of_faults)
    {
        scanf("%hd %hd %hd %d",&ele_id,&raise,&fault,&additional);
        event->fault[i].single_fault_header = single_fault_head_encode(ele_id,raise,fault);
        event->fault[i].additional = additional;
        i++;
    }
    return 0;
}
