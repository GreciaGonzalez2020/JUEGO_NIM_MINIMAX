/*Juego de Nim  Programa realizado para la materia de Inteligencia Artificial
/* El juego consiste en ir retirando palillos de un montón.
/* Pierde el jugador que se ve obligado a retirar el último palillo.
/* Estrategia: Siempre que sea el turno del ordenador, éste jugará si puede.
    de tal modo que queden (MAX_JUGADA+1)*(k-1)+1 palillos en el montón; si
    no le es posible, retira un único palillo del montón.
/* MAX_JUGADA = Número máximo de palillos que puede retirar un jugador del
   montón en una única jugada.
/*MIN_PALILLOS = Número mínimo de palillos para iniciar una jugada */

#include <stdlib.h>
#include <stdio.h>
#include <vector>

// Constantes
#define MAX_JUGADA   3
#define MIN_PALILLOS 5
#define MAX_PROF     10
#define INFINITO     1000000

typedef struct nodo {
    int numPalillos;
    int nivel;
    int jugador;
    struct nodo *hijos[MAX_JUGADA];
} Nodo;

int f(Nodo *u) {
    if (u->numPalillos == 1) {
        if (u->jugador == 0) {
            return INFINITO;
        } else {
            return -INFINITO;
        }
    } else {
        return 0;
    }
}

//Implementación de MINIMAX
float MiniMax(Nodo *u, int Prof, int Mano) {
    float m, t;
    if (u == NULL || Prof >= MAX_PROF) {
        return f(u);
    }
    if (Mano) {
        m = -INFINITO;
        for (int i = 0; i < MAX_JUGADA; i++) {
            if (u->hijos[i] != NULL) {
                t = MiniMax(u->hijos[i], Prof + 1, !Mano);
                if (t > m) {
                    m = t;
                }
            }
        }
    } else {
        m = INFINITO;
        for (int i = 0; i < MAX_JUGADA; i++) {
            if (u->hijos[i] != NULL) {
                t = MiniMax(u->hijos[i], Prof + 1, !Mano);
                if (t < m) {
                    m = t;
                }
            }
        }
    }
    return m;
}

void generaArbol(Nodo *u, int MaxJugada) {
    if (u->nivel >= MAX_PROF || u->numPalillos == 1) {
        return;
    }
    for (int i = 0; i < MaxJugada; i++) {
        Nodo *v = (Nodo *) malloc(sizeof(Nodo));
        v->numPalillos = u->numPalillos - (i + 1);
        v->nivel = u->nivel + 1;
        v->jugador = !(u->jugador);
        for (int j = 0; j < MaxJugada; j++) {
            v->hijos[j] = NULL;
        }
        u->hijos[i] = v;
        // Verificación para la última jugada del jugador humano
        if (v->numPalillos == 1 && v->jugador == 0) {
            return;
        }

        generaArbol(v, MaxJugada);
    }
}

int Estrategia(int NumPalillos, int MaxJugada) {
    Nodo *raiz = (Nodo *) malloc(sizeof(Nodo));
    raiz->numPalillos = NumPalillos;
    raiz->nivel = 0;
    raiz->jugador = 0;
    for (int j = 0; j < MaxJugada; j++) {
        raiz->hijos[j] = NULL;
    }

    generaArbol(raiz, MaxJugada);

    float m, t;
    int jugada = 1;
    m = -INFINITO;

     for (int i = 0; i < MaxJugada; i++) {
        if (raiz->hijos[i] != NULL) {
            int jugada = i + 1;
            int numPalillos = raiz->numPalillos - jugada;
            if (numPalillos % (MaxJugada+1) == 1) { // Si el número de palillos restantes es de la forma (MAX_JUGADA+1)*k+1, jugar para que queden (MAX_JUGADA+1)*(k-1)+1 palillos
                return jugada;
            }
            t = MiniMax(raiz->hijos[i], 0, 1);
            if (t >= m) {
                m = t;
                jugada = i + 1;
            }
        }
    }
    for (int i = 0; i < MaxJugada; i++) {
        if (raiz->hijos[i] != NULL) {
            free(raiz->hijos[i]);
        }
    }

    return jugada;
}

// Programa principal
int main(void) {
    int MaxJugada;
    int NumPalillos;
    int jugada;
    int turno = 0;

    printf("\nEL JUEGO DE NIM\n\n");

    do {
        printf("Numero de palillos (minimo %d): ", MIN_PALILLOS);
        scanf("%d", &NumPalillos);
    } while (NumPalillos < MIN_PALILLOS);

    MaxJugada=3;

    printf("\nQuedan %d palillos en el montón.\n", NumPalillos);

    while (NumPalillos > 1) {

        if (turno % 2 == 0) { // turno del jugador humano
            do {
                printf("¿Cuantos palillos desea retirar del monton (de 1 a %d)? ", MaxJugada);
                scanf("%d", &jugada);
            } while (jugada <= 0 || jugada > MaxJugada || jugada > NumPalillos);

            NumPalillos -= jugada;

            if (NumPalillos < 1) {
                printf("\nJugada incorrecta, podria haber ganado.\n");

            } else if (NumPalillos == 1) {

                printf("\n¡ Enhorabuena ! Ha ganado la partida.\n");

            } else {
                printf("Quedan %d palillos.\n", NumPalillos);
            }

        } else { // turno de IA
            jugada = Estrategia(NumPalillos, MaxJugada);

            printf("Yo retiro %d palillo%s.\n", jugada, jugada == 1 ? "" : "s");

            NumPalillos -= jugada;

            if (NumPalillos == 1) {
                printf("\nLo siento, ha perdido esta partida.\n");
            } else {
                printf("Quedan %d palillos.\n", NumPalillos);
            }
        }

        turno++;
    }

    return 0;
}

//Programa inicial sin minimax
/*// Programa principal

int main (void)
{
  int MaxJugada;
  int NumPalillos;
  int jugada;
  int Y;

  printf("\nEL JUEGO DE NIM\n\n");

  do {
    printf ("Numero de palillos (minimo %d): ", MIN_PALILLOS);
    scanf("%d", &NumPalillos);
  } while (NumPalillos<MIN_PALILLOS);

  do {
    printf ("Maxima jugada posible (1..%d): ", NumPalillos);
    scanf("%d", &MaxJugada);
  } while (MaxJugada<=1 || MaxJugada>=NumPalillos);

  printf ("\nQuedan %d palillos en el mont¢n.\n",NumPalillos);

  while (NumPalillos>1) {

    do {
       printf("¨ Cuantos palillos desea retirar del monton (de 1 a %d)? ", MaxJugada);
       scanf("%d", &jugada);
    } while (jugada<=0 || jugada>MaxJugada);

    NumPalillos -= jugada;


    if (NumPalillos<1) {
       printf ("\nJugada incorrecta, podria haber ganado.\n");

    } else if (NumPalillos==1) {

       printf ("\n­ Enhorabuena ! Ha ganado la partida.\n");

    }else {

       /* ------------------------------------------------------------ */
       /*  Estrategia seguida por el programa                          */
       /* ------------------------------------------------------------ */

  /*     jugada = NumPalillos%(MaxJugada+1);

       if (!jugada)
          jugada = MaxJugada;
       else if (jugada>1)
          jugada --;

       printf("Quedan %d palillos.\n",NumPalillos);

       if (jugada>1)
          printf ("Yo retiro %d palillos.\n",jugada);
       else
          printf ("Yo retiro un palillo.\n");

       NumPalillos -= jugada;

       if (NumPalillos==1) {
          printf ("\nLo siento, ha perdido esta partida.\n");
       } else {
          printf ("Por lo tanto, quedan %d palillos.\n",NumPalillos);
       }
    }
  }

  return 0;
}*/
