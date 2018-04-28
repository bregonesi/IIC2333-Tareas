
# Readme T2

## Integrantes  
| Nombre | Nº Alumno |
|--|--|
| Benjamín Regonesi | 14203847 |
| Rodrigo Orpis | 15209601 |

<br/><br/>
En el diseño de doer, la principal decisión que tomamos fue de lanzar los n procesos a ejecutar y después de haber lanzado todos se ve cuales han terminado. En este caso puede darse el caso que alguno termine un poco antes de terminar de lanzar todos los procesos pero se hizo así por simplicidad.
Para el programa lo que hicimos fue crear una cola de procesos a ejecutar, llamada "tareas" y otra cola para los procesos finalizados, llamada "tareas_finalizadas". Entonces lo que se hace es ir haciendo Dequeue a "tareas" y ejecutarlas, una vez que finalizan se van haciendo Enqueue a "tareas_finalizadas", y si fallan en su ejecución se vuelven a meter a "tareas", con un máximo de 2 ejecuciones.
<br/>
Para simulator, el programa lo primero que hace es ver la cantidad de bits por nivel a usar mediante una optimización en el tamaño necesario para encontrar la pagina, esta información se guarda en "info". Después, se crea la tabla de paginas completa usando "info" para saber cuantas hay que crear por nivel, también se crea la TLB (arreglo de 64 posiciones) y junto con esta se crean dos arreglos auxiliares, uno en donde se guardan los tiempos que lleva cada pagina en la TLB (para poder hacer LRU), y el otro para guardar la asociación de indice en la TLB - frame en memoria. También, se crea la RAM (arreglo de 256 posiciones) y junto con esta se crean dos arreglos auxiliares, uno para guardar las asociaciones de indice-pagina, y la otra para guardar los tiempos de cada frame en la RAM (para hacer LRU).
Luego el programa entra al main y sigue la misma lógica que se describe en el enunciado, dentro de este la mayoría de las lineas tiene su comentario de explicación hasta llegar a las estadísticas pedidas.

Las dos partes de la tarea cumplen con todo lo pedido.
