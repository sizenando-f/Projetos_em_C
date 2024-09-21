#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

int main(int argc, char* argv[])
{
	int32_t* V = NULL;
	FILE* fp;
	int i;
	char tipo;
	int N;
	char fnamein[256];
	char fnameout[256];
	
	
	if( argc != 3 || ((tipo = argv[2][0]) != 'c' && tipo != 'd' && tipo != 'a') || argv[2][1] != '\0' ) 
	{
		printf("Uso: %s num-elementos c|d|a\n", argv[0]);
		return 0;
	}

	N = atoi(argv[1]);

	if ( N > 0  )
		assert( (V = malloc(N*sizeof(int32_t))) != NULL );

	for(i=0; i<N; i++)
	{
		V[i] = i - N/2;
		/*
		printf("%i ", V[i]);
		*/
	}

	strcpy(fnamein, argv[1]);
	strcat(fnamein, ".");
	strcat(fnamein, argv[2]);
	strcpy(fnameout, fnamein);
	strcat(fnameout, ".out");	

	assert( (fp = fopen(fnameout, "w")) != NULL );
	fwrite(V, sizeof(int32_t), N, fp);
	fclose(fp);

	switch( tipo )
	{
		case 'a': /* aleatoria */	
			srand(time(NULL));

			for(i=N-1; i>0; i--)
			{
				int pos = rand() % (i+1);
				int32_t tmp = V[i];
				V[i] = V[pos];
				V[pos] = tmp;
			}
			break;
		case 'd': /* decrescente */
			for(i=0; i<N/2; i++)
			{
				int32_t tmp = V[i];
				V[i] = V[N-i-1];
				V[N-i-1] = tmp;
			}
			break;
		default: /* crescente */
			break;
	}

	/*
	for(i=0; i<N; i++)
		printf("%i ", V[i]);	
	*/

	assert( (fp = fopen(fnamein, "w")) != NULL );
	fwrite(V, sizeof(int32_t), N, fp);
	fclose(fp);

	if( V )
		free(V);

	return 0;
}
