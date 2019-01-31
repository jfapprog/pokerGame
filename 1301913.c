#include <stdio.h>
#include <stdlib.h>

#define NUMEROJOGADORES 4
#define NUMEROCARTAS 5
#define BARALHO 52
#define NUMEROVOLTAS 4

/*une partes ordenadas de um vetor*/
void unirPartesOrdenadas(int *v, int indiceMinimo, int indiceMedio, int indiceMaximo)
{
	int i = indiceMinimo;
	int j = indiceMedio;
	int k = 0;
	static int vetorAuxiliar[NUMEROCARTAS];
	/*copiar até que uma das partes chegue ao fim*/
	while (i<indiceMedio && j<= indiceMaximo)
	{
		/*se o elemento da primeira metade é menor que o da segunda copia-o e incrementa i*/
		if (v[i] < v[j])
			vetorAuxiliar[k++] = v[i++];
		/*caso contrário, copia o elemento da segunda metade e incrementa j*/
		else
			vetorAuxiliar[k++] = v[j++];
	}
	/*copiar os elementos restantes da primeira parte, se existirem*/
	while (i<indiceMedio)
		vetorAuxiliar[k++]=v[i++];
	/*copiar os elementos restantes da segunda parte, se existirem*/
	while (j<=indiceMaximo)
		vetorAuxiliar[k++]=v[j++];
	/*copiar os elementos do vetor Aux para o vetor original*/
	for (i=indiceMinimo, k=0;i<=indiceMaximo;i++, k++)
		v[i] = vetorAuxiliar[k];
}

/*ordena recursivamente um vetor de números inteiros*/
void ordenar(int *v, int indiceMinimo, int indiceMaximo)
{
	int indiceMedio;
	indiceMedio = (indiceMaximo+indiceMinimo)/2+1;
	/*condição de paragem*/
	if (indiceMinimo==indiceMaximo)
		return;
	/*chamadas recursivas*/
	else
	{
		ordenar(v,indiceMinimo,indiceMedio-1);
		ordenar(v,indiceMedio,indiceMaximo);
		unirPartesOrdenadas(v,indiceMinimo,indiceMedio,indiceMaximo);
	}
}

/*retorna o maximo de um vetor*/
int determinarMaximo(int *v, int dim)
{
	int i, maximo;
	maximo = v[0];
	for (i = 1; i < dim; i++)
	{
		/*compara o maximo atual com o valor do índice i, se este for maior atualiza o maximo*/
		if (v[i]>maximo)
			maximo = v[i];
	}
	return maximo;
}

/*retorna o minimo de um vetor*/
int determinarMinimo(int *v, int dim)
{
	int i, minimo;
	minimo = v[0];
	for (i = 1; i < dim; i++)
	{
		/*compara o minimo atual com o valor do índice i, se este for menor atualiza o maximo*/
		if (v[i]<minimo)
			minimo = v[i];
	}
	return minimo;
}

/*inicializa o vetor cartasUsadas ou o vetor cartasTrocadas a 0, significando que ainda nenhuma foi usada*/
void inicializarCartas(int *vetor)
{
	int i;
	for (i=0;i<BARALHO;i++)
		vetor[i] = 0;
}

/*escreve o número da carta*/
void escreverNumero(int numero)
{
	char numeroCarta[13] = {'2','3','4','5','6','7','8','9','1','V','D','R','A'};
	/*escrever o caratere correspondente ao número da carta, se a carta tem número 10 coloca um zero à frente do carater 1*/
	if (numero%13 == 8)
		printf("%c%c", numeroCarta[numero%13],'0');
	else
		printf("%c", numeroCarta[numero%13]);
}

/*escreve o naipe da carta*/
void escreverNaipe(int numero)
{
	char naipeCarta[4] = {'o','p','c','e'};
	/*escrever o naipe da carta*/
	printf("%c", naipeCarta[numero/13]);
}

/*escreve o vetor cartasUsadas ou o vetor cartasTrocadas*/
void listarCartas(int *vetor)
{
	int i;
	int naipeAnterior = -1;
	for (i=0;i<BARALHO;i++)
	{	
		/*apenas escreve a carta se ela corresponde a uma carta ativa do vetor*/
		if (vetor[i])
		{
			/*se a carta atual tem um naipe diferente da carta anterior escreve o naipe anterior*/
			if (naipeAnterior != -1 && i/13 != naipeAnterior)
			{
				escreverNaipe(naipeAnterior*13);
				putchar(' ');
			}
			escreverNumero(i);
			naipeAnterior = i/13;
		}
	}
	/*escreve o último naipe caso existam cartas ativas*/
	if (naipeAnterior != -1)
		escreverNaipe(naipeAnterior*13);
	putchar('\n');
}

/*escreve a mão de um jogador na tela*/
void escreverMao(int *maoJogador)
{
	int i;
	/*ordenar primeiro a mão*/
	ordenar(maoJogador,0,NUMEROCARTAS-1);
	for (i=0;i<NUMEROCARTAS;i++)
	{
		escreverNumero(maoJogador[i]);
		/*compara o naipe da carta atual com o da carta seguinte, só o escreve se for diferente ou se for a última carta*/
		if (i == NUMEROCARTAS-1 || maoJogador[i+1]/13 != maoJogador[i]/13)
		{
			escreverNaipe(maoJogador[i]);
			putchar(' ');
		}
	}
}

/*retorna aleatoriamente o número de uma carta ainda não usada, caso todas as cartas tenham sido utilizadas retorna -1*/
int retirarCartaBaralho(int *usadas, int *numeroUsadas)
{
	int numeroAleatorio;
	while (*numeroUsadas < BARALHO)
	{
		numeroAleatorio = rand() % BARALHO;
		/*testar se o número aleatório gerado não corresponde a uma carta usada*/
		if (!usadas[numeroAleatorio])
		{
			/*a carta selecionada passa a estar ativa nas cartas usadas*/
			usadas[numeroAleatorio] = 1;
			(*numeroUsadas)++;
			return numeroAleatorio;
		}
	}
	return -1;
}

/*gera aleatoriamente um conjunto de 5 cartas*/
void gerarMao(int *maoJogador, int *usadas, int *numeroUsadas)
{
	int i;
	for (i=0; i<NUMEROCARTAS; i++)
		maoJogador[i] = retirarCartaBaralho(usadas,numeroUsadas);
}

/*efetua a troca de duas cartas*/
int trocarCarta(int numeroCartaTrocar, int *maoJogador, int *usadas, int *numeroUsadas, int *trocadas)
{
	int cartaNova;
	cartaNova = retirarCartaBaralho(usadas,numeroUsadas);
	if (cartaNova != -1)
	{
		/*a carta que estava na mão a trocar passa a estar ativa nas cartas trocadas*/
		trocadas[maoJogador[numeroCartaTrocar]] = 1;
		maoJogador[numeroCartaTrocar] = cartaNova;
		/*retorna 1 porque a troca foi efetuada com sucesso*/
		return 1;
	}
	else
		/*retorna 0 porque a troca não foi efetuada*/
		return 0;
}

/*determina as frequências de cada número e de cada naipe numa mão*/
void calcularFrequencias(int *frequenciaNumero, int *frequenciaNaipe, int *maoJogador)
{
	int i, j, contador;
	/*determinar o número de cartas por cada número de carta*/
	for (i=0;i<13;i++)
	{
		contador = 0;
		for (j=0;j<NUMEROCARTAS;j++)
		{
			if (maoJogador[j]%13 == i)
				contador++;
		}
		frequenciaNumero[i] = contador;
	}
	/*determinar o número de cartas por cada naipe*/
	for (i=0;i<4;i++)
	{
		contador = 0;
		for (j=0;j<NUMEROCARTAS;j++)
		{
			if (maoJogador[j]/13 == i)
				contador++;
		}
		frequenciaNaipe[i] = contador;
	}
}

/*retorna um numero correspondente ao número das cartas a trocar*/
int determinarTrocas(int numeroJogador, int resultadoJogador, int *maoJogador)
{
	int numeroCartasTrocar = 0;
	int i;
	int casa = 1;
	int frequenciaNumero[13];
	int frequenciaNaipe[4];
	int maiorFrequencia, maiorNaipe;
	/*no caso do jogador humano pergunta*/
	if (numeroJogador == 0)
	{
		printf("Numero das cartas a trocar [0 para nao trocar]: ");
		scanf("%d", &numeroCartasTrocar);
		return numeroCartasTrocar;
	}
	/*no caso dos jogadores artificiais utiliza uma estratégia de troca*/
	else
	{
		/*determinar as frequências de cada número e de cada naipe de cartas*/
		calcularFrequencias(frequenciaNumero,frequenciaNaipe,maoJogador);
		/*determinar as cartas a trocar*/
		if (resultadoJogador >= 2 && resultadoJogador <= 4)
		{
			for (i=0;i<NUMEROCARTAS;i++)
			{
				/*verifica para cada carta se a frequência que corresponde ao seu número é diferente de 2 e 3,
				se for adiciona o seu índice nas cartas a trocar*/
				if (frequenciaNumero[maoJogador[i]%13] != 2 && frequenciaNumero[maoJogador[i]%13] != 3)
				{
					numeroCartasTrocar += (i+1)*casa;
					casa *= 10;
				}
			}
		}
		if (resultadoJogador == 1)
		{
			/*determinar a maior frequencia de naipe*/
			maiorFrequencia = determinarMaximo(frequenciaNaipe, 4);
			/*determinar primeiro naipe com maior frequencia*/
			for(i=0;i<4;i++)
			{
				if (frequenciaNaipe[i] == maiorFrequencia)
				{
					maiorNaipe = i;
					break;
				}
			}
			for (i=0;i<NUMEROCARTAS;i++)
			{
				/*verifica para cada carta se o seu naipe diferente do maior naipe,
				se for adiciona o seu índice nas cartas a trocar*/
				if (maoJogador[i]/13 != maiorNaipe)
				{
					numeroCartasTrocar += (i+1)*casa;
					casa *= 10;
				}
			}
		}
		return numeroCartasTrocar;
	}
}

/*efetua a troca das cartas de uma mão de um jogador*/
int trocarMao(int numeroJogador, int *maoJogador, int resultadoJogador, int *usadas, int *numeroUsadas, int *trocadas)
{
	int cartasTrocar;
	int numeroTrocas = 0;
	/*determinar primeiro o número das cartas a trocar*/
	cartasTrocar = determinarTrocas(numeroJogador, resultadoJogador, maoJogador);
	/*trocar cada uma das cartas*/
	while(cartasTrocar!=0)
	{
		/*trocar a carta, caso seja detetado que não se realizou a troca é porque já não cartas no baralho e por isso o ciclo termina*/
		if (!trocarCarta(cartasTrocar % 10 - 1, maoJogador,usadas,numeroUsadas,trocadas))
			break;
		numeroTrocas++;
		cartasTrocar /= 10;
	}
	return numeroTrocas;
}

/*inicializa a mesa de jogo*/
void inicializarMesa(int *moedasJogador, int *apostaJogador, int *mesa)
{
	int i;
	(*mesa) = 0;
	/*retirar uma moeda de cada jogador*/
	for (i=0;i<NUMEROJOGADORES;i++)
	{
		/*apenas retira moedas a jogadores com moedas*/
		if (moedasJogador[i]>0)
		{
			moedasJogador[i]--;
			apostaJogador[i] = 1;
			(*mesa)++;
		}
		else
			apostaJogador[i] = 0;
	}
}

/*coloca todos os jogadores in no início de uma volta*/
void inicializarInVolta(int *inVoltaJogador, int *moedasJogador)
{
	int i;
	/*se um jogador não tem moedas está out*/
	for (i=0;i<NUMEROJOGADORES;i++)
	{
		if (moedasJogador[i])
			inVoltaJogador[i] = 1;
		else
			inVoltaJogador[i] = 0;
	}
}

/*retorna a maior aposta entre os jogadores*/
int determinarAposta(int *apostaJogador,int *inVoltaJogador)
{
	int i;
	int vetorAuxiliar[NUMEROJOGADORES];
	/*para não contar com os jogadores out coloca no vetor auxiliar a sua aposta a -1*/
	for (i=0;i<NUMEROJOGADORES;i++)
	{
		if (inVoltaJogador[i])
			vetorAuxiliar[i] = apostaJogador[i];
		else
			vetorAuxiliar[i] = -1;
	}
	return determinarMaximo(vetorAuxiliar,NUMEROJOGADORES);
}

/*retorna o maior aposta possível*/
int determinarApostaMaxima(int *moedasInicio, int *inVoltaJogador)
{
	int i;
	int vetorAuxiliar[NUMEROJOGADORES];
	/*para não contar com os jogadores out coloca no vetor auxiliar o seu número de moedas a 10000 (problema, não é genérico)*/
	for (i=0;i<NUMEROJOGADORES;i++)
	{	
		if (inVoltaJogador[i])
			vetorAuxiliar[i] = moedasInicio[i];
		else
			vetorAuxiliar[i] = 10000;
	}
	return determinarMinimo(vetorAuxiliar,NUMEROJOGADORES);
}

/*escreve a situação atual da mesa - valor na mesa, aposta atual, aposta máxima, aposta de cada jogador e se está em jogo*/ 
void escreverMesaAtual(int mesa, int *apostaJogador, int *inVoltaJogador, int aposta, int apostaMaxima)
{
	int i;
	printf("Mesa: %d", mesa);
	printf(", aposta: %d (maximo: %d)", aposta, apostaMaxima);
	printf(", jogadores: ");
	for (i=0;i<NUMEROJOGADORES;i++)
	{
		printf("%d ", apostaJogador[i]);
		if (inVoltaJogador[i])
			printf("(in) ");
		else
			printf("(out) ");
	}
	putchar('\n');
}

/*retorna a decisão do jogador humano - manter, desistir, cobrir ou subir*/
int decisaoHumano(int aposta, int apostaMaxima, int apostaJogador, int moedasJogador)
{
	int opcao;
	/*compara a aposta em jogo, a aposta máxima, a aposta do jogador e as moedas disponíveis para aposta para apresentar as apostas disponíveis*/
	if (aposta == apostaMaxima)
	{
		if (apostaJogador == aposta)
			printf("1 - Manter, ");
		else if (apostaJogador < aposta)
		{
			printf("2 - Desistir, ");
			if (moedasJogador >= aposta - apostaJogador)
				printf("3 - Cobrir, ");
		}
	}
	else if (aposta < apostaMaxima)
	{
		if (apostaJogador == aposta)
		{
			printf("1 - Manter, ");
			if (moedasJogador >= 0)
				printf("4 - Subir, ");
		}
		else if (apostaJogador < aposta)
		{
			printf("2 - Desistir, ");
			if (moedasJogador >= aposta - apostaJogador)
				printf("3 - Cobrir, 4 - Subir, ");
		}
	}
	/*pergunta ao utilizador qual a sua opção (não valida os dados)*/
	printf("Opcao: ");
	scanf("%d", &opcao);
	return opcao;
}

/*retorna entre duas opções - auxiliar da funcao desisao artificial*/
int decisaoAuxiliar(int divisor, int opcaoA, int opcaoB, int moedasJogador, int moedasInicio)
{
	if (moedasJogador >= moedasInicio - moedasInicio/divisor)
		return opcaoB;
	else
		return opcaoA;
}

/*retorna a decisão de um jogador artificial - manter, desistir, cobrir ou subir*/
int decisaoArtificial(int aposta, int apostaMaxima, int apostaJogador, int moedasJogador, int moedasInicio, int resultadoJogador)
{
	int opcao;
	/*compara a aposta em jogo, a aposta máxima, a aposta do jogador e as moedas disponíveis para aposta para apresentar as apostas disponíveis,
	em cada situação decide de acordo com uma estratégia*/
	if (aposta == apostaMaxima)
	{
		if (apostaJogador == aposta)
		{
			printf("1 - Manter, ");
			opcao = 1;
		}
		else if (apostaJogador < aposta)
		{
			if (moedasJogador < aposta - apostaJogador)
			{
				printf("2 - Desistir, ");
				opcao = 2;
			}
			else
			{
				printf("2- Desistir, 3 - Cobrir, ");
				if (resultadoJogador >= 4)
					opcao = 3;
				if (resultadoJogador == 3)
					opcao = decisaoAuxiliar(2, 2, 3, moedasJogador, moedasInicio);
				if (resultadoJogador == 2)
				{
					opcao = decisaoAuxiliar(4, 2, 3, moedasJogador, moedasInicio);
					/*sempre que o resultado é 1 ou 2 e no caso de desistência faz bluff com 25% de probabilidade,
					retornado a decidir com um resultado agora aleatorio entre 1 e 9*/
					if (opcao == 2 && rand()%4 == 0)
						opcao = decisaoArtificial(aposta, apostaMaxima, apostaJogador, moedasJogador, moedasInicio,rand()%9 +1);
				}
				if (resultadoJogador == 1)
				{
					opcao = decisaoAuxiliar(8, 2, 3, moedasJogador, moedasInicio);
					if (opcao == 2 && rand()%4 == 0)
						opcao = decisaoArtificial(aposta, apostaMaxima, apostaJogador, moedasJogador, moedasInicio,rand()%9 +1);
				}
			}
		}
	}
	else if (aposta < apostaMaxima)
	{
		if (apostaJogador == aposta)
		{
			if (moedasJogador <= 0)
			{
				printf("1 - Manter, ");
				opcao = 1;
			}
			else
			{
				printf("1- Manter, 4 - Subir, ");
				if (resultadoJogador >= 8)
					opcao = 4;
				if (resultadoJogador == 7)
					opcao = decisaoAuxiliar(2, 1, 4, moedasJogador, moedasInicio);
				if (resultadoJogador == 5 || resultadoJogador == 6)
					opcao = decisaoAuxiliar(4, 1, 4, moedasJogador, moedasInicio);
				if (resultadoJogador == 4)
					opcao = decisaoAuxiliar(8, 1, 4, moedasJogador, moedasInicio);
				if (resultadoJogador == 3)
					opcao = decisaoAuxiliar(16, 1, 4, moedasJogador, moedasInicio);
				if (resultadoJogador == 1 || resultadoJogador == 2)
					opcao = 1;
			}
		}
		else if (apostaJogador < aposta)
		{
			if (moedasJogador < aposta - apostaJogador)
			{
				printf("2 - Desistir, ");
				opcao = 2;
			}
			else
			{
				printf("2. Desistir, 3 - Cobrir, 4 - Subir, ");
				if (resultadoJogador >= 8)
					opcao = 4;
				if (resultadoJogador == 7)
					opcao = decisaoAuxiliar(2, 3, 4, moedasJogador, moedasInicio);
				if (resultadoJogador == 5 || resultadoJogador == 6)
					opcao = decisaoAuxiliar(4, 3, 4, moedasJogador, moedasInicio);
				if (resultadoJogador == 4)
					opcao = decisaoAuxiliar(8, 3, 4, moedasJogador, moedasInicio);
				if (resultadoJogador == 3)
					opcao = decisaoAuxiliar(2, 2, decisaoAuxiliar(16, 3, 4, moedasJogador, moedasInicio), moedasJogador, moedasInicio);
				if (resultadoJogador == 2)
				{
					opcao = decisaoAuxiliar(4, 2, 3, moedasJogador, moedasInicio);
					if (opcao == 2 && rand()%4 == 0)
						opcao = decisaoArtificial(aposta, apostaMaxima, apostaJogador, moedasJogador, moedasInicio,rand()%8 +1);
				}
				if (resultadoJogador == 1)
				{
					opcao = decisaoAuxiliar(8, 2, 3, moedasJogador, moedasInicio);
					if (opcao == 2 && rand()%4 == 0)
						opcao = decisaoArtificial(aposta, apostaMaxima, apostaJogador, moedasJogador, moedasInicio,rand()%8 +1);
				}	
			}
		}
	}
	printf("Opcao: %d\n", opcao);
	return opcao;
}

/*atualiza a mesa após decisão do jogador*/
void atualizarMesa(int opcao, int *inVoltaJogador,int *mesa,int *moedasJogador,int *apostaJogador,int *aposta)
{
	int diferenca;
	if (opcao == 2)
		*inVoltaJogador = 0;
	if (opcao == 3)
	{
		diferenca = (*aposta) - (*apostaJogador);
		(*mesa) += diferenca;
		(*moedasJogador) -= diferenca;
		(*apostaJogador) += diferenca;
	}
	if (opcao == 4)
	{
		diferenca = (*aposta) - (*apostaJogador);
		(*mesa) += (diferenca + 1);
		(*moedasJogador) -= (diferenca + 1);
		(*apostaJogador) += (diferenca + 1);
		(*aposta)++;
	}
}

/*retorna o número de jogadores ainda em jogo*/
int jogadoresIn(int *inVoltaJogador)
{
	int numeroJogadoresIn=0;
	int i;
	for (i=0;i<NUMEROJOGADORES;i++)
	{
		if (inVoltaJogador[i])
			numeroJogadoresIn++;
	}
	return numeroJogadoresIn;
}

/*retorna a existência de uma sequência de números*/
int sequencia(int *frequenciaNumero)
{
	int i, j, contador;
	for (i=0;i<13;i++)
	{
		contador = 0;
		/*encontra uma frequência 1 e contabiliza as dos números seguintes se forem 1*/
		for(j=i;frequenciaNumero[j] == 1;j++)
			contador++;
		/*se o contador é 5 é porque existe uma sequência*/
		if (contador == 5)
			return 1;
	}
	return 0;
}

/*retorna o resultado de um jogador*/
int resultadoMao(int *maoJogador)
{
	int i, j;
	int frequenciaNumero[13];
	int frequenciaNaipe[4];
	/*determinar as frequências de cada número e de cada naipe de cartas*/
	calcularFrequencias(frequenciaNumero,frequenciaNaipe,maoJogador);
	/*determinar situações de cartas com o mesmo número*/
	for (i=0;i<13;i++)
	{
		if (frequenciaNumero[i] == 4)
			return 8;
		if (frequenciaNumero[i] == 3)
		{
			for (j=0;j<13;j++)
			{
				if (frequenciaNumero[j] == 2)
					return 7;
			}
			return 4;
		}
		if (frequenciaNumero[i] == 2)
		{
			for (j=0;j<13;j++)
			{
				if (j!=i && frequenciaNumero[j] == 2)
					return 3;
			}
			return 2;
		}
	}
	/*determinar situações de cartas com o mesmo naipe*/
	for (i=0;i<4;i++)
	{
		if (frequenciaNaipe[i] == 5)
		{
			if (sequencia(frequenciaNumero))
				return 9;
			return 6;
		}
	}
	/*determinar situação de sequencia de números apenas*/
	if (sequencia(frequenciaNumero))
		return 5;
	return 1;
}

/*distribui as moedas pelos jogadores ganhadores*/
void distribuirMoedas(int *resultadoJogador, int *moedasJogador, int *inVoltaJogador, int mesa)
{
	int i, contador = 0;
	int resultadoMaximo;
	/*colocar o resultado dos jogadores out a -1, para não serem contabilizados em caso de terem resultado igual ao ganhador*/
	for (i=0;i<NUMEROJOGADORES;i++)
	{
		if (!inVoltaJogador[i])
			resultadoJogador[i] = -1;
	}
	/*determinar resultado máximo*/
	resultadoMaximo = determinarMaximo(resultadoJogador, NUMEROJOGADORES);
	/*determinar o número de jogadores ganhadores*/
	for (i=0;i<NUMEROJOGADORES;i++)
	{
		if (resultadoJogador[i] == resultadoMaximo && inVoltaJogador[i] == 1)
			contador++;
	}
	/*distribuir as moedas pelos ganhadores*/
	for (i=0;i<NUMEROJOGADORES;i++)
	{
		if (resultadoJogador[i] == resultadoMaximo && inVoltaJogador[i] == 1)
			moedasJogador[i] += mesa/contador;
	}
}

/*retorna o número de falências*/
int contarFalencias(int *moedasJogador)
{
	int i, contador = 0;
	/*se no final de um jogo o número de moedas do jogador é zero, o jogador foi à falência*/
	for (i=0;i<NUMEROJOGADORES;i++)
	{
		if (moedasJogador[i] == 0)
			contador++;
	}
	return contador;
}

int main(int argc, char **argv)
{
	int i;
	int semente;
	int jogador[NUMEROJOGADORES][NUMEROCARTAS];
	int moedasJogador[NUMEROJOGADORES];
	int moedasInicio[NUMEROJOGADORES];
	int apostaJogador[NUMEROJOGADORES];
	int inVoltaJogador[NUMEROJOGADORES];
	int resultadoJogador[NUMEROJOGADORES];
	int usadas[BARALHO], trocadas[BARALHO];
	int numeroUsadas, numeroTrocas, numeroTrocasJogador;
	int numeroJogo, numeroVolta;
	int mesa, aposta, apostaMaxima;
	int opcao, subir;
	char *tipoResultado[9] = {"nada", "par", "duplo par", "trio", "sequencia", "cor", "fullen", "poker", "sequencia de cor"};
		
	if (argc != 2 || argc == 2 && atoi(argv[1]) < 0)
		printf("Sintaxe: efolioa <numero_inteiro_nao_negativo>\n");
	else
	{
		semente = atoi(argv[1]);
		srand(semente);
		numeroJogo = 1;
		/*inicializar número de moedas de cada jogador*/
		for (i=0;i<NUMEROJOGADORES;i++)
			moedasJogador[i] = 16;
		/*----------- INÍCIO DO JOGO ---------------*/
		while (contarFalencias(moedasJogador) < 3)
		{
			/*inicializar cartas usadas, cartas trocadas, moedas de início de jogo por jogador, mesa, mãos, número de volta e in/out*/
			inicializarCartas(usadas);
			numeroUsadas = 0;
			inicializarCartas(trocadas);
			for (i=0;i<NUMEROJOGADORES;i++)
				moedasInicio[i] = moedasJogador[i];
			inicializarMesa(moedasJogador, apostaJogador, &mesa);
			for (i=0;i<NUMEROJOGADORES;i++)
				gerarMao(jogador[i], usadas, &numeroUsadas);
			numeroVolta = 1;
			inicializarInVolta(inVoltaJogador, moedasInicio);
			/*----------------------- INÍCIO DA VOLTA ---------------------------------*/
			/*máximo de 4 voltas, termina se jogadores não realizarem trocas e se o baralho está vazio*/
			do
			{
				printf("\nJogo %d, volta %d:\n", numeroJogo, numeroVolta);
				/*inicializar número de trocas*/
				numeroTrocas = 0;
				/*escrever usadas, mao jogador humano e moedas de cada jogador*/
				printf("Usadas: ");
				listarCartas(trocadas);
				printf("Mao: ");
				escreverMao(jogador[0]);
				printf("\nApostas:\n");
				printf("Moedas dos jogadores: ");
				for (i=0;i<NUMEROJOGADORES;i++)
					printf("%d ", moedasJogador[i]);
				putchar('\n');
				/*determinar aposta atual, aposta máxima e resultado da mão de cada jogador*/
				aposta = determinarAposta(apostaJogador,inVoltaJogador);
				apostaMaxima = determinarApostaMaxima(moedasInicio, inVoltaJogador);
				for (i=0;i<NUMEROJOGADORES;i++)
					resultadoJogador[i] = resultadoMao(jogador[i]);
				/*escrever informações da situação atual da mesa*/
				escreverMesaAtual(mesa, apostaJogador,inVoltaJogador,aposta,apostaMaxima);
				/*realizar apostas dos jogadores in*/
				do
				{
					/*inicializar variável subir - controla se algum jogador subiu a aposta*/
					subir = 0;
					for (i=0; i<NUMEROJOGADORES;i++)
					{
						if (inVoltaJogador[i])
						{	
							printf("(jogador %d) ", i+1);
							if (i == 0)
								opcao = decisaoHumano(aposta,apostaMaxima, apostaJogador[i], moedasJogador[i]);
							else
								opcao = decisaoArtificial(aposta, apostaMaxima, apostaJogador[i], 
								moedasJogador[i], moedasInicio[i],resultadoJogador[i]);
							atualizarMesa(opcao,&inVoltaJogador[i],&mesa,&moedasJogador[i],&apostaJogador[i],&aposta);
							if (opcao == 4 && i!=0)
								subir = 1;
							/*escrever informações da situação atual da mesa*/
							escreverMesaAtual(mesa, apostaJogador,inVoltaJogador,aposta,apostaMaxima);
						}
					}
				} while (subir);
				/*verificar se apenas se mantém um jogador*/
				if (jogadoresIn(inVoltaJogador) == 1)
					break;
				/*realizar as trocas de cartas de cada jogador in*/
				for (i=0;i<NUMEROJOGADORES;i++)
				{
					if (inVoltaJogador[i])
					{
						numeroTrocasJogador = trocarMao(i,jogador[i],resultadoJogador[i],usadas,&numeroUsadas,trocadas);
						numeroTrocas += numeroTrocasJogador;
						if (numeroTrocasJogador)
							printf("Jogador %d troca %d carta(s)\n", i+1, numeroTrocasJogador);
					}
				}
				numeroVolta++;
			}
			while(numeroVolta <= NUMEROVOLTAS && numeroTrocas != 0  && numeroUsadas < BARALHO);
			/*------------------------- FIM DA VOLTA ------------------------------------------*/
			printf("\nFim do jogo %d:\nUsadas: ", numeroJogo);
			listarCartas(trocadas);
			/*determinar resultados de cada jogadores*/
			for (i=0;i<NUMEROJOGADORES;i++)
				resultadoJogador[i] = resultadoMao(jogador[i]);
			/*escrever mãos de cada jogador*/
			for (i=0;i<NUMEROJOGADORES;i++)
			{
				printf("Jogador %d: ", i+1);
				escreverMao(jogador[i]);
				printf("%s", tipoResultado[resultadoJogador[i]-1]);
				if (inVoltaJogador[i])
					printf(" (in)\n");
				else
					printf(" (out)\n");
			}
			/*distribuir as moedas pelos vitoriosos*/
			distribuirMoedas(resultadoJogador, moedasJogador, inVoltaJogador, mesa);
			printf("Moedas dos jogadores: ");
			/*escrever as moedas atuais de cada jogador*/
			for (i=0;i<NUMEROJOGADORES;i++)
				printf("%d ", moedasJogador[i]);
			putchar('\n');
			numeroJogo++;
		}
		/*----------- FIM DO JOGO ---------------*/
	}
}