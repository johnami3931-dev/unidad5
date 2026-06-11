/*
 * Autor: [Jhon jairo Mina Garces]
 * Asignatura: Estructura de Datos - Ingenieria de Sistemas
 * Taller Unidad 5: Metodos de Ordenamiento y Busqueda Binaria
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CONTENEDORES 30 

// Prototipos de las funciones
void registrarManual(int pesos[], int *n, int *ordenado);
void generarAleatorio(int pesos[], int *n, int *ordenado);
void mostrarArreglo(int pesos[], int n);
void busquedaBinaria(int pesos[], int n, int ordenado);

// Los 3 metodos eficientes con traza de ejecucion paso a paso
void metodoShell(int pesos[], int n);
void metodoQuickSort(int pesos[], int inicio, int fin, int n);
void metodoMergeSort(int pesos[], int izq, int der, int n);
void combinarMerge(int pesos[], int izq, int medio, int der);

int main() {
    int pesos[MAX_CONTENEDORES];
    int n = 0;          
    int ordenado = 0;   
    int opcion;

    srand(time(NULL));

    do {
        printf("\n---------------------------------------------\n");
        printf("   SISTEMA DE ORGANIZACION DE CONTENEDORES   \n");
        printf("               MUELLE DE CARGA               \n");
        printf("---------------------------------------------\n");
        printf("1. Registrar peso de contenedores manualmente\n");
        printf("2. Generar pesos aleatorios de contenedores\n");
        printf("3. Mostrar contenedores registrados\n");
        printf("4. Ordenar contenedores Metodo 1 (ShellSort)\n");
        printf("5. Ordenar contenedores Metodo 2 (QuickSort)\n");
        printf("6. Ordenar contenedores Metodo 3 (MergeSort)\n");
        printf("7. Buscar contenedor\n");
        printf("8. Salir\n");
        printf("---------------------------------------------\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                registrarManual(pesos, &n, &ordenado);
                break;
            case 2:
                generarAleatorio(pesos, &n, &ordenado);
                break;
            case 3:
                mostrarArreglo(pesos, n);
                break;
            case 4:
                if (n == 0) {
                    printf("\n[!] Error: No hay datos cargados.\n");
                } else {
                    printf("\n--- INICIANDO SHELLSORT ---");
                    metodoShell(pesos, n);
                    ordenado = 1; 
                    printf("\n[+] Arreglo ordenado con ShellSort correctamente.\n");
                    mostrarArreglo(pesos, n);
                }
                break;
            case 5:
                if (n == 0) {
                    printf("\n[!] Error: No hay datos cargados.\n");
                } else {
                    printf("\n--- INICIANDO QUICKSORT ---");
                    metodoQuickSort(pesos, 0, n - 1, n);
                    ordenado = 1; 
                    printf("\n[+] Arreglo ordenado con QuickSort correctamente.\n");
                    mostrarArreglo(pesos, n);
                }
                break;
            case 6:
                if (n == 0) {
                    printf("\n[!] Error: No hay datos cargados.\n");
                } else {
                    printf("\n--- INICIANDO MERGESORT ---");
                    metodoMergeSort(pesos, 0, n - 1, n);
                    ordenado = 1; 
                    printf("\n[+] Arreglo ordenado con MergeSort correctamente.\n");
                    mostrarArreglo(pesos, n);
                }
                break;
            case 7:
                busquedaBinaria(pesos, n, ordenado);
                break;
            case 8:
                printf("\nSaliendo del programa...\n");
                break;
            default:
                printf("\n[!] Opcion invalida, intente de nuevo.\n");
        }
    } while(opcion != 8);

    return 0;
}

// --- DESARROLLO DE LAS FUNCIONES ---

void registrarManual(int pesos[], int *n, int *ordenado) {
    int cant;
    printf("\n¿Cuantos contenedores va a ingresar? (Maximo %d): ", MAX_CONTENEDORES);
    scanf("%d", &cant);

    if (cant <= 0 || cant > MAX_CONTENEDORES) {
        printf("[!] Cantidad incorrecta.\n");
        return;
    }

    *n = cant;
    for (int i = 0; i < *n; i++) {
        printf("Ingrese peso del contenedor %d (en kg): ", i + 1);
        scanf("%d", &pesos[i]);
    }
    *ordenado = 0; 
    printf("[+] Datos guardados.\n");
}

void generarAleatorio(int pesos[], int *n, int *ordenado) {
    int cant;
    printf("\n¿Cuantos contenedores quiere generar? (Maximo %d): ", MAX_CONTENEDORES);
    scanf("%d", &cant);

    if (cant <= 0 || cant > MAX_CONTENEDORES) {
        printf("[!] Cantidad incorrecta.\n");
        return;
    }

    *n = cant;
    for (int i = 0; i < *n; i++) {
        pesos[i] = rand() % 29001 + 1000; 
    }
    *ordenado = 0; 
    printf("[+] Pesos aleatorios creados con éxito.\n");
}

void mostrarArreglo(int pesos[], int n) {
    if (n == 0) {
        printf("\n[-] El patio de contenedores esta vacio.\n");
        return;
    }
    printf("\n[ ");
    for (int i = 0; i < n; i++) {
        printf("%d ", pesos[i]);
    }
    printf("]\n");
}

// METODO 1: ShellSort (Muestra cómo se reduce el salto)
void metodoShell(int pesos[], int n) {
    int salto, i, j, aux, paso = 1;
    
    for (salto = n / 2; salto > 0; salto = salto / 2) {
        printf("\nSalto actual: %d", salto);
        for (i = salto; i < n; i++) {
            aux = pesos[i];
            for (j = i; j >= salto && pesos[j - salto] > aux; j = j - salto) {
                pesos[j] = pesos[j - salto];
            }
            pesos[j] = aux;
        }
        printf("\n  Estado %d: ", paso++);
        mostrarArreglo(pesos, n);
    }
}

// METODO 2: QuickSort (Muestra los cambios usando el pivote seleccionado)
void metodoQuickSort(int pesos[], int inicio, int fin, int n) {
    if (inicio < fin) {
        int pivote = pesos[fin]; 
        int i = (inicio - 1);
        int aux;

        printf("\nTomando pivote: %d (Rango indices [%d a %d])", pivote, inicio, fin);

        for (int j = inicio; j <= fin - 1; j++) {
            if (pesos[j] < pivote) {
                i++;
                aux = pesos[i];
                pesos[i] = pesos[j];
                pesos[j] = aux;
            }
        }
        
        aux = pesos[i + 1];
        pesos[i + 1] = pesos[fin];
        pesos[fin] = aux;
        
        int posPivote = i + 1;
        
        printf("\n  Sub-intercambio: ");
        mostrarArreglo(pesos, n);

        metodoQuickSort(pesos, inicio, posPivote - 1, n);
        metodoQuickSort(pesos, posPivote + 1, fin, n);
    }
}

// METODO 3: MergeSort (Muestra cómo se van uniendo/combinando las mitades)
void combinarMerge(int pesos[], int izq, int medio, int der) {
    int i, j, k;
    int tam1 = medio - izq + 1;
    int tam2 = der - medio;

    int ArregloIzq[tam1], ArregloDer[tam2];

    for (i = 0; i < tam1; i++) ArregloIzq[i] = pesos[izq + i];
    for (j = 0; j < tam2; j++) ArregloDer[j] = pesos[medio + 1 + j];

    i = 0; j = 0; k = izq;
    
    while (i < tam1 && j < tam2) {
        if (ArregloIzq[i] <= ArregloDer[j]) {
            pesos[k] = ArregloIzq[i];
            i++;
        } else {
            pesos[k] = ArregloDer[j];
            j++;
        }
        k++;
    }

    while (i < tam1) { pesos[k] = ArregloIzq[i]; i++; k++; }
    while (j < tam2) { pesos[k] = ArregloDer[j]; j++; k++; }
}

void metodoMergeSort(int pesos[], int izq, int der, int n) {
    if (izq < der) {
        int medio = izq + (der - izq) / 2;

        metodoMergeSort(pesos, izq, medio, n);
        metodoMergeSort(pesos, medio + 1, der, n);

        combinarMerge(pesos, izq, medio, der);
        
        printf("\n  Combinando indices [%d al %d]: ", izq, der);
        mostrarArreglo(pesos, n);
    }
}

// BUSQUEDA BINARIA
void busquedaBinaria(int pesos[], int n, int ordenado) {
    if (n == 0) {
        printf("\n[!] No hay ningun contenedor en el patio.\n");
        return;
    }
    if (ordenado == 0) {
        printf("\n[ERROR] No se puede buscar. El patio no esta ordenado por ningun metodo.\n");
        return;
    }

    int buscar;
    printf("\nIngrese el peso exacto a buscar: ");
    scanf("%d", &buscar);

    int i_izq = 0;
    int i_der = n - 1;
    int posicionEncontrada = -1;

    while (i_izq <= i_der) {
        int i_medio = i_izq + (i_der - i_izq) / 2;

        if (pesos[i_medio] == buscar) {
            posicionEncontrada = i_medio;
            break; 
        }
        if (pesos[i_medio] < buscar) {
            i_izq = i_medio + 1; 
        } else {
            i_der = i_medio - 1; 
        }
    }

    if (posicionEncontrada != -1) {
        printf("[+] Encontrado: El peso %d kg esta en la posicion con indice [%d].\n", buscar, posicionEncontrada);
    } else {
        printf("[-] El peso %d kg no existe en este patio.\n", buscar);
    }
}
