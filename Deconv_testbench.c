#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define rowi 8	//matrix rows without being conved and deconved
#define coli 8	//matrix columns without being conved and deconved
#define rowo 4	//matrix rows after being conved without being deconved
#define colo 4	//matrix lines after being conved without being deconved
#define K 5		//kernel size
#define S 1		//stride
#define pr (rowi*S-rowo+K-1) //padding rows
#define pc (coli*S-colo+K-1) //padding columns
#define N 8	//input channels
#define M 3	//output channels

void main() {

	float input_fmap[N][rowo][colo];		//input matrix
	float w[N][M][K][K];					//kernel
//	float fsp[N][rowo+pr][colo+pc] = {0};	//matrix after padding
	float output_fmap[M][rowi][coli] = {0};	//output matrix
	float test_output[M][rowi][coli] = {0};	//absolute truth
	int check;
//	float fsp[N][rowo+pr][colo+pc] = {0};	
	int Row, Col, i, j, Depth;
  	int to, ti;
  	float temp;
  	int row, col;

	FILE *fp1;
  	FILE *fp2;
  	FILE *fp3;
  	
void Deconv (float input_fm[N][rowo][colo], float w[N][M][K][K],
			float output_fm[M][rowi][coli]);

  	if((fp1=fopen("D:\\WorkSpace\\data\\test_weight.txt","r"))==NULL)	
	{
		printf("cant open file,press any key M exit");
		getch();
		exit(1);
	}

  	for (to = 0; to < M; to++)				
  	{
  		for (ti = 0; ti < N; ti++)
  		{
	  		for (Row  = 0; Row < K; Row++)
	  		{
	  			for (Col = 0; Col < K; Col++)
	  			{
	  				fscanf(fp1,"%f", &w[ti][to][Row][Col]);
	  			}
	  		}
	  	}
  	}
  	fclose(fp1);

	if((fp2=fopen("D:\\WorkSpace\\data\\test_input.txt","r"))==NULL)	//以只读格式打开word_list.txt文件
	{
		printf("cant open file,press any key M exit");
		getch();
		exit(1);
	}

	for (Depth = 0; Depth < N ; Depth++)
	{
		for (Row = 0; Row < rowo; Row++)
		{
			for (Col = 0; Col < colo; Col++)
			{
				fscanf(fp2,"%f", &input_fmap[Depth][Row][Col]);
			}
		}
	}
	fclose(fp2);

	/*	  	printf("This is the input_fmap Matrix.\n");
	for (Depth = 0; Depth < N; Depth++)
	{
		printf("\n");
		for (row = 0; row < rowo; row++)
		{
			printf("\n");
			for (col = 0; col < colo; col++)
			{
				printf("%f ", input_fmap[Depth][row][col]);
			}
		}
	}
*/

	if((fp3=fopen("D:\\WorkSpace\\data\\test_output.txt","r"))==NULL)	
	{
		printf("cant open file,press any key M exit");
		getch();
		exit(1);
	}

	for (Depth = 0; Depth < M ; Depth++)
	{
		for (Row = 0; Row < rowi; Row++)
		{
			for (Col = 0; Col < coli; Col++)
			{
				fscanf(fp3,"%f", &test_output[Depth][Row][Col]);
			}
		}
	}
	fclose(fp3);



/*printf("\nThis is the w.\n");
	for (to = 0; to < M; to++)
	{
		printf("\nThis is kernel:%d\n",to);
		for (ti = 0; ti < N; ti++)
		{
			printf("\n");
			for (Row = 0; Row < K; Row++)
			{
				printf("\n");
				for (Col = 0; Col < K; Col++)
				{
					printf("%f ", w[ti][to][Row][Col]);
				}
			}
		}
	}
*/
	Deconv(input_fmap, w, output_fmap);

		check = 0;

	for (Depth = 0; Depth < M ; Depth++)
	{
		for (Row = 0; Row < rowi; Row++)
		{
			for (Col = 0; Col < coli; Col++)
			{	
				check += output_fmap[Depth][Row][Col] - test_output[Depth][Row][Col];
				
			}
		}
	}

	printf("\nThis is the final Matrix.\n");
	for (Depth = 0; Depth < M; Depth++)
	{
			printf("\n");
		for (Row = 0; Row < rowi; Row++)
		{
			printf("\n");
			for (Col = 0; Col < coli; Col++)
			{
				printf("%f ", output_fmap[Depth][Row][Col]);
			}
		}
	}

	printf("\nThis is the TEST final Matrix.\n");
	for (Depth = 0; Depth < M; Depth++)
	{
			printf("\n");
		for (Row = 0; Row < rowi; Row++)
		{
			printf("\n");
			for (Col = 0; Col < coli; Col++)
			{
				printf("%f ", test_output[Depth][Row][Col]);
			}
		}
	}

	if ( check != 0 )
		{
			printf("\nYour Code is WRONG!\n");
		}
	else
		{
			printf("\nThe Code is RIGHT!\n");
		}

}