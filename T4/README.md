

# Readme T4

## Integrantes  
| Nombre | Nº Alumno |
|--|--|
| Benjamín Regonesi | 14203847 |
| Rodrigo Orpis | 15209601 |


# Codificaciones y decodificaciones
Se utilizaron funciones para codificar y decodificar el protocolo mencionado en la tarea.

 - `char* codificar(int mensaje_id, char* mensaje)`: Transforma el string `mensaje` al formato de envío. Es decir, con el largo del mensaje calculo el `payload size` y transformo mensaje a binario. Ademas agrego el `mensaje_id` en binario.
 - `char* codificar_cartas(int mensaje_id, int** cartas, int cantidad_cartas)`: Transformo el arreglo `cartas` al formato de envio agregandole el `mensaje_id` en binario.
 - `char* codificar_ints(int mensaje_id, int* valores, int cantidad)`: Parecido a `codificar` pero aqui codifico un arreglo de ints (`valores`) agregandole el `mensaje_id` en binario.
 - `char** decodificar(char* codificado)`: Decodifica el mensaje recibido. Retorna tres strings donde el primero es el id del mensaje (un string donde contiene un decimal que indica el id). El segundo indica el payload size de la misma manera. Y el tercero es el mensaje que resulta de transformar el binario a string.
 - `Decodificar_Mazo* decodificar_cartas(char* codificado)`: Transforma el mensaje recibido en un pequeño mazo de cartas.

# Cartas
Para esto se creo una estructura `Mazo` que contiene un arreglo de arreglos de `ints` y un contador de cuantas cartas quedan disponibles.
Para escoger cartas se creo la funcion `crear_mazo` que escoge un numero al azar entre 1 y la cantidad de cartas restantes.
