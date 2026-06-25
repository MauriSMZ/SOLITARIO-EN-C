#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

typedef struct Carta {
    int color, grupo, estado, valor, visible;
    char impresion[4];
} Carta;

enum Grupos {CORAZON = 3, DIAMANTE, TREBOL, ESPADA};

int victoria = 0;
int puntuacion = 0;

char letra(int n) {
    switch(n) {
        case 1: return 'A';
        case 11: return 'J';
        case 12: return 'Q';
        case 13: return 'K';
        default: return '0';
    }
}

void generar_mazo(Carta Mazo[]) {
    int grupo, valor, c;
    c = 0;
    for(grupo = CORAZON; grupo <= ESPADA; grupo++) {
        for(valor = 1; valor <= 13; valor++) {
            Mazo[c].valor = valor;
            Mazo[c].grupo = grupo;
            Mazo[c].estado = 0;
            Mazo[c].visible = 1;
            if(grupo == CORAZON || grupo == DIAMANTE)
                Mazo[c].color = FOREGROUND_RED | FOREGROUND_INTENSITY;
            else
                Mazo[c].color = 0;

            if(valor > 1 && valor < 11)
                sprintf(Mazo[c].impresion, "%d%c", valor, grupo);
            else
                sprintf(Mazo[c].impresion, "%c%c", letra(valor), grupo);
            c++;
        }
    }
}

void desordenar_mazo(Carta Mazo[], int cantidad) {
    int i;
    for(i = 0; i < cantidad; i++) {
        int j;
        Carta temp;
        j = rand() % cantidad;
        temp = Mazo[i];
        Mazo[i] = Mazo[j];
        Mazo[j] = temp;
    }
}

void imprimir_mazo(Carta Mazo[]) {
    int indice;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for(indice = 0; indice < 52; indice++) {
        SetConsoleTextAttribute(hConsole, Mazo[indice].color | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
        printf("%s\n", Mazo[indice].impresion);
        SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
    }
}

Carta traer_carta(Carta Mazo[]) {
    Carta carta;
    int indice;
    do {
        indice = rand() % 52;
        carta = Mazo[indice];
    } while(Mazo[indice].estado);
    Mazo[indice].estado = 1;
    return carta;
}

void cargar_tablero(Carta Mazo[], int filas, int columnas, Carta tablero[][columnas]) {
    int fila, columna;
    for(fila = 0; fila < 7; fila++) {
        for(columna = 0; columna < 7; columna++) {
            if(columna < fila) {
                tablero[fila][columna].valor = 0;
            } else {
                tablero[fila][columna] = traer_carta(Mazo);
                if(columna > fila) {
                    tablero[fila][columna].visible = 0;
                }
            }
        }
    }
}

void generar_mazo_descartes(Carta Mazo[], Carta Mazo_d[], int *num_cartas_descartes) {
    int indice, contador;
    contador = 0;
    for(indice = 0; indice < 52; indice++) {
        if (!Mazo[indice].estado) {
            Mazo_d[contador++] = Mazo[indice];
        }
    }
    *num_cartas_descartes = contador;
}

void log_tablero(int filas, int columnas, Carta tablero[][columnas], Carta montones_finales[4][13], Carta Mazo_d[], int indice_mazo, int num_cartas_descartes) {
    int i, fila, columna;
    FILE *logfile = fopen("log.txt", "a");
    if (!logfile) {
        perror("Error abriendo el archivo de log");
        return;
    }

    fprintf(logfile, "\nM: ");
    if (indice_mazo < num_cartas_descartes) {
        if (Mazo_d[indice_mazo].grupo == CORAZON || Mazo_d[indice_mazo].grupo == DIAMANTE) {
            fprintf(logfile, "%c%c", Mazo_d[indice_mazo].impresion[0], Mazo_d[indice_mazo].grupo == CORAZON ? 'C' : 'D');
        } else {
            fprintf(logfile, "%c%c", Mazo_d[indice_mazo].impresion[0], Mazo_d[indice_mazo].grupo == TREBOL ? 'T' : 'P');
        }
    }

    fprintf(logfile, "\nP:");
    for (i = 0; i < 13; i++) {
        if (montones_finales[0][i].valor != 0 && montones_finales[0][i].visible) {
            fprintf(logfile, " %c%c ", montones_finales[0][i].impresion[0], montones_finales[0][i].grupo == CORAZON ? 'C' : montones_finales[0][i].grupo == DIAMANTE ? 'D' : montones_finales[0][i].grupo == TREBOL ? 'T' : 'P');
        } else {
            fprintf(logfile, " ");
        }
    }

    fprintf(logfile, "T:");
    for (i = 0; i < 13; i++) {
        if (montones_finales[1][i].valor != 0 && montones_finales[1][i].visible) {
            fprintf(logfile, " %c%c ", montones_finales[1][i].impresion[0], montones_finales[1][i].grupo == CORAZON ? 'C' : montones_finales[1][i].grupo == DIAMANTE ? 'D' : montones_finales[1][i].grupo == TREBOL ? 'T' : 'P');
        } else {
            fprintf(logfile, " ");
        }
    }

    fprintf(logfile, "R:");
    for (i = 0; i < 13; i++) {
        if (montones_finales[2][i].valor != 0 && montones_finales[2][i].visible) {
            fprintf(logfile, " %c%c ", montones_finales[2][i].impresion[0], montones_finales[2][i].grupo == CORAZON ? 'C' : montones_finales[2][i].grupo == DIAMANTE ? 'D' : montones_finales[2][i].grupo == TREBOL ? 'T' : 'P');
        } else {
            fprintf(logfile, " ");
        }
    }

    fprintf(logfile, "S:");
    for (i = 0; i < 13; i++) {
        if (montones_finales[3][i].valor != 0 && montones_finales[3][i].visible) {
            fprintf(logfile, " %c%c ", montones_finales[3][i].impresion[0], montones_finales[3][i].grupo == CORAZON ? 'C' : montones_finales[3][i].grupo == DIAMANTE ? 'D' : montones_finales[3][i].grupo == TREBOL ? 'T' : 'P');
        } else {
            fprintf(logfile, " ");
        }
    }

    fprintf(logfile, "\n\n");
    fprintf(logfile, "   ");
    for (columna = 0; columna < columnas; columna++) {
        fprintf(logfile, "  %c   ", 'A' + columna);
    }
    fprintf(logfile, "\n");

    for (fila = 0; fila < filas; fila++) {
        fprintf(logfile, "%2d ", fila + 1);
        for (columna = 0; columna < columnas; columna++) {
            if (tablero[fila][columna].valor == 0) {
                fprintf(logfile, "      ");
            } else {
                if (tablero[fila][columna].visible) {
                    fprintf(logfile, " %c%c  ", tablero[fila][columna].impresion[0], tablero[fila][columna].grupo == CORAZON ? 'C' : tablero[fila][columna].grupo == DIAMANTE ? 'D' : tablero[fila][columna].grupo == TREBOL ? 'T' : 'P');
                } else {
                    fprintf(logfile, "  #   ");
                }
            }
        }
        fprintf(logfile, "\n");
    }

    fclose(logfile);
}

void imprimir_tablero(int filas, int columnas, Carta tablero[][columnas], Carta montones_finales[4][13], Carta Mazo_d[], int indice_mazo, int num_cartas_descartes) {
    int fila, columna, i;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    system("cls");

    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);

    printf("M: ");

    if (indice_mazo < num_cartas_descartes) {
        if (Mazo_d[indice_mazo].grupo == CORAZON || Mazo_d[indice_mazo].grupo == DIAMANTE) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
        } else {
            SetConsoleTextAttribute(hConsole, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
        }
        printf("%s", Mazo_d[indice_mazo].impresion);
    }
    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);

    printf("\n");
    printf("            P:");
    for (i = 0; i < 13; i++) {
        if (montones_finales[0][i].valor != 0 && montones_finales[0][i].visible) {
            if (montones_finales[0][i].grupo == CORAZON || montones_finales[0][i].grupo == DIAMANTE)
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
            else
                SetConsoleTextAttribute(hConsole, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);

            printf("%3s", montones_finales[0][i].impresion);
            SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
        } else {
            printf("");
        }
    }

    printf("     T:");
    for (i = 0; i < 13; i++) {
        if (montones_finales[1][i].valor != 0 && montones_finales[1][i].visible) {
            if (montones_finales[1][i].grupo == CORAZON || montones_finales[1][i].grupo == DIAMANTE)
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
            else
                SetConsoleTextAttribute(hConsole, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);

            printf(" %3s  ", montones_finales[1][i].impresion);
            SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
        } else {
            printf("");
        }
    }

    printf("     R:");
    for (i = 0; i < 13; i++) {
        if (montones_finales[2][i].valor != 0 && montones_finales[2][i].visible) {
            if (montones_finales[2][i].grupo == CORAZON || montones_finales[2][i].grupo == DIAMANTE)
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
            else
                SetConsoleTextAttribute(hConsole, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);

            printf(" %3s  ", montones_finales[2][i].impresion);
            SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
        } else {
            printf("");
        }
    }

    printf("     S:");
    for (i = 0; i < 13; i++) {
        if (montones_finales[3][i].valor != 0 && montones_finales[3][i].visible) {
            if (montones_finales[3][i].grupo == CORAZON || montones_finales[3][i].grupo == DIAMANTE)
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
            else
                SetConsoleTextAttribute(hConsole, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);

            printf("%3s", montones_finales[3][i].impresion);
            SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
        } else {
            printf("");
        }
    }

    printf("\n");
    printf("\n");
    printf("   ");
    for (columna = 0; columna < columnas; columna++) {
        printf("  %c   ", 'A' + columna);
    }
    printf("\n");

    for (fila = 0; fila < filas; fila++) {
        printf("%2d ", fila + 1);

        for (columna = 0; columna < columnas; columna++) {
            if (tablero[fila][columna].valor == 0) {
                printf("      ");
            } else {
                if (tablero[fila][columna].visible) {
                    if (tablero[fila][columna].grupo == CORAZON || tablero[fila][columna].grupo == DIAMANTE)
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
                    else
                        SetConsoleTextAttribute(hConsole, 0 | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);

                    printf(" %3s  ", tablero[fila][columna].impresion);
                    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
                } else {
                    SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
                    printf("  #   ");
                }
            }
        }
        printf("\n");
    }

    log_tablero(filas, columnas, tablero, montones_finales, Mazo_d, indice_mazo, num_cartas_descartes);
}

void mover_carta_mazo(Carta Mazo_d[], int *indice_mazo, int *num_cartas_descartes) {
    (*indice_mazo)++;
    if (*indice_mazo >= *num_cartas_descartes) {
        *indice_mazo = 0;
        puntuacion -= 50; // Restar 50 puntos al reiniciar el mazo
    }
}

void eliminar_carta_mazo(Carta Mazo_d[], int *indice_mazo, int *num_cartas_descartes) {
    int i;
    for (i = *indice_mazo; i < *num_cartas_descartes - 1; i++) {
        Mazo_d[i] = Mazo_d[i + 1];
    }

    (*num_cartas_descartes)--;

    if (*indice_mazo >= *num_cartas_descartes) {
        *indice_mazo = 0;
    }
}

void mover_mazo_pila(Carta Mazo_d[], int *indice_mazo, int *num_cartas_descartes, Carta montones_finales[][13], char destino, int fila_destino) {
    int columna_destino, i;

    switch (destino) {
        case 'P':
            columna_destino = 0;
            break;
        case 'T':
            columna_destino = 1;
            break;
        case 'R':
            columna_destino = 2;
            break;
        case 'S':
            columna_destino = 3;
            break;
        default:
            printf("Movimiento no valido\n");
            return;
    }

    for (i = 12; i >= 0; --i) {
        if (montones_finales[columna_destino][i].valor != 0) {
            break;
        }
    }

    if (i == -1) {
        if (Mazo_d[*indice_mazo].valor == 1) {
            montones_finales[columna_destino][0] = Mazo_d[*indice_mazo];
            montones_finales[columna_destino][0].visible = 1;
            eliminar_carta_mazo(Mazo_d, indice_mazo, num_cartas_descartes);
            puntuacion += 10; // Sumar 10 puntos por cargar una carta
        } else {
            printf("Movimiento no valido\n");
            getchar();
        }
    } else {
        if (Mazo_d[*indice_mazo].valor == montones_finales[columna_destino][i].valor + 1 &&
            Mazo_d[*indice_mazo].grupo == montones_finales[columna_destino][i].grupo) {

            montones_finales[columna_destino][i].visible = 0;
            montones_finales[columna_destino][i + 1] = Mazo_d[*indice_mazo];
            montones_finales[columna_destino][i + 1].visible = 1;
            eliminar_carta_mazo(Mazo_d, indice_mazo, num_cartas_descartes);
            puntuacion += 10; // Sumar 10 puntos por cargar una carta
        } else {
            printf("Movimiento no valido\n");
            getchar();
        }
    }
}

void mover_pila_tablero(Carta tablero[][7], char origen, Carta montones_finales[4][13], char destino, int fila_destino) {
    int columna_destino = destino - 'A';
    int columna_origen, indice_carta_origen, i, fila_actual;
    Carta carta_origen, carta_destino;

    switch (origen) {
        case 'P':
            columna_origen = 0;
            break;
        case 'T':
            columna_origen = 1;
            break;
        case 'R':
            columna_origen = 2;
            break;
        case 'S':
            columna_origen = 3;
            break;
        default:
            printf("Movimiento no valido\n");
            getchar();
            return;
    }

    indice_carta_origen = -1;

    for (i = 12; i >= 0; --i) {
        if (montones_finales[columna_origen][i].valor != 0) {
            indice_carta_origen = i;
            break;
        }
    }

    if (indice_carta_origen == -1) {
        printf("Movimiento no valido\n");
        getchar();
        return;
    }

    carta_origen = montones_finales[columna_origen][indice_carta_origen];

    fila_actual = fila_destino;
    while (fila_actual >= 0 && tablero[fila_actual][columna_destino].valor == 0) {
        fila_actual--;
    }

    if (fila_actual == -1) {
        tablero[fila_destino][columna_destino] = carta_origen;
        montones_finales[columna_origen][indice_carta_origen].valor = 0;
        montones_finales[columna_origen][indice_carta_origen].visible = 0;
        return;
    } else {
        carta_destino = tablero[fila_actual][columna_destino];
        if (carta_origen.valor == carta_destino.valor - 1 && carta_origen.color != carta_destino.color) {
            tablero[fila_destino][columna_destino] = carta_origen;
            montones_finales[columna_origen][indice_carta_origen].valor = 0;
            montones_finales[columna_origen][indice_carta_origen].visible = 0;
            if (indice_carta_origen > 0) {
                montones_finales[columna_origen][indice_carta_origen - 1].visible = 1;
            }
            return;
        } else {
            printf("Movimiento no valido\n");
            getchar();
            return;
        }
    }
}

void mover_tablero_pila(Carta tablero[][7], char origen, int fila_origen, Carta montones_finales[][13], char destino) {
    int columna_origen = origen - 'A';
    int columna_destino, i, fila_revelar;
    Carta carta_origen;

    switch (destino) {
        case 'P':
            columna_destino = 0;
            break;
        case 'T':
            columna_destino = 1;
            break;
        case 'R':
            columna_destino = 2;
            break;
        case 'S':
            columna_destino = 3;
            break;
        default:
            printf("Movimiento no valido\n");
            getchar();
            return;
    }

    if (tablero[fila_origen - 1][columna_origen].valor == 0) {
        printf("Movimiento no valido\n");
        getchar();
        return;
    }

    carta_origen = tablero[fila_origen - 1][columna_origen];

    for (i = 12; i >= 0; --i) {
        if (montones_finales[columna_destino][i].valor != 0) {
            break;
        }
    }

    if (i == -1) {
        if (carta_origen.valor == 1) {
            montones_finales[columna_destino][0] = carta_origen;
            montones_finales[columna_destino][0].visible = 1;
            puntuacion += 10; // Sumar 10 puntos por cargar una carta
        } else {
            printf("Movimiento no valido\n");
            getchar();
            return;
        }
    } else {
        if (carta_origen.valor == montones_finales[columna_destino][i].valor + 1 &&
            carta_origen.grupo == montones_finales[columna_destino][i].grupo) {

            montones_finales[columna_destino][i].visible = 0;
            montones_finales[columna_destino][i + 1] = carta_origen;
            montones_finales[columna_destino][i + 1].visible = 1;
            puntuacion += 10; // Sumar 10 puntos por cargar una carta
        } else {
            printf("Movimiento no valido\n");
            getchar();
            return;
        }
    }

    tablero[fila_origen - 1][columna_origen].valor = 0;

    fila_revelar = fila_origen - 2;
    while (fila_revelar >= 0) {
        if (tablero[fila_revelar][columna_origen].valor != 0) {
            tablero[fila_revelar][columna_origen].visible = 1;
            break;
        }
        fila_revelar--;
    }
}

void mover_carta_tablero(Carta Mazo_d[], int *indice_mazo, int *num_cartas_descartes, Carta tablero[][7], char destino, int fila_destino) {
    int columna_destino = destino - 'A';
    int fila, fila_anterior;
    Carta carta_destino, carta_mazo;
    int colores_diferentes, valor_correcto;

    if (espacio_vacio(tablero, 0, columna_destino)) {
        tablero[0][columna_destino] = Mazo_d[*indice_mazo];
        tablero[0][columna_destino].visible = 1;
        eliminar_carta_mazo(Mazo_d, indice_mazo, num_cartas_descartes);
        return;
    }

    for (fila = 0; fila < 19; fila++) {
        if (espacio_vacio(tablero, fila, columna_destino)) {
            fila_anterior = fila - 1;
            if (fila_anterior >= 0 && tablero[fila_anterior][columna_destino].valor != 0) {
                carta_destino = tablero[fila_anterior][columna_destino];
                carta_mazo = Mazo_d[*indice_mazo];

                colores_diferentes = ((carta_destino.grupo == CORAZON || carta_destino.grupo == DIAMANTE) &&
                                      (carta_mazo.grupo == TREBOL || carta_mazo.grupo == ESPADA)) ||
                                     ((carta_destino.grupo == TREBOL || carta_destino.grupo == ESPADA) &&
                                      (carta_mazo.grupo == CORAZON || carta_mazo.grupo == DIAMANTE));

                valor_correcto = (carta_mazo.valor == carta_destino.valor - 1);

                if (valor_correcto && colores_diferentes) {
                    tablero[fila][columna_destino] = carta_mazo;
                    tablero[fila][columna_destino].visible = 1;
                    eliminar_carta_mazo(Mazo_d, indice_mazo, num_cartas_descartes);
                    return;
                }
            } else if (fila_anterior < 0) {
                tablero[fila][columna_destino] = Mazo_d[*indice_mazo];
                tablero[fila][columna_destino].visible = 1;
                eliminar_carta_mazo(Mazo_d, indice_mazo, num_cartas_descartes);
                return;
            }

            printf("Movimiento invalido\n");
            getchar();
            return;
        }
    }

    printf("Movimiento invalido\n");
    getchar();
}

void mover_carta_columna(Carta tablero[][7], char origen, int fila_origen, char destino, int fila_destino) {
    int columna_origen = origen - 'A';
    int columna_destino = destino - 'A';
    int fila, i;
    Carta carta_origen;

    if (tablero[fila_origen - 1][columna_origen].valor == 0) {
        printf("Movimiento no valido\n");
        getchar();
        return;
    }

    carta_origen = tablero[fila_origen - 1][columna_origen];
    if (!carta_origen.visible) {
        printf("Movimiento no valido\n");
        getchar();
        return;
    }

    if (fila_destino == -1) {
        for (fila = 0; fila < 19; fila++) {
            if (tablero[fila][columna_destino].valor == 0) {
                fila_destino = fila + 1;
                break;
            }
        }
    }

    if (fila_destino > 0 && tablero[fila_destino - 1][columna_destino].valor == 0) {
        i = 0;
        while (tablero[fila_origen - 1 + i][columna_origen].valor != 0) {
            tablero[fila_destino - 1 + i][columna_destino] = tablero[fila_origen - 1 + i][columna_origen];
            tablero[fila_origen - 1 + i][columna_origen].valor = 0;
            tablero[fila_destino - 1 + i][columna_destino].visible = 1;
            i++;
        }

        if (fila_origen > 1 && !tablero[fila_origen - 2][columna_origen].visible) {
            tablero[fila_origen - 2][columna_origen].visible = 1;
        }

    } else {
        printf("Movimiento no valido\n");
        getchar();
        return;
    }
}

int espacio_vacio(Carta tablero[][7], int fila, int columna) {
    return (tablero[fila][columna].valor == 0);
}

int encuentra_guion(char string[]) {
    int pos, len;
    pos = 0;
    len = strlen(string);
    while ((string[pos] != '-') && (len > 1)) {
        pos++;
        len--;
    }
    return pos;
}

void capturar(char string[]) {
    printf("Introduce tu movimiento: ");
    scanf("%s", string);
    getchar();
}

int verificar_victoria(Carta montones_finales[4][13]) {
    int i; // Declarar 'i' aquí
    for (i = 0; i < 4; i++) {
        if (montones_finales[i][12].valor != 13) {
            return 0;
        }
    }
    return 1;
}

void imprimir_puntuacion() {
    printf("Puntuacion: %d\n", puntuacion);
}

void parsear_movimiento(const char* movimiento, char* origen, int* fila_origen, char* destino, int* fila_destino) {
    sscanf(movimiento, "%c%d-%c%d", origen, fila_origen, destino, fila_destino);
}

int main() {
    int fila, columna;
    char movimiento[8];
    Carta Mazo[52], Mazo_d[52], tablero[19][7];
    Carta montones_finales[4][13] = {{{0}}};
    int indice_mazo = 0;
    int num_cartas_descartes;

    system("color 70");
    srand(time(NULL));

    generar_mazo(Mazo);
    desordenar_mazo(Mazo, 52);

    for (fila = 0; fila < 19; fila++)
        for (columna = 0; columna < 7; columna++)
            tablero[fila][columna].valor = 0;

    cargar_tablero(Mazo, 19, 7, tablero);
    generar_mazo_descartes(Mazo, Mazo_d, &num_cartas_descartes);
    imprimir_tablero(19, 7, tablero, montones_finales, Mazo_d, indice_mazo, num_cartas_descartes);

    while (1) {
        capturar(movimiento);

        if (strcmp(movimiento, "M") == 0) {
            mover_carta_mazo(Mazo_d, &indice_mazo, &num_cartas_descartes);
        } else if (strlen(movimiento) == 3 && movimiento[0] == 'M' && (movimiento[2] == 'P' || movimiento[2] == 'T' || movimiento[2] == 'R' || movimiento[2] == 'S')) {
            int columna_destino;

            switch (movimiento[2]) {
                case 'P':
                    columna_destino = 0;
                    break;
                case 'T':
                    columna_destino = 1;
                    break;
                case 'R':
                    columna_destino = 2;
                    break;
                case 'S':
                    columna_destino = 3;
                    break;
                default:
                    printf("Movimiento no válido\n");
                    return 1;
            }

            mover_mazo_pila(Mazo_d, &indice_mazo, &num_cartas_descartes, montones_finales, movimiento[2], columna_destino);
        } else if (strlen(movimiento) == 4 && movimiento[0] == 'M') {
            char destino;
            int fila_destino;
            sscanf(movimiento, "M-%c%d", &destino, &fila_destino);
            mover_carta_tablero(Mazo_d, &indice_mazo, &num_cartas_descartes, tablero, destino, fila_destino);
        } else if (strlen(movimiento) >= 5) {
            char origen, destino;
            int fila_origen, fila_destino;
            parsear_movimiento(movimiento, &origen, &fila_origen, &destino, &fila_destino);
            mover_carta_columna(tablero, origen, fila_origen, destino, fila_destino);
        } else if (strlen(movimiento) == 4 && (movimiento[0] == 'P' || movimiento[0] == 'T' || movimiento[0] == 'R' || movimiento[0] == 'S')) {
            char origen = movimiento[0];
            char destino;
            int fila_destino;
            sscanf(movimiento, "%*c-%c%d", &destino, &fila_destino);
            mover_pila_tablero(tablero, origen, montones_finales, destino, fila_destino - 1);

        } else if (strlen(movimiento) == 4 && (movimiento[3] == 'P' || movimiento[3] == 'T' || movimiento[3] == 'R' || movimiento[3] == 'S')) {
            char origen = movimiento[0];
            int fila_origen;
            char destino = movimiento[3];
            sscanf(movimiento, "%*c%d-%c", &fila_origen, &destino);
            mover_tablero_pila(tablero, origen, fila_origen, montones_finales, destino);
        }

        imprimir_tablero(19, 7, tablero, montones_finales, Mazo_d, indice_mazo, num_cartas_descartes);
        imprimir_puntuacion(); // Imprimir la puntuación actual

        if (verificar_victoria(montones_finales)) {
            printf("¡Felicidades! Has ganado el juego.\n");
            victoria = 1;
            break;
        }

        if (victoria) {
            break;
        }
    }

    return 0;
}
