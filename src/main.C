#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

// Estructura del cliente
struct Client {
    char *name;
    char *apellido;
    char *num_account;
    char *money;
    char *type_investment;
    char *password;
};

// Prototipos
int registrar();
void guardarClienteComoJSON(struct Client client, const char *nombreArchivo);
void asignarBufferAVariable(char *buffer, char **variable);

/* CICLO PRINCIPAL DE PROGRAMA */
int main() {
    while (1) {
        printf("Bienvenido al sistema de registro de clientes\n");
        printf("1. Registrar cliente\n");
        printf("2. Salir\n");
        printf("Seleccione una opcion: ");

        int opcion;
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                registrar();
                break;
            case 2:
                printf("Saliendo del programa...\n");
                return 0;
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                break;
        }
    }
}

/* GUARDAR DATOS JSON USANDO cJSON */
void guardarClienteComoJSON(struct Client client, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    cJSON *clientes_json = NULL;

    if (archivo) {
        fseek(archivo, 0, SEEK_END);
        long length = ftell(archivo);
        fseek(archivo, 0, SEEK_SET);

        char *contenido = (char *)malloc(length + 1);
        fread(contenido, 1, length, archivo);
        contenido[length] = '\0';
        fclose(archivo);

        clientes_json = cJSON_Parse(contenido);
        free(contenido);
    }

    if (!clientes_json) {
        clientes_json = cJSON_CreateArray();
    }

    cJSON *cliente_json = cJSON_CreateObject();
    cJSON_AddStringToObject(cliente_json, "", "\n");  // Titulo: valor
    cJSON_AddStringToObject(cliente_json, "name", client.name);
    cJSON_AddStringToObject(cliente_json, "apellido", client.apellido);
    cJSON_AddStringToObject(cliente_json, "num_account", client.num_account);
    cJSON_AddStringToObject(cliente_json, "money", client.money);
    cJSON_AddStringToObject(cliente_json, "type_investment", client.type_investment);
    cJSON_AddStringToObject(cliente_json, "password", client.password);

    cJSON_AddItemToArray(clientes_json, cliente_json);

    char *json_str = cJSON_Print(clientes_json);
    archivo = fopen(nombreArchivo, "w");
    fprintf(archivo, "%s", json_str);
    fclose(archivo);

    free(json_str);
    cJSON_Delete(clientes_json);
}

/* FUNCIÓN ASIGNACIÓN BUFFER A VARIABLE */
void asignarBufferAVariable(char *buffer, char **variable) {
  if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 1;
    buffer[strcspn(buffer, "\n")] = 0;
    *variable = (char *)malloc(strlen(buffer) + 1);
    if (*variable == NULL) {
        printf("Error al asignar memoria para la variable.\n");
        return;
    }
    strcpy(*variable, buffer);
}

/* FUNCIÓN REGISTRAR CLIENTE */
int registrar() {
    struct Client client;
    char buffer[1024];

    printf("Ingrese el nombre del cliente: ");
    asignarBufferAVariable(buffer, &client.name);

    printf("Ingrese el apellido del cliente: ");
    asignarBufferAVariable(buffer, &client.apellido);

    printf("Ingrese el numero de cuenta del cliente: ");
    asignarBufferAVariable(buffer, &client.num_account);

    printf("Ingrese el contraseña del cliente: ");
    asignarBufferAVariable(buffer, &client.password);

    printf("Ingrese el dinero del cliente: ");
    asignarBufferAVariable(buffer, &client.money);

    printf("Ingrese el tipo de inversion del cliente: ");
    asignarBufferAVariable(buffer, &client.type_investment);

    printf("\n***** DATOS DEL CLIENTE *****\n");
    printf("Nombre: %s\n", client.name);
    printf("Cuenta: %s\n", client.num_account);
    printf("Dinero: %s\n", client.money);
    printf("Inversión: %s\n", client.type_investment);
    printf("****************************\n");

    guardarClienteComoJSON(client, "../data/clientes.json");
    printf("Datos guardados exitosamente en JSON.\n");

    free(client.name);
    free(client.num_account);
    free(client.money);
    free(client.type_investment);
    free(client.password);

    return 0;
}


