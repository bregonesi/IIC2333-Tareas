
# Readme T3

## Integrantes  
| Nombre | Nº Alumno |
|--|--|
| Benjamín Regonesi | 14203847 |
| Rodrigo Orpis | 15209601 |

## Consideraciones
* Para el correcto funcionamiento de todo hay que tener solo un `czFILE` abierto por nombre al mismo tiempo. En caso de querer hacer `cz_cp` o `cz_rm` asegurarse de cerrarlo antes (y liberarlo).
* Para montar el sistema de archivo utilizar `void cz_mount(char* diskfileName)`
* Para liberar memoria utilizada por los `czFILE` utilizar `void cz_free(czFILE* file)`
