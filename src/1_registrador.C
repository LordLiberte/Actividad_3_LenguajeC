#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Definición de la estructura del cliente
struct Client {

    // Información del cliente
    char *name; // Nombre del cliente
    char *num_account; // Número de cuenta del cliente
    char *money; // Cantidad de dinero del cliente
    char *type_investment; // Tipo de inversión del cliente
    char *password;

  };

/*  FUNCIÓN GUARDAR DATOS JSON  */
void guardarClienteComoJSON(struct Client client, const char *nombreArchivo) {
  FILE *archivo = fopen(nombreArchivo, "a"); // Abrimos el archivo en modo append (añadir al final)
  if (!archivo) {
      perror("Error al abrir el archivo");
      return;
  }

  fprintf(archivo, "{\n");
  fprintf(archivo, "  \"name\": \"%s\",\n", client.name);
  fprintf(archivo, "  \"num_account\": \"%s\",\n", client.num_account);
  fprintf(archivo, "  \"money\": \"%s\",\n", client.money);
  fprintf(archivo, "  \"type_investment\": \"%s\"\n", client.type_investment);
  fprintf(archivo, "}\n");

  fclose(archivo);
}

/* FUNCIÓN ASIGNACIÓN BUFFER A VARIABLE */
void asignarBufferAVariable(char *buffer, char **variable) {
  *variable = (char *)malloc(strlen(buffer) + 1); // Reservamos memoria para la variable
  if (*variable == NULL) {
    printf("Error al asignar memoria para la variable.\n");
    return;
  }
  strcpy(*variable, buffer); // Copiamos el buffer a la variable
}

int main() {

  /*  CREACIÓN DEL CLIENTE Y BUFFER DE DETERMINACIÓN DE MEMORIA */
  struct Client client;
  char buffer[1024]; // Definimos un buffer para almacenar variables, será temporal y se libera automaticamente

  /*  ASIGNACIÓN DE NOMBRE DEL CLIENTE  */
  printf("Ingrese el nombre del cliente: ");

  if (fgets(buffer, sizeof(buffer), stdin) == NULL){ // Leemos el nombre del cliente y lo almacenamos en el buffer. Usamos fgets para evitar desbordamientos de buffer
    printf("Error al leer el nombre del cliente.\n");
    return 1;
  }

  buffer[strcspn(buffer, "\n")] = 0; // Una vez leido el nombre, eliminamos el salto de línea al final (cosas de pulsar enter)
  asignarBufferAVariable(buffer, &client.name); // Asignamos el buffer a la variable del cliente (nombre)

  /*  ASIGNACIÓN NÚMERO DE CUENTA DEL CLIENTE */
  printf("Ingrese el numero de cuenta del cliente: ");
  if (fgets(buffer, sizeof(buffer), stdin) == NULL){
    printf("Error al leer el numero de cuenta del cliente.\n");
    return 1;
  }
  buffer[strcspn(buffer, "\n")] = 0;
  asignarBufferAVariable(buffer, &client.num_account); // Asignamos el buffer a la variable del cliente (número de cuenta)

  /*  ASIGNACIÓN PASSWORD DEL CLIENTE */
  printf("Ingrese el contraseña del cliente: ");
  if (fgets(buffer, sizeof(buffer), stdin) == NULL){
    printf("Error al leer el password del cliente.\n");
    return 1;
  }
  buffer[strcspn(buffer, "\n")] = 0;
  asignarBufferAVariable(buffer, &client.password); // Asignamos el buffer a la variable del cliente (dinero)

  /*  ASIGNACIÓN DINERO DEL CLIENTE */
  printf("Ingrese el dinero del cliente: ");
  if (fgets(buffer, sizeof(buffer), stdin) == NULL){
    printf("Error al leer el dinero del cliente.\n");
    return 1;
  }
  buffer[strcspn(buffer, "\n")] = 0;
  asignarBufferAVariable(buffer, &client.money); // Asignamos el buffer a la variable del cliente (dinero)


  /*  ASIGNACIÓN TIPO INVERSIÓN DEL CLIENTE */
  printf("Ingrese el tipo de inversion del cliente: ");
  if (fgets(buffer, sizeof(buffer), stdin) == NULL){
    printf("Error al leer el tipo de inversion del cliente.\n");
    return 1;
  }
  buffer[strcspn(buffer, "\n")] = 0;
  asignarBufferAVariable(buffer, &client.type_investment); // Asignamos el buffer a la variable del cliente (tipo de inversión)


  /*  IMPRESIÓN EN PANTALLA DE RESULTADO  */
  printf("\n***** DATOS DEL CLIENTE *****\n"); // Mostramos los datos del cliente
  printf("Nombre del cliente: %s\n", client.name); // Mostramos el nombre del cliente
  printf("Numero de cuenta del cliente: %s\n", client.num_account); // Mostramos el número de cuenta del cliente
  printf("Dinero del cliente: %s\n", client.money); // Mostramos el dinero del cliente
  printf("Tipo de inversion del cliente: %s\n", client.type_investment); // Mostramos el tipo de inversión del cliente
  printf("****************************\n"); // Fin de la impresión de datos del cliente

  /*  GUARDAMOS INFORMACIÓN EN JSON */
  guardarClienteComoJSON(client, "../data/clientes.json"); // Guardamos los datos del cliente en un archivo JSON
  printf("Los datos del cliente se han guardado en el archivo clientes.json\n"); // Mensaje de confirmación de guardado


  /*  ZONA LIBERACIÓN MEMORIA USADA */
  free(client.name); // Liberamos la memoria del nombre del cliente
  free(client.num_account); // Liberamos la memoria del número de cuenta del cliente
  free(client.money); // Liberamos la memoria del dinero del cliente
  free(client.type_investment); // Liberamos la memoria del tipo de inversión del cliente

  return 0;
}

