#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"

// Definición de tu nodo centinela NIL original
NodoViaje T_NIL_NODE = {0, 0, 0, NEGRO, NULL, NULL, NULL};
NodoViaje* NIL = &T_NIL_NODE;

// ============================================================================
// GESTIÓN DE DESTINOS
// ============================================================================
NodoDestino* registrarDestino(NodoDestino* cabeza) {
    NodoDestino* nuevo = (NodoDestino*)malloc(sizeof(NodoDestino)); 
    if (!nuevo) {
        printf("Error: Sin memoria para crear el destino.\n");
        return cabeza;
    }

    printf("Ingrese el codigo del destino (entero): "); 
    scanf("%d", &nuevo->codigo_destino);
    while (getchar() != '\n'); 

    NodoDestino* aux = cabeza;
    while (aux != NULL) {
        if (aux->codigo_destino == nuevo->codigo_destino) {
            printf("Error: Ya existe un destino registrado con ese codigo.\n");
            free(nuevo);
            return cabeza;
        }
        aux = aux->siguiente;
    }

    printf("Ingrese el nombre del destino: "); 
    fgets(nuevo->nombre_destino, 32, stdin);
    nuevo->nombre_destino[strcspn(nuevo->nombre_destino, "\n")] = 0; 

    printf("Ingrese el nombre de la empresa que lo gestiona: "); 
    fgets(nuevo->empresa_gestion, 50, stdin);
    nuevo->empresa_gestion[strcspn(nuevo->empresa_gestion, "\n")] = 0;

    nuevo->cola_espera.frente = NULL; 
    nuevo->cola_espera.final = NULL;
    nuevo->raiz_viajes = NIL; 
    nuevo->siguiente = NULL;

    if (cabeza == NULL) {
        cabeza = nuevo;
    } else {
        aux = cabeza;
        while (aux->siguiente != NULL) {
            aux = aux->siguiente;
        }
        aux->siguiente = nuevo;
    }
    printf("¡Destino '%s' registrado exitosamente!\n", nuevo->nombre_destino);
    return cabeza;
}

NodoDestino* buscarDestinoPorCodigo(NodoDestino* cabeza, int codigo) { 
    NodoDestino* aux = cabeza;
    while (aux != NULL) {
        if (aux->codigo_destino == codigo) return aux;
        aux = aux->siguiente;
    }
    return NULL;
}

void mostrarTodosLosDestinos(NodoDestino* cabeza) {
    if (cabeza == NULL) {
        printf("No hay destinos registrados en el sistema.\n");
        return;
    }
    NodoDestino* aux = cabeza;
    printf("\n--- DESTINOS REGISTRADOS ---\n");
    while (aux != NULL) {
        printf("Codigo: %d | Nombre: %s | Empresa: %s\n", 
               aux->codigo_destino, aux->nombre_destino, aux->empresa_gestion);
        aux = aux->siguiente;
    }
}

// ============================================================================
// REGISTRO Y CONTROL DE PASAJEROS (FIFO)
// ============================================================================
int existePasajeroGlobal(NodoDestino* cabeza, int doc) {
    NodoDestino* destAux = cabeza;
    while (destAux != NULL) {
        NodoPasajero* pasAux = destAux->cola_espera.frente; 
        while (pasAux != NULL) {
            if (pasAux->documento == doc) return 1; 
            pasAux = pasAux->siguiente;
        }
        destAux = destAux->siguiente;
    }
    return 0; 
}

void registrarPasajero(NodoDestino* cabeza) {
    if (cabeza == NULL) {
        printf("Error: Debe registrar al menos un destino antes de agregar pasajeros.\n");
        return;
    }

    int doc, codDest, tDoc;
    printf("Ingrese el numero de documento (entero): "); 
    scanf("%d", &doc);

    if (existePasajeroGlobal(cabeza, doc)) {
        printf("Error: El pasajero con documento %d ya se encuentra registrado en una cola.\n", doc);
        return;
    }

    printf("Seleccione Tipo Documento (1.Cedula, 2.Pasaporte, 3.Tarjeta Identidad): "); 
    scanf("%d", &tDoc);
    if (tDoc < 1 || tDoc > 3) {
        printf("Error: Tipo de documento invalido. Registro rechazado.\n"); 
        return;
    }

    mostrarTodosLosDestinos(cabeza);
    printf("Ingrese el codigo del destino de viaje: "); 
    scanf("%d", &codDest);

    NodoDestino* destino = buscarDestinoPorCodigo(cabeza, codDest);
    if (destino == NULL) {
        printf("Error: El destino seleccionado no existe.\n");
        return;
    }

    NodoPasajero* nuevo = (NodoPasajero*)malloc(sizeof(NodoPasajero));
    if (!nuevo) {
        printf("Error en asignacion de memoria.\n");
        return;
    }
    nuevo->documento = doc;
    nuevo->tipo_documento = (TipoDoc)tDoc;
    nuevo->estado = EN_ESPERA; 
    nuevo->siguiente = NULL;

    if (destino->cola_espera.frente == NULL) { 
        destino->cola_espera.frente = nuevo; 
    } else {
        destino->cola_espera.final->siguiente = nuevo;
    }
    destino->cola_espera.final = nuevo; 

    printf("¡Pasajero encolado con exito para %s!\n", destino->nombre_destino);
}

void mostrarPasajerosPorDestino(NodoDestino* cabeza) {
    if (cabeza == NULL) {
        printf("No hay destinos en el sistema.\n");
        return;
    }
    int codigo;
    printf("Ingrese el codigo del destino a consultar: ");
    scanf("%d", &codigo);

    NodoDestino* dest = buscarDestinoPorCodigo(cabeza, codigo);
    if (!dest) {
        printf("Destino no encontrado.\n");
        return;
    }

    NodoPasajero* aux = dest->cola_espera.frente; 
    printf("\n--- PASAJEROS EN FILA PARA: %s ---\n", dest->nombre_destino);
    if (aux == NULL) {
        printf("Cola vacia. No hay pasajeros esperando.\n"); 
        return;
    }

    while (aux != NULL) {
        printf("Doc: %d | Estado: %s\n", aux->documento, 
               (aux->estado == EN_ESPERA) ? "En Espera" : "Embarcado");
        aux = aux->siguiente;
    }
}

void realizarEmbarque(NodoDestino* cabeza) {
    if (cabeza == NULL) {
        printf("No hay destinos configurados.\n");
        return;
    }
    int codDest, codViaje;
    printf("Ingrese el codigo del destino donde se hara el embarque: ");
    scanf("%d", &codDest);

    NodoDestino* dest = buscarDestinoPorCodigo(cabeza, codDest);
    if (!dest) {
        printf("Destino no encontrado.\n");
        return;
    }

    if (dest->cola_espera.frente == NULL) { 
        printf("No hay pasajeros en fila esperando para este destino.\n");
        return;
    }

    printf("Ingrese el codigo del viaje asignado para abordar: ");
    scanf("%d", &codViaje);
    NodoViaje* viaje = buscarViaje(dest->raiz_viajes, codViaje);
    if (viaje == NIL) {
        printf("Error: Ese codigo de viaje no esta programado para este destino.\n");
        return;
    }

    if (viaje->pasajeros_embarcados >= viaje->capacidad_maxima) {
        printf("Error: El viaje se encuentra al limite de su capacidad maxima.\n");
        return;
    }

    NodoPasajero* pasajeroAEmbarcar = dest->cola_espera.frente; 
    dest->cola_espera.frente = dest->cola_espera.frente->siguiente; 

    if (dest->cola_espera.frente == NULL) { 
        dest->cola_espera.final = NULL; 
    }

    pasajeroAEmbarcar->estado = EMBARCADO; 
    viaje->pasajeros_embarcados++;

    printf("¡Pasajero con Doc: %d ha sido exitosamente embarcado en el viaje %d!\n", 
           pasajeroAEmbarcar->documento, viaje->codigo_viaje);

    free(pasajeroAEmbarcar); 
}

void consultarPasajeroUbicacion(NodoDestino* cabeza) {
    int doc;
    printf("Ingrese el numero de documento a consultar: ");
    scanf("%d", &doc);

    NodoDestino* destAux = cabeza;
    while (destAux != NULL) {
        NodoPasajero* pasAux = destAux->cola_espera.frente; 
        while (pasAux != NULL) {
            if (pasAux->documento == doc) {
                printf("Pasajero encontrado: Doc %d | Destino: %s | Estado: %s\n",
                       doc, destAux->nombre_destino, 
                       (pasAux->estado == EN_ESPERA) ? "En Espera" : "Embarcado");
                return;
            }
            pasAux = pasAux->siguiente;
        }
        destAux = destAux->siguiente;
    }
    printf("El pasajero con documento %d no se encuentra registrado en el terminal.\n", doc);
}

// ============================================================================
// ÁRBOL BALANCEADO ROJO-NEGRO (GESTIÓN DE VIAJES)
// ============================================================================
void rotarIzquierda(NodoViaje** raiz, NodoViaje* x) {
    NodoViaje* y = x->derecho;
    x->derecho = y->izquierdo;
    if (y->izquierdo != NIL) {
        y->izquierdo->padre = x;
    }
    y->padre = x->padre;
    if (x->padre == NIL) {
        *raiz = y;
    } else if (x == x->padre->izquierdo) {
        x->padre->izquierdo = y;
    } else {
        x->padre->derecho = y;
    }
    y->izquierdo = x;
    x->padre = y;
}

void rotarDerecha(NodoViaje** raiz, NodoViaje* y) {
    NodoViaje* x = y->izquierdo;
    y->izquierdo = x->derecho;
    if (x->derecho != NIL) {
        x->derecho->padre = y;
    }
    x->padre = y->padre;
    if (y->padre == NIL) {
        *raiz = x;
    } else if (y == y->padre->derecho) {
        y->padre->derecho = x;
    } else {
        y->padre->izquierdo = x;
    }
    x->derecho = y;
    y->padre = x;
}

void balancearInsertarRojoNegro(NodoViaje** raiz, NodoViaje* z) {
    while (z->padre->color == ROJO) {
        if (z->padre == z->padre->padre->izquierdo) {
            NodoViaje* y = z->padre->padre->derecho; 
            if (y->color == ROJO) {
                z->padre->color = NEGRO;
                y->color = NEGRO;
                z->padre->padre->color = ROJO;
                z = z->padre->padre;
            } else {
                if (z == z->padre->derecho) {
                    z = z->padre;
                    rotarIzquierda(raiz, z);
                }
                z->padre->color = NEGRO;
                z->padre->padre->color = ROJO;
                rotarDerecha(raiz, z->padre->padre);
            }
        } else { 
            NodoViaje* y = z->padre->padre->izquierdo;
            if (y->color == ROJO) {
                z->padre->color = NEGRO;
                y->color = NEGRO;
                z->padre->padre->color = ROJO;
                z = z->padre->padre;
            } else {
                if (z == z->padre->izquierdo) {
                    z = z->padre;
                    rotarDerecha(raiz, z);
                }
                z->padre->color = NEGRO;
                z->padre->padre->color = ROJO;
                rotarIzquierda(raiz, z->padre->padre);
            }
        }
    }
    (*raiz)->color = NEGRO; 
}

void registrarViajeProgramado(NodoDestino* cabeza) {
    if (cabeza == NULL) {
        printf("Error: No existen destinos registrados en el terminal.\n");
        return;
    }
    int codDest, codViaje, capMax;
    mostrarTodosLosDestinos(cabeza);
    printf("Ingrese el codigo del destino para asociar el viaje: ");
    scanf("%d", &codDest);

    NodoDestino* dest = buscarDestinoPorCodigo(cabeza, codDest);
    if (!dest) {
        printf("Destino no encontrado.\n");
        return;
    }

    printf("Ingrese el codigo unico del viaje (entero): "); 
    scanf("%d", &codViaje);
    
    if (buscarViaje(dest->raiz_viajes, codViaje) != NIL) {
        printf("Error: El codigo de viaje %d ya existe para este destino.\n", codViaje);
        return;
    }

    printf("Ingrese la capacidad maxima de pasajeros para la embarcacion: "); 
    scanf("%d", &capMax);

    NodoViaje* nuevo = (NodoViaje*)malloc(sizeof(NodoViaje));
    nuevo->codigo_viaje = codViaje;
    nuevo->capacidad_maxima = capMax;
    nuevo->pasajeros_embarcados = 0;
    nuevo->color = ROJO; 
    nuevo->izquierdo = NIL;
    nuevo->derecho = NIL;
    nuevo->padre = NIL;

    NodoViaje* y = NIL;
    NodoViaje* x = dest->raiz_viajes;

    while (x != NIL) {
        y = x;
        if (nuevo->codigo_viaje < x->codigo_viaje)
            x = x->izquierdo;
        else
            x = x->derecho;
    }

    nuevo->padre = y;
    if (y == NIL) {
        dest->raiz_viajes = nuevo; 
    } else if (nuevo->codigo_viaje < y->codigo_viaje) {
        y->izquierdo = nuevo;
    } else {
        y->derecho = nuevo;
    }

    balancearInsertarRojoNegro(&(dest->raiz_viajes), nuevo);
    printf("¡Viaje %d registrado y arbol balanceado de forma correcta!\n", codViaje);
}

NodoViaje* buscarViaje(NodoViaje* raiz, int codigo) {
    if (raiz == NIL || codigo == raiz->codigo_viaje) return raiz;
    if (codigo < raiz->codigo_viaje) return buscarViaje(raiz->izquierdo, codigo);
    return buscarViaje(raiz->derecho, codigo);
}

void buscarViajeMenu(NodoDestino* cabeza) {
    int codigo;
    printf("Ingrese el codigo de viaje a buscar de manera global: ");
    scanf("%d", &codigo);

    NodoDestino* aux = cabeza;
    while (aux != NULL) {
        NodoViaje* viaje = buscarViaje(aux->raiz_viajes, codigo);
        if (viaje != NIL) {
            printf("Viaje encontrado -> Codigo: %d | Capacidad Max: %d | Abordados: %d | Color Nodo: %s (Destino: %s)\n",
                   viaje->codigo_viaje, viaje->capacidad_maxima, viaje->pasajeros_embarcados,
                   (viaje->color == ROJO) ? "ROJO" : "NEGRO", aux->nombre_destino);
            return;
        }
        aux = aux->siguiente;
    }
    printf("Viaje con codigo %d no encontrado en el sistema.\n", codigo);
}

// Vista gráfica jerárquica lateral 2D (Sustituye al In-Order plano para ver rotaciones)
void mostrarArbolViajes2D(NodoViaje* raiz, int espacio) {
    if (raiz == NIL) return;
    espacio += 8;
    
    mostrarArbolViajes2D(raiz->derecho, espacio);
    printf("\n");
    for (int i = 8; i < espacio; i++) printf(" ");
    printf("[%d:%s]\n", raiz->codigo_viaje, (raiz->color == ROJO) ? "R" : "N");
    mostrarArbolViajes2D(raiz->izquierdo, espacio);
}

void mostrarArbolViajesMenu(NodoDestino* cabeza) {
    int codigo;
    mostrarTodosLosDestinos(cabeza);
    printf("Ingrese el codigo del destino para ver su arbol de viajes: ");
    scanf("%d", &codigo);

    NodoDestino* dest = buscarDestinoPorCodigo(cabeza, codigo);
    if (!dest) {
        printf("Destino no encontrado.\n");
        return;
    }
    printf("\n--- ESTRUCTURA DEL ARBOL DE VIAJES (2D) PARA %s ---\n", dest->nombre_destino);
    if (dest->raiz_viajes == NIL) {
        printf("No hay viajes programados para este destino.\n");
        return;
    }
    mostrarArbolViajes2D(dest->raiz_viajes, 0);
}

// ============================================================================
// CÁLCULO DE ESTADÍSTICAS OBLIGATORIAS
// ============================================================================
void contarPasajerosArbol(NodoViaje* raiz, int* total) {
    if (raiz != NIL) {
        contarPasajerosArbol(raiz->izquierdo, total);
        *total += raiz->pasajeros_embarcados;
        contarPasajerosArbol(raiz->derecho, total);
    }
}

void calcularEstadisticas(NodoDestino* cabeza) {
    if (cabeza == NULL) {
        printf("No hay datos suficientes para generar estadisticas.\n");
        return;
    }

    int totalDestinos = 0;
    int totalPasajerosEsperaGlobal = 0;
    int totalPasajerosEmbarcadosGlobal = 0;

    NodoDestino* maxEsperaDest = cabeza;
    NodoDestino* minEsperaDest = cabeza;
    int maxEspera = -1, minEspera = 999999;

    NodoDestino* aux = cabeza;
    printf("\n====== REPORTE ESTADISTICO GENERAL ======\n");

    while (aux != NULL) {
        totalDestinos++;
        
        int pasajerosEsperaDestino = 0;
        NodoPasajero* p = aux->cola_espera.frente; 
        while (p != NULL) {
            pasajerosEsperaDestino++;
            p = p->siguiente;
        }
        totalPasajerosEsperaGlobal += pasajerosEsperaDestino;

        int pasajerosEmbarcadosDestino = 0;
        contarPasajerosArbol(aux->raiz_viajes, &pasajerosEmbarcadosDestino);
        totalPasajerosEmbarcadosGlobal += pasajerosEmbarcadosDestino;

        if (pasajerosEsperaDestino > maxEspera) {
            maxEspera = pasajerosEsperaDestino;
            maxEsperaDest = aux;
        }
        if (pasajerosEsperaDestino < minEspera) {
            minEspera = pasajerosEsperaDestino;
            minEsperaDest = aux;
        }

        aux = aux->siguiente;
    }

    printf("1. Promedio de pasajeros en espera por destino: %.2f\n", (float)totalPasajerosEsperaGlobal / totalDestinos);
    printf("2. Cantidad total de pasajeros registrados (En Espera): %d\n", totalPasajerosEsperaGlobal);
    printf("3. Cantidad total de pasajeros embarcados: %d\n", totalPasajerosEmbarcadosGlobal);
    printf("4. Destino con MAYOR pasajeros en espera: %s (%d pasajeros)\n", maxEsperaDest->nombre_destino, maxEspera);
    printf("5. Destino con MENOR pasajeros en espera: %s (%d pasajeros)\n", minEsperaDest->nombre_destino, minEspera);
    printf("==========================================\n");
}

// ============================================================================
// LIMPIEZA Y LIBERACIÓN DE MEMORIA (EVITAR FUGAS)
// ============================================================================
void liberarArbol(NodoViaje* raiz) {
    if (raiz != NIL) {
        liberarArbol(raiz->izquierdo);
        liberarArbol(raiz->derecho);
        free(raiz); 
    }
}

void liberarTodo(NodoDestino* cabeza) {
    NodoDestino* auxDest = cabeza;
    while (auxDest != NULL) {
        NodoDestino* siguienteDestino = auxDest->siguiente;

        NodoPasajero* auxPas = auxDest->cola_espera.frente; 
        while (auxPas != NULL) {
            NodoPasajero* siguientePasajero = auxPas->siguiente;
            free(auxPas);
            auxPas = siguientePasajero;
        }

        liberarArbol(auxDest->raiz_viajes);
        free(auxDest);
        auxDest = siguienteDestino;
    }
}