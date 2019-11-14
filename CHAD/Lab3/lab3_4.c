/*O código falha (produz um valor diferente de zero para ​ error count ​ ) se compilado com alguns
compiladores OpenACC. Qual poderá ser a causa? Como pode o erro ser prevenido?*/

#include <stdio.h>
const int N=100, M=200;

int main() {
	int m[N][M];
	for(int i=0; i<N; i++)
		for(int j=0; j<M; j++)
			m[i][j]=1;
#pragma acc kernels
		{
			for(int i=0; i<N; i++)
				for(int j=M-i; j<M; j++)
					m[i][j]=i+j+1;
		}
// verify result
int errcnt=0;
for(int i=0; i<N; i++)
	for(int j=0; j<M; j++) {
		int expect=(j>=M-i)? i+j+1: 1;
		if(m[i][j]!=expect)
			errcnt++;
		}

	printf("Encountered %d errors\n", errcnt);
	return (errcnt != 0);
}
/*RESPOSTA:m é o endereço do primeiro elemento duma matriz 2D [N][M] e é acedido multiplas vezes em todos os loops.
Para garantir bom comportamento, devemos garantir que esta matriz só pode ser acedida por m (não é aliased)
e devemos declarar o ponteiro m como restrict. Para além disso, podemos também fazer uma gestão de memória explícita:
fazer copyout(m), alocar memória no device que será devolvida para o host (pode prevenir problemas com cache/read after write?)