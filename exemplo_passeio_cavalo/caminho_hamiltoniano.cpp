#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>

// Definindo as dire��es poss�veis de movimento do cavalo
int movimentos[8][2] = {
    {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
    {-2, -1}, {-1, -2}, {1, -2}, {2, -1}
};

// Prot�tipos das fun��es
bool resolver_passeio(int N, int M, int x_inicial, int y_inicial);
bool passeio_cavalo(int x, int y, int passo, int N, int M, int *caminho, bool *visitado);
bool posicao_valida(int x, int y, int N, int M);
int contar_movimentos_validos(int x, int y, int N, int M, bool *visitado);
void ordenar_movimentos(int x, int y, int N, int M, bool *visitado, int *ordem);
void imprimir_tabuleiro(int N, int M, int *caminho);
bool converter_coordenada_xadrez(const char *coord, int N, int M, int *x, int *y);
void imprimir_caminho_xadrez(int N, int M, int *caminho);

int main() {
	
	setlocale(LC_ALL, "");
	
    int N, M;
    char coord[3];
    int x_inicial, y_inicial;

    printf("Digite as dimens�es do tabuleiro (N M): ");
    
    if (scanf("%d %d", &N, &M) != 2 || N <= 0 || M <= 0) {
        printf("Dimens�es inv�lidas!\n");
        return 1;
    }

    printf("Digite a posi��o inicial (ex: a1): ");
    scanf("%2s", coord);

    if (!converter_coordenada_xadrez(coord, N, M, &x_inicial, &y_inicial)) {
        printf("Posi��o inicial inv�lida!\n");
        return 1;
    }

    if (!resolver_passeio(N, M, x_inicial, y_inicial)) {
        printf("\nN�o foi poss�vel encontrar um passeio do cavalo para este tabuleiro e posi��o inicial.\n");
    }

    return 0;
}

// Converte nota��o de xadrez (ex: "a1") para coordenadas de matriz (0-based)
bool converter_coordenada_xadrez(const char *coord, int N, int M, int *x, int *y) {
	
    if (strlen(coord) != 2) return false;
    
    char coluna = tolower(coord[0]);
    char linha = coord[1];
    
    // Converte coluna (letra) para x (0-based)
    *x = coluna - 'a';
    
    // Converte linha (n�mero) para y (0-based)
    *y = linha - '1';
    
    // Verifica se as coordenadas est�o dentro do tabuleiro
    return (*x >= 0 && *x < M && *y >= 0 && *y < N);
}

// Fun��o principal para resolver o passeio do cavalo
bool resolver_passeio(int N, int M, int x_inicial, int y_inicial) {
	
    // Aloca mem�ria para o caminho e tabuleiro visitado
    int *caminho = (int *)malloc(N * M * 2 * sizeof(int));
    bool *visitado = (bool *)calloc(N * M, sizeof(bool));

    // Inicializa a posi��o inicial
    caminho[0] = x_inicial;
    caminho[1] = y_inicial;
    visitado[x_inicial * M + y_inicial] = true;

    // Chama a fun��o recursiva com a heur�stica de Warnsdorff
    if (passeio_cavalo(x_inicial, y_inicial, 1, N, M, caminho, visitado)) {
    	
        printf("\nPasseio do Cavalo encontrado:\n");
        imprimir_tabuleiro(N, M, caminho);
        
        printf("\nCaminho em nota��o de xadrez:\n");
        imprimir_caminho_xadrez(N, M, caminho);
        
        free(caminho);
        free(visitado);
        
        return true;
    }

    free(caminho);
    free(visitado);
    
    return false;
}

// Imprime o caminho em nota��o de xadrez
void imprimir_caminho_xadrez(int N, int M, int *caminho) {
	
    int total_passos = N * M;
    
    for (int passo = 0; passo < total_passos; passo++) {
        int x = caminho[passo * 2];
        int y = caminho[passo * 2 + 1];
        
        // Converte coordenadas de matriz para nota��o de xadrez
        char coluna = 'a' + x;
        int linha = y + 1;
        
        printf("Passo %2d: %c%d", passo + 1, coluna, linha);
        
        // Quebra de linha para melhor visualiza��o
        if ((passo + 1) % 5 == 0 || passo == total_passos - 1) {
            printf("\n");
        } else {
            printf(" -> ");
        }
    }
}

// Fun��o recursiva com heur�stica de Warnsdorff
bool passeio_cavalo(int x, int y, int passo, int N, int M, int *caminho, bool *visitado) {
	
    if (passo == N * M) {
        return true;
    }

    int ordem[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    
    ordenar_movimentos(x, y, N, M, visitado, ordem);

    for (int i = 0; i < 8; i++) {
    	
        int movimento = ordem[i];
        int nx = x + movimentos[movimento][0];
        int ny = y + movimentos[movimento][1];

        if (posicao_valida(nx, ny, N, M) && !visitado[nx * M + ny]) {
            visitado[nx * M + ny] = true;
            caminho[passo * 2] = nx;
            caminho[passo * 2 + 1] = ny;

            if (passeio_cavalo(nx, ny, passo + 1, N, M, caminho, visitado)) {
                return true;
            }

            visitado[nx * M + ny] = false;
        }
    }

    return false;
}

bool posicao_valida(int x, int y, int N, int M) {
    return (x >= 0 && x < M && y >= 0 && y < N);
}

//Contabiliza os movimentos v�lidos
int contar_movimentos_validos(int x, int y, int N, int M, bool *visitado) {
	
    int count = 0;
    
    for (int i = 0; i < 8; i++) {
    	
        int nx = x + movimentos[i][0];
        int ny = y + movimentos[i][1];
        
        if (posicao_valida(nx, ny, N, M) && !visitado[nx * M + ny]) {
            count++;
        }
    }
    
    return count;
}

//Verifica a quantidade de movimentos para a posicao e ordena-os de forma crescente
void ordenar_movimentos(int x, int y, int N, int M, bool *visitado, int *ordem) {
	
    int counts[8];
    
    for (int i = 0; i < 8; i++) {
    	
        int nx = x + movimentos[i][0];
        int ny = y + movimentos[i][1];
        
        if (posicao_valida(nx, ny, N, M) && !visitado[nx * M + ny]) {
            counts[i] = contar_movimentos_validos(nx, ny, N, M, visitado);
        } else {
            counts[i] = 9;
        }
        
    }
    
    for (int i = 0; i < 7; i++) {
    	
        int min_idx = i;
        
        for (int j = i + 1; j < 8; j++) {
            if (counts[j] < counts[min_idx]) {
                min_idx = j;
            }
        }
        
        int temp = ordem[i];
        ordem[i] = ordem[min_idx];
        ordem[min_idx] = temp;
        
        int temp_count = counts[i];
        counts[i] = counts[min_idx];
        counts[min_idx] = temp_count;
        
    }
}

void imprimir_tabuleiro(int N, int M, int *caminho) {
	
    int tabuleiro[M][N]; // Invertido para corresponder � nota��o de xadrez
    
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            tabuleiro[i][j] = -1;
        }
    }
    
    for (int passo = 0; passo < N * M; passo++) {
        int x = caminho[passo * 2];
        int y = caminho[passo * 2 + 1];
        tabuleiro[x][y] = passo + 1;
    }
    
    printf("\nTabuleiro com o passeio (coordenadas em xadrez):\n");
    
    // Imprime letras das colunas
    printf("   ");
    for (int i = 0; i < M; i++) {
        printf("  %c ", 'a' + i);
    }
    printf("\n");
    
    // Imprime tabuleiro com n�meros das linhas
    for (int j = N - 1; j >= 0; j--) {
        printf("%2d ", j + 1);
        for (int i = 0; i < M; i++) {
            if (tabuleiro[i][j] == -1) {
                printf("  . ");
            } else {
                printf("%3d ", tabuleiro[i][j]);
            }
        }
        printf("\n");
    }
    
}
