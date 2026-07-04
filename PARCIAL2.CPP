#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// ESTRUCTURAS DE DATOS
// ==========================================

// Estructura para la Cola de Usuarios (FIFO)
typedef struct Usuario {
    char nombre[50];
    char documento[20];
    struct Usuario* siguiente;
} Usuario;

typedef struct Cola {
    Usuario* frente;
    Usuario* final;
} Cola;

// Estructura para las Pilas de Trámites (LIFO)
typedef struct Tramite {
    char codigo[10];
    char tipo[50];
    char documentoUsuario[20];
    struct Tramite* siguiente;
} Tramite;

typedef struct Pila {
    Tramite* tope;
} Pila;

// ==========================================
// FUNCIONES DE LA COLA (FIFO)
// ==========================================

void inicializarCola(Cola* c) {
    c->frente = NULL;
    c->final = NULL;
}

void encolarUsuario(Cola* c, char* nombre, char* documento) {
    Usuario* nuevo = (Usuario*)malloc(sizeof(Usuario));
    if (nuevo == NULL) return;
    
    strcpy(nuevo->nombre, nombre);
    strcpy(nuevo->documento, documento);
    nuevo->siguiente = NULL;

    if (c->final == NULL) {
        c->frente = nuevo;
        c->final = nuevo;
    } else {
        c->final->siguiente = nuevo;
        c->final = nuevo;
    }
    printf(">> Usuario %s registrado en la cola de espera.\n", nombre);
}

Usuario* desencolarUsuario(Cola* c) {
    if (c->frente == NULL) return NULL;
    
    Usuario* temporal = c->frente;
    c->frente = c->frente->siguiente;
    
    if (c->frente == NULL) {
        c->final = NULL;
    }
    return temporal;
}

// ==========================================
// FUNCIONES DE LA PILA (LIFO)
// ==========================================

void inicializarPila(Pila* p) {
    p->tope = NULL;
}

void apilarTramite(Pila* p, char* codigo, char* tipo, char* documento) {
    Tramite* nuevo = (Tramite*)malloc(sizeof(Tramite));
    if (nuevo == NULL) return;
    
    strcpy(nuevo->codigo, codigo);
    strcpy(nuevo->tipo, tipo);
    strcpy(nuevo->documentoUsuario, documento);
    
    nuevo->siguiente = p->tope;
    p->tope = nuevo;
}

Tramite* desapilarTramite(Pila* p) {
    if (p->tope == NULL) return NULL;
    
    Tramite* temporal = p->tope;
    p->tope = p->tope->siguiente;
    return temporal;
}

// ==========================================
// PROCESO DE CANCELACIÓN (Pila Auxiliar)
// ==========================================
int cancelarTramite(Pila* p, char* codigo) {
    Pila pilaAux;
    inicializarPila(&pilaAux);
    int encontrado = 0;

    // 1. Desapilar buscando el código del trámite
    while (p->tope != NULL) {
        Tramite* actual = desapilarTramite(p);
        if (strcmp(actual->codigo, codigo) == 0) {
            encontrado = 1;
            printf(">> CONFIRMACION: El tramite con codigo [%s] ha sido cancelado con exito.\n", actual->codigo);
            free(actual); // Liberar memoria del trámite eliminado
            break;        // Detener la búsqueda
        } else {
            // CORRECCIÓN: Primero guardamos los datos en la auxiliar, LUEGO liberamos
            apilarTramite(&pilaAux, actual->codigo, actual->tipo, actual->documentoUsuario);
            free(actual);
        }
    }

    // 2. Devolver los trámites restantes de la pila auxiliar a la original
    while (pilaAux.tope != NULL) {
        Tramite* temp = desapilarTramite(&pilaAux);
        apilarTramite(p, temp->codigo, temp->tipo, temp->documentoUsuario);
        free(temp);
    }

    return encontrado;
}

// Mostrar el contenido actual de las pilas
void mostrarPila(Pila* p, char* nombrePila) {
    printf("\n--- %s ---\n", nombrePila);
    if (p->tope == NULL) {
        printf("[Pila Vacia]\n");
        return;
    }
    Tramite* actual = p->tope;
    while (actual != NULL) {
        printf("Codigo: %s | Tipo: %s | Doc. Usuario: %s\n", actual->codigo, actual->tipo, actual->documentoUsuario);
        actual = actual->siguiente;
    }
}

// ==========================================
// PROGRAMA PRINCIPAL / MENÚ
// ==========================================
int main() {
    Cola colaEspera;
    Pila pFinanciera, pAdministrativa;
    
    inicializarCola(&colaEspera);
    inicializarPila(&pFinanciera);
    inicializarPila(&pAdministrativa);

    int opcion;
    char nom[50], doc[20], cod[10], tipoTramite[50];

    do {
        printf("\n============================================\n");
        printf("       SISTEMA DE ATENCION BANCARIA\n");
        printf("============================================\n");
        printf("1. Registrar usuario en la Cola (Llegada)\n");
        printf("2. Atender usuario en Ventanilla (Registrar Tramite)\n");
        printf("3. Cancelar un tramite por Codigo\n");
        printf("4. Procesar proximo tramite de las Pilas\n");
        printf("5. Visualizar estado de las Pilas\n");
        printf("6. Salir\n");
        printf("Seleccione una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n');
            printf(">> Por favor, ingrese un numero valido.\n");
            continue;
        }
        getchar(); // Limpiar el búfer

        switch(opcion) {
            case 1:
                printf("Ingrese nombre del usuario: ");
                fgets(nom, sizeof(nom), stdin); nom[strcspn(nom, "\n")] = 0;
                printf("Ingrese numero de documento: ");
                fgets(doc, sizeof(doc), stdin); doc[strcspn(doc, "\n")] = 0;
                encolarUsuario(&colaEspera, nom, doc);
                break;

            case 2: {
                Usuario* atendido = desencolarUsuario(&colaEspera);
                if (atendido == NULL) {
                    printf(">> No hay usuarios esperando en la cola.\n");
                } else {
                    printf("\nAtendiendo a: %s (Doc: %s)\n", atendido->nombre, atendido->documento);
                    printf("Ingrese Codigo Unico del Tramite: ");
                    fgets(cod, sizeof(cod), stdin); cod[strcspn(cod, "\n")] = 0;
                    
                    printf("Seleccione Tipo de Tramite:\n");
                    printf("  1. Financiero (Consignaciones, retiros, pagos)\n");
                    printf("  2. Administrativo (Apertura de cuentas, datos, certificados)\n");
                    printf("Opcion: ");
                    int t;
                    scanf("%d", &t);
                    getchar();

                    if (t == 1) {
                        strcpy(tipoTramite, "Financiero");
                        apilarTramite(&pFinanciera, cod, tipoTramite, atendido->documento);
                        printf(">> Tramite Financiero registrado en la pila.\n");
                    } else if (t == 2) {
                        strcpy(tipoTramite, "Administrativo");
                        apilarTramite(&pAdministrativa, cod, tipoTramite, atendido->documento); 
                        printf(">> Tramite Administrativo registrado en la pila.\n");
                    } else {
                        printf(">> Opcion de tramite invalida. El usuario pierde el turno.\n");
                    }
                    free(atendido);
                }
                break;
            }

            case 3:
                printf("Ingrese el codigo del tramite que desea CANCELAR: ");
                fgets(cod, sizeof(cod), stdin); cod[strcspn(cod, "\n")] = 0;
                
                if (!cancelarTramite(&pFinanciera, cod)) {
                    if (!cancelarTramite(&pAdministrativa, cod)) {
                        printf(">> ERROR: La cancelacion no es posible (El tramite no existe o ya fue procesado).\n");
                    }
                }
                break;

            case 4:
                printf("¿De que pila desea procesar (LIFO) el siguiente tramite?\n1. Financiera\n2. Administrativa\nOpcion: ");
                int pOpc;
                scanf("%d", &pOpc);
                getchar();
                
                if (pOpc == 1) {
                    Tramite* t = desapilarTramite(&pFinanciera);
                    if(t) {
                        printf(">> PROCESADO (Financiero) -> Codigo: %s | Usuario: %s\n", t->codigo, t->documentoUsuario);
                        free(t);
                    } else printf(">> La Pila Financiera esta vacia.\n");
                } else if (pOpc == 2) {
                    Tramite* t = desapilarTramite(&pAdministrativa);
                    if(t) {
                        printf(">> PROCESADO (Administrativo) -> Codigo: %s | Usuario: %s\n", t->codigo, t->documentoUsuario);
                        free(t);
                    } else printf(">> La Pila Administrativa esta vacia.\n");
                } else {
                    printf(">> Opcion invalida.\n");
                }
                break;

            case 5:
                mostrarPila(&pFinanciera, "PILA DE TRÁMITES FINANCIEROS (LIFO)");
                mostrarPila(&pAdministrativa, "PILA DE TRÁMITES ADMINISTRATIVOS (LIFO)");
                break;

            case 6:
                printf("Finalizando el programa del Banco...\n");
                break;

            default:
                printf(">> Opcion no valida.\n");
        }
    } while (opcion != 6);

    return 0;
}