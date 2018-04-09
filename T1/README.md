# Readme T1

Autores: Benjamim Regonesi () y Rodrigo Orpis (15209601)


*Diseño:*  

Para esta tarea lo que hicimos fue crear estructuras de cola de prioridades y listas ligadas,
en particular, para guardar todas las colas de procesos de MLFQ lo que hicimos fue crear una cola de prioridades (llamemosla "gran cola") en donde se guardan cada una de esas colas en orden de prioridad. Después,cada cola de procesos es una cola de prioridad en donde se guardan los procesos en orden para su ejecución. Esto es, que cuando un proceso nuevo llega, queda al final de la cola con más prioridad.
Para ejecutar un proceso, el scheduler lo que hace es buscar en la "gran cola" la primera cola que tenga procesos en estado READY (size > 0), y una vez encontrada esa cola, selecciona el proceso con más prioridad que será el siguiente en ejecutar.

Para el funcionamiento del Scheduler lo que hicimos fue crear un clock del programa en donde en cada clock
se ve el estado actual de el proceso en estado RUNNING para disminuir su tiempo restante y ver las condiciones
para ver si ya completó su quantum o ya terminó el burst y enviarlo al final de la misma cola o al final de la
cola con menos prioridad que esa segun corresponda.

*Análisis:*
