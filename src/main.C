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
int asignarBufferAVariable(char *buffer, size_t size, char **variable);
int calcularIntereses();
int verificarCliente(struct Client *cliente_encontrado);
int gestiontransacciones();
void actualizarClienteEnJSON(struct Client cliente_actualizado, const char *nombreArchivo);

/* CLIENTE */
struct Client datoscliente() {
    struct Client client;
    char buffer[1024];

    printf("Ingrese el nombre del cliente: ");
    asignarBufferAVariable(buffer, sizeof(buffer), &client.name);

    printf("Ingrese el apellido del cliente: ");
    asignarBufferAVariable(buffer, sizeof(buffer), &client.apellido);

    printf("Ingrese el numero de cuenta del cliente: ");
    asignarBufferAVariable(buffer, sizeof(buffer), &client.num_account);

    printf("Ingrese la contrasena del cliente: ");
    asignarBufferAVariable(buffer, sizeof(buffer), &client.password);

    printf("Ingrese el dinero del cliente: ");
    asignarBufferAVariable(buffer, sizeof(buffer), &client.money);

    printf("Ingrese el tipo de inversion del cliente: ");
    asignarBufferAVariable(buffer, sizeof(buffer), &client.type_investment);

    return client;
}

/* CICLO PRINCIPAL DE PROGRAMA */
int main() {
    while (1) {
        printf("Bienvenido al sistema de registro de clientes\n");
        printf("1. Registrar cliente\n");
        printf("2. Calculo de intereses y rendimientos\n");
        printf("3. Gestion de transacciones\n");
        printf("4. Salir\n");
        printf("Seleccione una opcion: ");

        int opcion;
        scanf("%d", &opcion);
        while (getchar() != '\n');

        switch (opcion) {
            case 1:
                registrar();
                break;
            case 2:
                calcularIntereses();
                break;
            case 3:
                gestiontransacciones();
                break;
            case 4:
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

/* ASIGNACION DE INFORMACION DE BUFFER TEMPORAL A VARIABLE */
int asignarBufferAVariable(char *buffer, size_t size, char **variable) {
    if (fgets(buffer, size, stdin) == NULL) return 0;
    buffer[strcspn(buffer, "\n")] = 0;
    *variable = (char *)malloc(strlen(buffer) + 1);
    if (*variable == NULL) {
        printf("Error al asignar memoria para la variable.\n");
        return 0;
    }
    strcpy(*variable, buffer);
    return 1;
}

/* VERIFICACION DE CLIENTE */
int verificarCliente(struct Client *cliente_encontrado) {
    char buffer[1024];
    char *nombre, *apellido, *cuenta, *password;

    printf("Ingrese el nombre del cliente: ");
    asignarBufferAVariable(buffer, sizeof(buffer), &nombre);

    printf("Ingrese el apellido del cliente: ");
    asignarBufferAVariable(buffer, sizeof(buffer), &apellido);

    printf("Ingrese el numero de cuenta: ");
    asignarBufferAVariable(buffer, sizeof(buffer), &cuenta);

    printf("Ingrese la contrasena: ");
    asignarBufferAVariable(buffer, sizeof(buffer), &password);

    FILE *archivo = fopen("../data/clientes.json", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo JSON.\n");
        return 0;
    }

    fseek(archivo, 0, SEEK_END);
    long length = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);

    char *contenido = malloc(length + 1);
    fread(contenido, 1, length, archivo);
    contenido[length] = '\0';
    fclose(archivo);

    cJSON *json = cJSON_Parse(contenido);
    free(contenido);
    if (!json) return 0;

    int num_clientes = cJSON_GetArraySize(json);
    int encontrado = 0;

    for (int i = 0; i < num_clientes; i++) {
        cJSON *cliente = cJSON_GetArrayItem(json, i);
        cJSON *jname = cJSON_GetObjectItem(cliente, "name");
        cJSON *japellido = cJSON_GetObjectItem(cliente, "apellido");
        cJSON *jcuenta = cJSON_GetObjectItem(cliente, "num_account");
        cJSON *jpass = cJSON_GetObjectItem(cliente, "password");

        if (strcmp(nombre, jname->valuestring) == 0 &&
            strcmp(apellido, japellido->valuestring) == 0 &&
            strcmp(cuenta, jcuenta->valuestring) == 0 &&
            strcmp(password, jpass->valuestring) == 0) {

            cliente_encontrado->name = strdup(jname->valuestring);
            cliente_encontrado->apellido = strdup(japellido->valuestring);
            cliente_encontrado->num_account = strdup(jcuenta->valuestring);
            cliente_encontrado->password = strdup(jpass->valuestring);
            cliente_encontrado->money = strdup(cJSON_GetObjectItem(cliente, "money")->valuestring);
            cliente_encontrado->type_investment = strdup(cJSON_GetObjectItem(cliente, "type_investment")->valuestring);
            encontrado = 1;
            break;
        }
    }

    cJSON_Delete(json);
    free(nombre);
    free(apellido);
    free(cuenta);
    free(password);

    return encontrado;
}

/* CALCULO DE INTERESES */
int calcularIntereses() {
    struct Client cliente;
    if (!verificarCliente(&cliente)) {
        printf("Cliente no encontrado o contrasena incorrecta.\n\n");
        return 1;
    }

    float monto = atof(cliente.money);
    float interes;
    int tipo = 0;

    if (strcmp(cliente.type_investment, "Renta Fija") == 0) tipo = 1;
    else if (strcmp(cliente.type_investment, "Renta Variable") == 0) tipo = 2;
    else if (strcmp(cliente.type_investment, "ETFs") == 0) tipo = 3;

    switch (tipo) {
        case 1: interes = monto * 0.05f; break;
        case 2: interes = monto * 0.07f; break;
        case 3: interes = monto * 0.10f; break;
        default: interes = monto * 0.03f; break;
    }

    printf("Interes generado: %.2f\n", interes);
    printf("Total estimado: %.2f\n\n", monto + interes);

    free(cliente.name);
    free(cliente.apellido);
    free(cliente.num_account);
    free(cliente.money);
    free(cliente.type_investment);
    free(cliente.password);

    return 0;
}

/* REGISTRO DE CLIENTE */
int registrar() {
    struct Client client = datoscliente();

    FILE *archivo = fopen("../data/clientes.json", "r");
    if (archivo != NULL) {
        fseek(archivo, 0, SEEK_END);
        long length = ftell(archivo);
        fseek(archivo, 0, SEEK_SET);

        char *contenido = malloc(length + 1);
        fread(contenido, 1, length, archivo);
        contenido[length] = '\0';
        fclose(archivo);

        cJSON *json = cJSON_Parse(contenido);
        free(contenido);

        if (json) {
            int num_clientes = cJSON_GetArraySize(json);
            for (int i = 0; i < num_clientes; i++) {
                cJSON *cliente = cJSON_GetArrayItem(json, i);
                cJSON *jname = cJSON_GetObjectItem(cliente, "name");
                cJSON *japellido = cJSON_GetObjectItem(cliente, "apellido");
                cJSON *jcuenta = cJSON_GetObjectItem(cliente, "num_account");

                if (
                    strcmp(client.name, jname->valuestring) == 0 &&
                    strcmp(client.apellido, japellido->valuestring) == 0 &&
                    strcmp(client.num_account, jcuenta->valuestring) == 0
                ) {
                    printf("Ya existe un cliente con ese nombre, apellido y numero de cuenta.\n\n");
                    cJSON_Delete(json);

                    free(client.name);
                    free(client.apellido);
                    free(client.num_account);
                    free(client.money);
                    free(client.type_investment);
                    free(client.password);
                    return 1;
                }
            }
            cJSON_Delete(json);
        }
    }

    guardarClienteComoJSON(client, "../data/clientes.json");
    printf("Cliente registrado exitosamente.\n\n");

    free(client.name);
    free(client.apellido);
    free(client.num_account);
    free(client.money);
    free(client.type_investment);
    free(client.password);

    return 0;
}

/* GESTION DE TRANSACCIONES */
int gestiontransacciones() {
    char buffer[1024];
    struct Client cliente_encontrado;
    if (!verificarCliente(&cliente_encontrado)) {
        printf("Cliente no encontrado o contrasena incorrecta.\n\n");
        return 1;
    }

    while (1) {
        printf("\n1. Depositar dinero\n2. Retirar dinero\n3. Transferir dinero\n4. Volver\nSeleccione una opcion: ");
        fgets(buffer, sizeof(buffer), stdin);
        int opcion = atoi(buffer);

        switch (opcion) {

            // Deposito de dinero
            case 1: {
                printf("Cantidad a depositar: ");
                fgets(buffer, sizeof(buffer), stdin);
                float deposito = atof(buffer);
                float saldo_actual = atof(cliente_encontrado.money);
                saldo_actual += deposito;
                sprintf(cliente_encontrado.money, "%.2f", saldo_actual);
                actualizarClienteEnJSON(cliente_encontrado, "../data/clientes.json");
                printf("Has depositado: %.2f\n", deposito);
                printf("Saldo actual: %.2f\n", saldo_actual);
                break;
            }
            // Retiro de dinero
            case 2: {
                printf("Cantidad a retirar: ");
                fgets(buffer, sizeof(buffer), stdin);
                float retiro = atof(buffer);
                float saldo = atof(cliente_encontrado.money);
                if (retiro > saldo) {
                    printf("Fondos insuficientes. Saldo actual: %.2f\n", saldo);
                } else {
                    saldo -= retiro;
                    sprintf(cliente_encontrado.money, "%.2f", saldo);
                    actualizarClienteEnJSON(cliente_encontrado, "../data/clientes.json");

                    printf("\nACTUALIZACIÓN DE DATOS: \n");
                    printf("Has retirado: %.2f\n", retiro);
                    printf("Saldo actual: %.2f\n", saldo);
                    printf("*****************************\n");
                }
                break;
            }
            // Transferencia de dinero
            case 3:
                printf("Transferir dinero\n");
                printf("Ingrese el numero de cuenta del destinatario: ");
                char cuenta_destino[128];
                fgets(cuenta_destino, sizeof(cuenta_destino), stdin);
                cuenta_destino[strcspn(cuenta_destino, "\n")] = 0;

                // Buscar cliente destino
                FILE *archivo = fopen("../data/clientes.json", "r");
                if (!archivo) {
                    printf("No se pudo abrir el archivo de clientes.\n");
                    break;
                }

                fseek(archivo, 0, SEEK_END);
                long length = ftell(archivo);
                fseek(archivo, 0, SEEK_SET);
                char *contenido = malloc(length + 1);
                fread(contenido, 1, length, archivo);
                contenido[length] = '\0';
                fclose(archivo);

                cJSON *json = cJSON_Parse(contenido);
                free(contenido);
                if (!json) {
                    printf("Error al procesar el archivo JSON.\n");
                    break;
                }

                struct Client cliente_destino = {0};
                int encontrado = 0;
                int num_clientes = cJSON_GetArraySize(json);
                for (int i = 0; i < num_clientes; i++) {
                    cJSON *cliente = cJSON_GetArrayItem(json, i);
                    cJSON *jcuenta = cJSON_GetObjectItem(cliente, "num_account");

                    if (strcmp(cuenta_destino, jcuenta->valuestring) == 0 &&
                        strcmp(cliente_encontrado.num_account, cuenta_destino) != 0) {

                        cliente_destino.name = strdup(cJSON_GetObjectItem(cliente, "name")->valuestring);
                        cliente_destino.apellido = strdup(cJSON_GetObjectItem(cliente, "apellido")->valuestring);
                        cliente_destino.num_account = strdup(jcuenta->valuestring);
                        cliente_destino.money = strdup(cJSON_GetObjectItem(cliente, "money")->valuestring);
                        encontrado = 1;
                        break;
                    }
                }

                if (!encontrado) {
                    printf("Cuenta destino no encontrada o es igual a tu cuenta.\n");
                    cJSON_Delete(json);
                    break;
                }

                printf("Ingrese el monto a transferir: ");
                fgets(buffer, sizeof(buffer), stdin);
                float monto = atof(buffer);

                float saldo_origen = atof(cliente_encontrado.money);
                float saldo_destino = atof(cliente_destino.money);

                if (monto > saldo_origen) {
                    printf("Fondos insuficientes para realizar la transferencia.\n");
                    cJSON_Delete(json);
                    free(cliente_destino.name);
                    free(cliente_destino.apellido);
                    free(cliente_destino.num_account);
                    free(cliente_destino.money);
                    break;
                }

                printf("\nResumen de la transferencia:\n");
                printf("De: %s %s (Cuenta: %s)\n", cliente_encontrado.name, cliente_encontrado.apellido, cliente_encontrado.num_account);
                printf("A: %s %s (Cuenta: %s)\n", cliente_destino.name, cliente_destino.apellido, cliente_destino.num_account);
                printf("Monto: %.2f\n", monto);
                printf("Confirma la transaccion escribiendo 'SI': ");

                char confirmacion[10];
                fgets(confirmacion, sizeof(confirmacion), stdin);
                confirmacion[strcspn(confirmacion, "\n")] = 0;

                if (strcmp(confirmacion, "SI") != 0) {
                    printf("Transaccion cancelada por el usuario.\n");
                    cJSON_Delete(json);
                    free(cliente_destino.name);
                    free(cliente_destino.apellido);
                    free(cliente_destino.num_account);
                    free(cliente_destino.money);
                    break;
                }

                // Ejecutar transferencia
                saldo_origen -= monto;
                saldo_destino += monto;
                sprintf(cliente_encontrado.money, "%.2f", saldo_origen);
                sprintf(cliente_destino.money, "%.2f", saldo_destino);

                // Actualizar JSON
                for (int i = 0; i < num_clientes; i++) {
                    cJSON *cliente = cJSON_GetArrayItem(json, i);
                    cJSON *jcuenta = cJSON_GetObjectItem(cliente, "num_account");

                    if (strcmp(jcuenta->valuestring, cliente_encontrado.num_account) == 0) {
                        cJSON_ReplaceItemInObject(cliente, "money", cJSON_CreateString(cliente_encontrado.money));
                    }

                    if (strcmp(jcuenta->valuestring, cliente_destino.num_account) == 0) {
                        cJSON_ReplaceItemInObject(cliente, "money", cJSON_CreateString(cliente_destino.money));
                    }
                }

                // Guardar archivo
                char *json_str = cJSON_Print(json);
                archivo = fopen("../data/clientes.json", "w");
                if (archivo) {
                    fprintf(archivo, "%s", json_str);
                    fclose(archivo);
                }
                free(json_str);
                cJSON_Delete(json);

                printf("Transferencia realizada con exito.\n");
                printf("Nuevo saldo de tu cuenta: %.2f\n", saldo_origen);
                
                // Liberar memoria
                free(cliente_destino.name);
                free(cliente_destino.apellido);
                free(cliente_destino.num_account);
                free(cliente_destino.money);

                break;
            case 4:
                return 0;
            default:
                printf("Opcion no valida.\n");
        }
    }
}

/* ACTUALIZAR CLIENTE EN JSON */
void actualizarClienteEnJSON(struct Client cliente_actualizado, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (!archivo) return;

    fseek(archivo, 0, SEEK_END);
    long length = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);

    char *contenido = malloc(length + 1);
    fread(contenido, 1, length, archivo);
    contenido[length] = '\0';
    fclose(archivo);

    cJSON *clientes_json = cJSON_Parse(contenido);
    free(contenido);
    if (!clientes_json) return;

    int num_clientes = cJSON_GetArraySize(clientes_json);
    for (int i = 0; i < num_clientes; i++) {
        cJSON *cliente = cJSON_GetArrayItem(clientes_json, i);
        cJSON *jcuenta = cJSON_GetObjectItem(cliente, "num_account");

        if (strcmp(cliente_actualizado.num_account, jcuenta->valuestring) == 0) {
            cJSON_ReplaceItemInObject(cliente, "money", cJSON_CreateString(cliente_actualizado.money));
            break;
        }
    }

    char *json_str = cJSON_Print(clientes_json);
    archivo = fopen(nombreArchivo, "w");
    fprintf(archivo, "%s", json_str);
    fclose(archivo);

    free(json_str);
    cJSON_Delete(clientes_json);
}