#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define rowi 8	//matrix rows without being conved and deconved
#define coli 8	//matrix columns without being conved and deconved
#define rowo 4	//matrix rows after being conved without being deconved
#define colo 4	//matrix lines after being conved without being deconved
#define K 5		//kernel size (K=rowi-rowo+1)
#define S 1		//stride
#define pr (rowi*S-rowo+K-1) //padding rows pr=2*(rowi-rowo)
#define pc (coli*S-colo+K-1) //padding columns
#define N 8	//input channels
#define M 3	//output channels
#define Tr rowi
#define Tc coli
#define Tm M
#define Tn N
void padding(float frame[N][rowo][colo], float fpo[N][rowo+pr][colo+pc], int D);

//void rotate(float KERNEL[N][M][K][K]);

void read_fm_deconv(float data_input_fm[N][rowo][colo], float in_fm[N][rowo][colo], int D);
void read_kernel(float data_k[N][M][K][K], float k[Tn][Tm][K][K], int tmm, int tnn);
void write_fm_deconv(float out_fm[Tm][Tr][Tc], float data_output_fm[M][rowi][coli], 
					 int tmm, int trr, int tcc);
void read_padded_fm(float data_padded_fm[N][rowo+pr][colo+pc], float padded_fm_temp[Tn][Tr+K-1][Tc+K-1], 
					int tnn, int trr, int tcc);
void comp_engine_conv_2d(float in_fmp_temp[Tn][Tr+K-1][Tr+K-1], float out_fm_temp[Tm][Tr][Tc],
						 float k[Tn][Tm][K][K], int tmm, int tnn, int trr, int tcc);

void Deconv (float input_fm[N][rowo][colo], float w[N][M][K][K],
			float output_fm[M][rowi][coli])

{	
	int row, col, to, ti;
	int Row, Col, Depth;
	int lr_i=0;
	int ti_r, tr_r, tc_r, to_r;
	float data_fsp[N][rowo+pr][colo+pc] = {0};	
	//float fsp[N][rowo+pr][colo+pc] = {0};

  	float in_fm[N][rowo][colo] ={0};
  	float out_fm_temp[Tm][Tr][Tc] = {0};
  	float k_temp[Tn][Tm][K][K] = {0};
  	float padded_fm_temp[Tn][Tr+K-1][Tc+K-1] ={0};
  	float padded_fm_temp1[Tn][Tr+K-1][Tc+K-1] ={0};


  	for (row = 0; row < rowi; row += rowi)
  	{
  		for (col = 0; col < coli; col += coli)
  		{
			for (ti = 0; ti < N; ti += Tn)
  			{
  				read_fm_deconv(input_fm, in_fm, ti);
  				padding(in_fm, data_fsp, ti);
  			}
  		}
  	}



	
  	for (row = 0; row < rowi; row += Tr)
  	{
  		for (col = 0; col < coli; col += Tc)
  		{
 			for (to = 0; to < M; to += Tm)
  			{
  				for (ti = 0; ti < N; ti += Tn)
  				{
  				read_kernel(w, k_temp, to, ti);
//  				rotate(k);		
/*					if (lr_i==0)
					{
                        //ping pong logic for index shifting
                        //ti_r=ti;
                        to_r=to;
                        tc_r=col;
                        tr_r=row;
                        ti_r=ti+Tn;
                        if (ti_r==N)
                        {
                            ti_r=0;
                            if(to == M-Tm )
                            {
                                tr_r=row+Tr;
                                if(tr_r==rowo+pr)
                                {
                                    tr_r=0;
                                    tc_r=tc_r+Tc;
                                    if(tc_r==colo+pc)
                                    {
                                        tc_r=0;
                                    }
                                }
                            }
                        }

  					read_padded_fm(data_fsp, padded_fm_temp1, ti_r, tr_r, tc_r);
                    comp_engine_conv_2d(padded_fm_temp, out_fm_temp, k_temp, to, ti, row, col);
					lr_i=1-lr_i;
					}

					 else
					 {
                        //ping pong logic for index shifting
                        //ti_r=ti;
                        to_r=to;
                        tc_r=col;
                        tr_r=row;
                        ti_r=ti+Tn;
                        if (ti_r==N)
                        {
                            ti_r=0;
                            if(to == M-Tm )
                            {
                                tr_r=row+Tr;
                                if(tr_r==rowo+pr)
                                {
                                    tr_r=0;
                                    tc_r=tc_r+Tc;
                                    if(tc_r==colo+pc)
                                    {
                                        tc_r=0;
                                    }
                                }
                            }
                        }*/
  					read_padded_fm(data_fsp, padded_fm_temp, ti, row, col);
                    comp_engine_conv_2d(padded_fm_temp, out_fm_temp, k_temp, to, ti, row, col);
                   /* lr_i=1-lr_i;
                	}*/
 				}
  				write_fm_deconv(out_fm_temp, output_fm, to, row, col);
  			}
  		}
  	}

  		
/*  	printf("\nThis is the final Matrix.\n");
	for (Depth = 0; Depth < M; Depth++)
	{
			printf("\n");
		for (Row = 0; Row < rowi; Row++)
		{
			printf("\n");
			for (Col = 0; Col < coli; Col++)
			{
				printf("%f ", output_fm[Depth][Row][Col]);
			}
		}
	}
*/ 	
	
}

void read_padded_fm(float data_padded_fm[N][rowo+pr][colo+pc], float padded_fm_temp[Tn][Tr+K-1][Tc+K-1], 
					int tnn, int trr, int tcc)
{
	int Row, Col, Depth;

	for (Depth = 0; Depth < Tn ; Depth++)
	{
		for (Row = 0; Row < Tr+K-1; Row++)
		{
			for (Col = 0; Col < Tc+K-1; Col++)
			{
#pragma HLS PIPELINE
				padded_fm_temp[Depth][Row][Col] = data_padded_fm[tnn+Depth][trr+Row][tcc+Col];
			}
		}
	}
/*	printf("%d\n", tnn);
	printf("%d\n", trr);
	printf("%d\n", tcc);
	printf("\nThis is DATA_padded_fm_temp\n");
	for (Depth = 0; Depth < Tn ; Depth++)
	{
		printf("\n");
		for (Row = 0; Row < Tr+K-1; Row++)
		{
			printf("\n");
			for (Col = 0; Col < Tc+K-1; Col++)
			{
				printf("%f ", data_padded_fm[Depth][Row][Col]);
			}
		}
	}

	printf("\nThis is padded_fm_temp\n");
	for (Depth = 0; Depth < Tn ; Depth++)
	{
		printf("\n");
		for (Row = 0; Row < Tr+K-1; Row++)
		{
			printf("\n");
			for (Col = 0; Col < Tc+K-1; Col++)
			{
				printf("%f ", padded_fm_temp[Depth][Row][Col]);
			}
		}
	}*/
}


void write_fm_deconv(float out_fm[Tm][Tr][Tc], float data_output_fm[M][rowi][coli],
					 int tmm, int trr, int tcc)
{
	int Row, Col, Depth;

	for (Depth = 0; Depth < Tm ; Depth++)
	{
		for (Row = 0; Row < Tr; Row++)
		{
			for (Col = 0; Col < Tc; Col++)
			{
#pragma HLS PIPELINE
				data_output_fm[tmm+Depth][trr+Row][tcc+Col] = out_fm[Depth][Row][Col];
			}
		}
	}

	for (Depth = 0; Depth < Tm ; Depth++)
	{
		for (Row = 0; Row < Tr; Row++)
		{
			for (Col = 0; Col < Tc; Col++)
			{
#pragma HLS PIPELINE
				out_fm[Depth][Row][Col] = 0;
			}
		}
	}
}

void read_fm_deconv(float data_input_fm[N][rowo][colo], float in_fm[N][rowo][colo], int D)
{
	int Row, Col, Depth;
  	
	for (Depth = 0; Depth < Tn ; Depth++)
	{
		for (Row = 0; Row < rowo; Row++)
		{
			for (Col = 0; Col < colo; Col++)
			{
#pragma HLS PIPELINE
				in_fm[D+Depth][Row][Col] = data_input_fm[D+Depth][Row][Col];
			}
		}
	}
}

void read_kernel(float data_k[N][M][K][K], float k[Tn][Tm][K][K], int tmm, int tnn)
{
	int Row, Col, Depth;
  	int to, ti;
  	
	for (to = 0; to < Tm; to++)				
  	{
  		for (ti = 0; ti < Tn; ti++)
  		{
	  		for (Row  = 0; Row < K; Row++)
	  		{
	  			for (Col = 0; Col < K; Col++)
	  			{
#pragma HLS PIPELINE
	  				k[ti][to][K-1-Row][K-1-Col] = data_k[tnn+ti][tmm+to][Row][Col];
	  			}
	  		}
	  	}
  	}
}

void comp_engine_conv_2d(float in_fmp_temp[Tn][Tr+K-1][Tc+K-1], float out_fmp_temp[Tm][Tr][Tc], 
						 float k[Tn][Tm][K][K], int tmm, int tnn, int trr, int tcc)
{
	int Row, Col, i, j, Depth;
  	int too, tii;
  	float temp1;
/* 			printf("\nThis is the w.\n");
	for (too = 0; too < Tm; too++)
	{
		printf("\nThis is kernel:%d\n",too);
		for (tii = 0; tii < Tn; tii++)
		{
			printf("\n");
			for (Row = 0; Row < K; Row++)
			{
				printf("\n");
				for (Col = 0; Col < K; Col++)
				{
					printf("%f ", k[tii][too][Row][Col]);
				}
			}
		}
	}

	printf("\nThis is the input Matrix.\n");
	for (Depth = 0; Depth < Tn; Depth++)
	{
			printf("\n");
		for (Row = 0; Row < Tr+K-1; Row++)
		{
			printf("\n");
			for (Col = 0; Col < Tc+K-1; Col++)
			{
				printf("%f ", in_fmp_temp[Depth][Row][Col]);
			}
		}
	}*/

//  	printf("This is tcc's value%d\n", trr);
	for(i = 0; i < K; i++) 
	{
//#pragma HLS UNROLL
		for(j = 0; j < K; j++) 
		{
//#pragma HLS UNROLL
			for(Row = 0; Row < Tr; Row++) 
			{
//#pragma HLS UNROLL
				for(Col = 0; Col < Tc; Col++) 
				{
#pragma HLS PIPELINE
					for (tii = 0; tii < Tn; tii++)
					{
#pragma HLS UNROLL
						#pragma HLS DEPENDENCE variable=in_fmp_temp inter false
						temp1 = in_fmp_temp[tii][S*Row+i][S*Col+j]; 
						for (too = 0; too < Tm; too++)
						{  
							#pragma HLS DEPENDENCE variable=out_fmp_temp inter false
#pragma HLS UNROLL
							out_fmp_temp[too][Row][Col] += 
							temp1*k[tii][too][i][j];
						}
					}
				}		
			}
		}			
	}
			

	

}


void padding(float frame[N][rowo][colo], float fpo[N][rowo+pr][colo+pc], int D)
{
	int row, col, depth;
	int temp;

	for (depth = 0; depth < Tn; depth++)
	{
//#pragma HLS piepline
		for (row = 0; row < rowo+pr; row++)
		{
//#pragma HLS UNROLL
			for (col = 0; col < colo+pc; col++)
			{
#pragma HLS PIPELINE
				if (((row>=pr/2) && (row<=(rowo-1+pr/2))) && ((col>=pc/2) && (col<=(colo-1+pc/2))) ) 		
				//if (((row>=2) && (row<=3)) && ((col>=2) && (col<=3)) )
				{
					fpo[D+depth][row][col]=frame[D+depth][row-pr/2][col-pc/2];
				}
				else
				{
					fpo[D+depth][row][col]=0;
				}
			}
		}
	}
	
}

/*void rotate(float KERNEL[N][M][K][K])
{
	float temp;

	int to, ti, Row, Col;

	for (to = 0; to < M; to++)				
  	{
//#pragma HLS PIPELINE
  		for (ti = 0; ti < N; ti++)
  		{
//#pragma HLS PIPELINE
	  		for (Row  = 0; Row < K; Row++)
	  		{
//#pragma HLS UNROLL
	  			for (Col = 0; Col < K/2; Col++)
	  			{
//#pragma HLS UNROLL
	  				temp = KERNEL[ti][to][Row][K-1-Col];
	  				KERNEL[ti][to][Row][K-1-Col] = KERNEL[ti][to][Row][Col];
	  				KERNEL[ti][to][Row][Col] = temp;
	  			}
	  		}

	  		for (Row  = 0; Row < K/2; Row++)
	  		{
//#pragma HLS UNROLL
	  			for (Col = 0; Col < K; Col++)
	  			{
//#pragma HLS UNROLL
	  				temp = KERNEL[ti][to][K-1-Row][Col];
	  				KERNEL[ti][to][K-1-Row][Col] = KERNEL[ti][to][Row][Col];
	  				KERNEL[ti][to][Row][Col] = temp;
	  			}
	  		}

	  	}
  	}*/
/*
  	for (to = 0; to < M; to++)				
  	{
//#pragma HLS PIPELINE
  		for (ti = 0; ti < N; ti++)
  		{
//#pragma HLS UNROLL
	  		for (Row  = 0; Row < K/2; Row++)
	  		{
//#pragma HLS UNROLL
	  			for (Col = 0; Col < K; Col++)
	  			{
//#pragma HLS UNROLL
	  				temp = KERNEL[ti][to][K-1-Row][Col];
	  				KERNEL[ti][to][K-1-Row][Col] = KERNEL[ti][to][Row][Col];
	  				KERNEL[ti][to][Row][Col] = temp;
	  			}
	  		}
	  	}
  	}
	
}*/
