#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <unistd.h>
#define MAX_IRQ_AMOUNT 1000

volatile sig_atomic_t exitRequested = 0;

void sigintHandler(int sig_num)
{
   exitRequested = 1;
}

int main (int argc, char **argv)
{
    signal(SIGINT, sigintHandler);
    char *input;   //input from stdin
    size_t buffer_size = 100;
    input = malloc(buffer_size*sizeof(char));
	
	//number of irq,sum of latency and  max latency of each line
    int count = 0; 
	int sum = 0;
    int max = 0; 
	
	//number of irq,sum of latency and  max latency of all lines
	int count_all = 0;
	int sum_all = 0;
    int max_all = 0; 
   	
	//latency_char of irq line with line number(col1) and number of occurance(col2)
    int latency_int[16][MAX_IRQ_AMOUNT]; 
	//number of event of IRQ line
    int irq_count[MAX_IRQ_AMOUNT];

    int index = 0; // 0 = irq_line, 1 = latency_char (clk) when reading input from std
    int	pos = 0; // index for irq and latency_char
    int irq_index = 0; // keep track of irq line being read
    
    //reading from stdin and not hitting SIGINT
    while(getline(&input, &buffer_size, stdin) > 0 && !exitRequested)
    {
        //character value read from stdin
        char irq[2]; // irq line number
        char latency_char[10]; // latency_char
        
        index = 0;
		
	for(int i = 0; i < strlen(input); i++)
        {
            //reading input, if not reach "," yet
	    if(input[i] != ',')
            {
                //save the irq line
		if(index == 0){
		   irq[pos] = input[i];
		
		   //if number read was 1 digit, fill the 2nd digit with 0 to avoid garbage
		   if(input[i+1] == ',' && pos == 0){
			irq[1] = 0;
			irq[0] = input[i]; 
		   }
		    pos++;
		}
		//save the latency_char 
		else if(index == 1){
	    	   latency_char[pos] = input[i];
		   pos++;
		}
            }
            
            //reading input, if "," reached
            else if(input[i] == ','){
                if(index == 0){
                   int temp=atoi(irq);
		
		   if(irq_index != temp){
		      irq_index = temp;
		      count = 0;
		   }
		
		   //move to latency_char value 
		   pos = 0;
		   index = 1;
           	}
	   	else if(index == 1){
		   latency_int[irq_index][count]=atoi(latency_char);
		   count++;
		   irq_count[irq_index] = count; 
		
		   //reset for the next line
		   pos = 0;
		   index = 0;
		   for(int i=0;i<sizeof(irq);i++){
		      irq[i]=0;
		   }
	    	   for(int j=0;j<sizeof(latency_char);j++){
	              latency_char[j]=0;
	           }
                   break;
	        }
            }
        }
    }
	
	//when hitting SIGINT and there are IRQs available
    if(count > 0){
        
        irq_index = 0;
        while(irq_index <= 15){
            max = 0;
            sum = 0;
            for(count = 0; count < irq_count[irq_index]; count++){
                if(max < latency_int[irq_index][count]){
                    max = latency_int[irq_index][count];
                }
                if(max_all < latency_int[irq_index][count]){
                    max_all = latency_int[irq_index][count];
                }
                sum += latency_int[irq_index][count];
                sum_all += latency_int[irq_index][count];
                count_all++;
            }	
            printf("%d,%d,%.2f,%d\n",irq_index,count,(float)sum/count,max);
            irq_index++;
        }
        printf("%d,%d,%.2f,%d\n",-1,count_all,(float)sum_all/count_all,max_all);
    }
}

