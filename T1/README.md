
# Readme T1

## Integrantes  
| Nombre | Nº Alumno |
|--|--|
| Benjamín Regonesi | 14203847 |
| Rodrigo Orpis | 15209601 |


## Diseño

Para esta tarea lo que hicimos fue crear estructuras de cola de prioridades y listas ligadas. En particular, para guardar todas las colas de procesos de MLFQ, lo que hicimos fue crear una cola de prioridades (llamemosla "gran cola"), en donde se guardan cada una de esas colas en orden de prioridad. Después, cada cola de procesos es una cola de prioridad en donde se guardan los procesos en orden para su ejecución. Esto es, que cuando un proceso nuevo llega, queda al final de la cola con más prioridad. Cabe mencionar que en nuestro programa la cola de mayor prioridad tiene `id = 0`, y la de menor prioridad `id = n`. Esto no influye en el funcionamiento del programa.

Para ejecutar un proceso, el scheduler lo que hace es buscar en la "gran cola" algún proceso que este en estado *RUNNING* y ejecuta todo su quantum siguiendo las reglas. Si en la gran cola no encuentra ningún proceso en estado *RUNNING*, entonces busca el primer proceso en estado *READY* partiendo por la cola de mayor prioridad. Una vez encontrado este proceso se cambia a estado *RUNNING*.

Para el funcionamiento del *Scheduler* lo que hicimos fue crear un clock del programa en donde, en cada clock se ve el estado actual de el proceso en estado RUNNING para disminuir su tiempo restante y ver las condiciones mencionadas en el enunciado.
Un caso borde no explicado en el enunciado es cuando se acaba el *burst* y *quantum*. En este caso para nosotros ocurre una interrupción (aumentando este contador). Esto se explica en el readme ya que el profesor dijo en la clase del Lunes 09 de Abril que explicaramos que hace nuestro programa en este caso borde.
Si el programa no se interrumpe, el `scheduler` ejecuta de forma indefinida hasta que no quedan procesos por llegar y ademas todos los procesos se encuentren en la cola *finished*.


## Análisis

Después de varias simulaciones con diferentes parámetros pudimos notar el patrón de que la versión `v3` es más justa, ya que además de evitar la inanición de procesos antiguos (por el uso de `Aging`), a los procesos grandes (CPU-bound) se les da más quantum promedio. En particular, la versión `v3` es más justa que la `v2` y esta a su vez es más justa que la `v1` pero no necesariamente con *Turnaround* menor.

Por ejemplo, para la siguiente lista de procesos:

> PROCESS0 5 3 4 3 5
PROCESS1 11 3 1 4 7
PROCESS2 14 2 14 2
PROCESS3 18 2 12 16
PROCESS4 22 2 1 2
PROCESS5 25 5 7 2 6 5 9
PROCESS6 2 3 17 6 5
PROCESS7 15 2 17 26
PROCESS8 27 6 3 5 11 8 4 7
PROCESS9 6 4 5 7 13 2
PROCESS10 12 6 4 2 3 7 5 12
PROCESS11 13 1 32
PROCESS12 7 7 3 2 5 4 7 1 2
PROCESS13 0 1 99

En una simulación con parámetros: `5 colas` `quantum = 5` `S = 50`
Se pueden ver los siguientes resultados:

**Para el PROCESS13:**

| Parámetro | v1 | v2 | v3 |
|:--|:--:|:--:|:--:|
| Turnos de CPU | 20 |	20 |	12 |
| Cantidad de bloqueos | 19 | 19 | 11 |
| Turnaround | 424 |	424 |	424 |
| Response time | 0 |	0 |	0 |
| Waiting time | 325 |	325 |	325 |

Aquí podemos ver que la versión `v3` le da más quantum promedio a los procesos más grandes, esto se puede ver porque el proceso terminó al mismo tiempo pero en menor cantidad de turnos en la CPU.

**Para el PROCESS6:**

| Parámetro | v1 | v2 | v3 |
|:--|:--:|:--:|:--:|
| Turnos de CPU | 8 |	8 |	8 |
| Cantidad de bloqueos | 5 | 5 | 5 |
| Turnaround | 326 |	285 |	295 |
| Response time | 3 |	3 |	3 |
| Waiting time | 298 |	257 |	267 |

Podemos ver que en un proceso estándar el `Aging` lo beneficia. Pero como la versión `v3` beneficia además a los procesos grandes entonces empeora con respecto a la versión `v2`.

**Y para el PROCESS10:**

| Parámetro | v1 | v2 | v3 |
|:--|:--:|:--:|:--:|
| Turnos de CPU | 12 |	12 |	12 |
| Cantidad de bloqueos | 6 | 6 | 6 |
| Turnaround | 343 |	350 |	365 |
| Response time | 11 |	11 |	11 |
| Waiting time | 310 |	317 |	332 |

En este caso podemos ver que la versión `v3` perjudica a los procesos con *bursts* pequeños. Esto es porque mantiene al proceso más tiempo en la misma cola enviándolo al final de esta misma, mientras que a los procesos con mayores *bursts* los envía a una cola inferior que hace que se incremente su *quantum* y después de un `Aging` volverán a subir, lo que hace que terminen antes que los con *bursts* pequeños.
