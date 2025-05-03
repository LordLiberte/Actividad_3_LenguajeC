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
        while (getchar() != '\n'); // Elimina el problema de solapamiento de inputs

        switch (opcion) {
            case 1:
                registrar();
                break;
            case 2:
                calcularIntereses();
                break;  // En python esto no es así, como que break para que continue el programa y return para que acabe >:(
            case 3:
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

/* FUNCIÓN ASIGNACIÓN BUFFER A VARIABLE */
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


/* FUNCIÓN PARA INGRESAR DATOS DEL CLIENTE */
struct Client datoscliente() {
  
  struct Client client;
  char buffer[1024];

  printf("Ingrese el nombre del cliente: ");
  asignarBufferAVariable(buffer, sizeof(buffer), &client.name);

  printf("Ingrese el apellido del cliente: ");
  asignarBufferAVariable(buffer, sizeof(buffer), &client.apellido);

  printf("Ingrese el numero de cuenta del cliente: ");
  asignarBufferAVariable(buffer, sizeof(buffer), &client.num_account);

  printf("Ingrese el contraseña del cliente: ");
  asignarBufferAVariable(buffer, sizeof(buffer), &client.password);

  printf("Ingrese el dinero del cliente: ");
  asignarBufferAVariable(buffer, sizeof(buffer), &client.money);

  printf("Ingrese el tipo de inversion del cliente: ");
  asignarBufferAVariable(buffer, sizeof(buffer), &client.type_investment);
  return client;

}

/* FUNCIÓN REGISTRAR CLIENTE */
int registrar() {
  struct Client client = datoscliente();

  // Abrimos el archivo JSON para verificar duplicados
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

                  // Liberamos memoria
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

  // Si no existe, lo registramos
  printf("\n***** DATOS DEL CLIENTE *****\n");
  printf("Nombre Completo: %s %s\n", client.name, client.apellido);
  printf("Cuenta: %s\n", client.num_account);
  printf("Dinero: %s\n", client.money);
  printf("Inversión: %s\n", client.type_investment);
  printf("****************************\n");

  guardarClienteComoJSON(client, "../data/clientes.json");
  printf("Cliente registrado exitosamente.\n\n");

  // Liberamos memoria
  free(client.name);
  free(client.apellido);
  free(client.num_account);
  free(client.money);
  free(client.type_investment);
  free(client.password);

  return 0;
}



/* FUNCIÓN CALCULO DE INTERESES Y RENDIMIENTOS */
int calcularIntereses() {
  char buffer[1024];
  char *nombre, *apellido, *cuenta, *password;

  // Pedimos datos del usuario
  printf("Ingrese el nombre del cliente: ");
  asignarBufferAVariable(buffer, sizeof(buffer), &nombre);

  printf("Ingrese el apellido del cliente: ");
  asignarBufferAVariable(buffer, sizeof(buffer), &apellido);

  printf("Ingrese el numero de cuenta: ");
  asignarBufferAVariable(buffer, sizeof(buffer), &cuenta);

  printf("Ingrese la contraseña: ");
  asignarBufferAVariable(buffer, sizeof(buffer), &password);

  printf("Verificando datos...\n");

  // Leemos el JSON
  FILE *archivo = fopen("../data/clientes.json", "r");
  if (archivo == NULL) {
      printf("Error al abrir el archivo JSON.\n");
      return 1;
  }

  fseek(archivo, 0, SEEK_END);
  long length = ftell(archivo);
  fseek(archivo, 0, SEEK_SET);

  char *contenido = malloc(length + 1);
  fread(contenido, 1, length, archivo);
  contenido[length] = '\0';
  fclose(archivo);

  cJSON *json = cJSON_Parse(contenido);
  if (!json) {
      printf("Error al parsear el JSON.\n");
      free(contenido);
      return 1;
  }

  int num_clientes = cJSON_GetArraySize(json);
  int encontrado = 0;

  for (int i = 0; i < num_clientes; i++) {
      cJSON *cliente = cJSON_GetArrayItem(json, i);

      cJSON *jname = cJSON_GetObjectItem(cliente, "name");
      cJSON *japellido = cJSON_GetObjectItem(cliente, "apellido");
      cJSON *jcuenta = cJSON_GetObjectItem(cliente, "num_account");
      cJSON *jmoney = cJSON_GetObjectItem(cliente, "money");
      cJSON *jinv = cJSON_GetObjectItem(cliente, "type_investment");
      cJSON *jpass = cJSON_GetObjectItem(cliente, "password");

      // Verificamos coincidencias con autenticación completa
      if (strcmp(nombre, jname->valuestring) == 0 &&
          strcmp(apellido, japellido->valuestring) == 0 &&
          strcmp(cuenta, jcuenta->valuestring) == 0 &&
          strcmp(password, jpass->valuestring) == 0) {

          printf("\nSesion iniciada correctamente\n\n");
          printf("Cliente: %s %s\n", jname->valuestring, japellido->valuestring);
          printf("Cuenta: %s\n", jcuenta->valuestring);
          printf("Dinero: %s\n", jmoney->valuestring);
          printf("Tipo de inversión: %s\n\n", jinv->valuestring);

          float monto = atof(jmoney->valuestring);
          float interes;
          int tipo = 0;

          if (strcmp(jinv->valuestring, "Renta Fija") == 0) tipo = 1;
          else if (strcmp(jinv->valuestring, "Renta Variable") == 0) tipo = 2;
          else if (strcmp(jinv->valuestring, "ETFs") == 0) tipo = 3;
          else tipo = 0;

          switch (tipo) {
              case 1:
                  interes = monto * 0.05f;
                  break;  // Otra vez me da problemas el maldito break por poner return 0 >>>>>:((((((
              case 2:
                  interes = monto * 0.07f;
                  break;
              case 3:
                  interes = monto * 0.10f;
                  break;
              default:
                  interes = monto * 0.03f;
                  break;
                  // Voy a tener pesadillas con el break >:(
          }

          printf("Interes generado: %.2f\n", interes);
          printf("Total estimado: %.2f\n\n", monto + interes);
          encontrado = 1;
          break;
      }
  }

  if (!encontrado) {
      printf("Cliente no encontrado o contraseña incorrecta.\n\n");
  }

  // Limpieza
  cJSON_Delete(json);
  free(contenido);
  free(nombre);
  free(apellido);
  free(cuenta);
  free(password);

  return 0;
}

