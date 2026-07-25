#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

// ============================================================================
// DEFINICIÓN DE ESTRUCTURAS DE DATOS (Tus estructuras originales)
// ============================================================================

typedef enum { CEDULA = 1, PASAPORTE, TARJETA_IDENTIDAD } TipoDoc;
typedef enum { EN_ESPERA, EMBARCADO } EstadoPasajero;
typedef enum { ROJO, NEGRO } Color; 

typedef struct NodoPasajero {
    int documento;                  
    TipoDoc tipo_documento;         
    EstadoPasajero estado;          
    struct NodoPasajero* siguiente; 
} NodoPasajero;

typedef struct {
    NodoPasajero* frente; 
    NodoPasajero* final;  
} ColaPasajeros;

typedef struct NodoViaje {
    int codigo_viaje;          
    int capacidad_maxima;      
    int pasajeros_embarcados;  
    Color color;               
    struct NodoViaje* izquierdo;
    struct NodoViaje* derecho;
    struct NodoViaje* padre;   
} NodoViaje;

typedef struct NodoDestino {
    int codigo_destino;             
    char nombre_destino[32];        
    char empresa_gestion[50];       
    ColaPasajeros cola_espera;     
    NodoViaje* raiz_viajes;        
    struct NodoDestino* siguiente; 
} NodoDestino;

// Declaración externa del centinela NIL para evitar duplicaciones al enlazar
extern NodoViaje* NIL;

// Prototipos de funciones de tu código
NodoDestino* registrarDestino(NodoDestino* cabeza);
NodoDestino* buscarDestinoPorCodigo(NodoDestino* cabeza, int codigo);
void mostrarTodosLosDestinos(NodoDestino* cabeza);
int existePasajeroGlobal(NodoDestino* cabeza, int doc);
void registrarPasajero(NodoDestino* cabeza);
void mostrarPasajerosPorDestino(NodoDestino* cabeza);
void realizarEmbarque(NodoDestino* cabeza);
void consultarPasajeroUbicacion(NodoDestino* cabeza);
void rotarIzquierda(NodoViaje** raiz, NodoViaje* x);
void rotarDerecha(NodoViaje** raiz, NodoViaje* y);
void balancearInsertarRojoNegro(NodoViaje** raiz, NodoViaje* z);
void registrarViajeProgramado(NodoDestino* cabeza);
NodoViaje* buscarViaje(NodoViaje* raiz, int codigo);
void buscarViajeMenu(NodoDestino* cabeza);
void mostrarArbolViajes2D(NodoViaje* raiz, int espacio);
void mostrarArbolViajesMenu(NodoDestino* cabeza);
void calcularEstadisticas(NodoDestino* cabeza);
void contarPasajerosArbol(NodoViaje* raiz, int* total);
void liberarArbol(NodoViaje* raiz);
void liberarTodo(NodoDestino* cabeza);

#endif