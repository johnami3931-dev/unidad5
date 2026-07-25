#include <stdio.h>
#include "estructuras.h"

int main() {
    NodoDestino* lista_destinos = NULL; 
    int opcion;

    do {
        printf("\n=============================================\n");
        printf("  SISTEMA DE GESTION - TERMINAL PORTUARIO\n");
        printf("=============================================\n");
        printf("1. Registrar destino\n");              
        printf("2. Registrar pasajero\n");             
        printf("3. Mostrar pasajeros por destino\n");   
        printf("4. Registrar viaje programado\n");      
        printf("5. Buscar viaje\n");                   
        printf("6. Mostrar arbol de viajes\n");         
        printf("7. Realizar embarque\n");               
        printf("8. Consultar ubicacion de pasajero\n"); 
        printf("9. Ver Reportes Estadisticos\n");       
        printf("0. Salir\n");                           
        printf("=============================================\n");
        printf("Seleccione una opcion: ");
        if (scanf("%d", &opcion) != 1) {
            printf("Entrada invalida.\n");
            while (getchar() != '\n'); 
            opcion = -1;
            continue;
        }

        switch (opcion) {
            case 1: lista_destinos = registrarDestino(lista_destinos); break; 
            case 2: registrarPasajero(lista_destinos); break;                 
            case 3: mostrarPasajerosPorDestino(lista_destinos); break;        
            case 4: registrarViajeProgramado(lista_destinos); break;          
            case 5: buscarViajeMenu(lista_destinos); break;
            case 6: mostrarArbolViajesMenu(lista_destinos); break;
            case 7: realizarEmbarque(lista_destinos); break;                  
            case 8: consultarPasajeroUbicacion(lista_destinos); break;        
            case 9: calcularEstadisticas(lista_destinos); break;              
            case 0: printf("Liberando memoria y cerrando programa...\n"); break; 
            default: printf("Opcion invalida. Intente de nuevo.\n"); break;
        }
    } while (opcion != 0); 

    liberarTodo(lista_destinos); 
    return 0;
}