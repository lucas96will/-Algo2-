#include "hash.h"
#include <stdlib.h>
#include <string.h>

#define CAPACIDAD_INICIAL 101 //Num_primo
#define MULTIPLICADOR_REDIMENSION 2 //Ir probando
#define FACTOR_DE_CARGA 0.5 //Ir probando
#define MAX_REPOSICIONES 3

typedef enum {VACIO, BORRADO, OCUPADO} estado_t;

struct campo {
    estado_t estado;
    char* clave;
    void* dato;
};

struct hash {
    campo_t* tabla;
    size_t cantidad;
    size_t capacidad;
    size_t borrados;
    hash_destruir_dato_t f_destruccion;
};

struct hash_iter;

/* ******************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

//Funcion de Hashing
unsigned int fnv_hashing(char* str, unsigned int length) {
	const unsigned int fnv_prime = 0x811C9DC5;
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < length; str++, i++)
	{
		hash *= fnv_prime;
		hash ^= (*str);
	}

	return hash;
}

campo_t* crear_tabla(size_t capacidad) {
    campo_t* tabla = malloc(sizeof(campo_t) * capacidad);
    
    if (!tabla) {
        return NULL;
    }
    return tabla;
}

void asignar_tabla(hash_t* hash) {
    size_t i = 0;

    while (i < hash->capacidad) {
        hash->tabla[i].estado = VACIO;
        hash->tabla[i].clave = NULL;
        hash->tabla[i].dato = NULL;
        i++;
    }
}


size_t clave_obtener_posicion(const char *clave, size_t largo_tabla) {
    char copia[strlen(clave)];
    strcpy(copia, clave);
    size_t resultado_hash = fnv_hashing(copia, strlen(copia));
    return resultado_hash % largo_tabla;
}


size_t busqueda_tabla(const hash_t* hash, const char* clave) {
    
    size_t resultado_hash = fnv_hashing(clave, strlen(clave));
    size_t pos = resultado_hash % hash->capacidad;

    while (pos < hash->capacidad) {
        if (hash->tabla[pos].estado == OCUPADO && strcmp(clave, hash->tabla[pos].clave) == 0) {
            return pos;
        }

        pos = pos + 1; //lineal
    }

    return -1;
}

bool tabla_redimensionar(hash_t* hash) {
    size_t nueva_capacidad = hash->capacidad * MULTIPLICADOR_REDIMENSION;
    campo_t* nueva_tabla = crear_tabla(nueva_capacidad);
    if (!nueva_tabla) {
        return false;
    }
    
    campo_t* tabla_anterior = hash->tabla;
    size_t capacidad_anterior = hash->capacidad;

    hash->tabla = nueva_tabla;
    hash->capacidad = nueva_capacidad;
    hash->cantidad = 0;
    asignar_tabla(hash);

    for (size_t i = 0; i < capacidad_anterior; i++) {
        if (tabla_anterior[i].estado == OCUPADO) {
            hash_guardar(hash, tabla_anterior[i].clave, tabla_anterior[i].dato);
        }
    }

    free(tabla_anterior);
    return true;

}



/* *****************************************************************
 *                    PRIMITIVAS DEL HASH
 * *****************************************************************/


/* Crea el hash
 */
hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t* hash = malloc(sizeof(hash_t));
    
    if (!hash) {
        return NULL;
    }

    hash->tabla = crear_tabla(CAPACIDAD_INICIAL);
    if (!hash->tabla) {
        free (hash);
        return NULL;
    }

    hash->capacidad = CAPACIDAD_INICIAL;
    hash->cantidad = 0;
    hash->borrados = 0;
    hash->f_destruccion = destruir_dato;
    asignar_tabla(hash);

    return hash;
}

/* Guarda un elemento en el hash, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura hash fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool hash_guardar(hash_t *hash, const char *clave, void *dato) { //continuar
    
    if ((float) ((hash->cantidad + hash->borrados) / hash->capacidad) >= (float) FACTOR_DE_CARGA) {
        bool redimension = tabla_redimensionar(hash);
        if (!redimension) {
            return false;
        }

    }
    size_t resultado_hash = fnv_hashing(clave, strlen(clave));
    size_t pos = resultado_hash % hash->capacidad;
}

/* Borra un elemento del hash y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura hash fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *hash_borrar(hash_t *hash, const char *clave) {
    if(!hash_pertenece(hash, clave)) {
        return NULL;
    }
    size_t posicion_encontrada = busqueda_tabla(hash, clave);

    // Libero la memoria asociada a la clave que se guardo
    free(hash->tabla[posicion_encontrada].clave);

    hash->tabla[posicion_encontrada].estado = BORRADO;
    hash->cantidad--;
    hash->borrados++;

    return hash->tabla[posicion_encontrada].dato;
}



void *hash_obtener(const hash_t *hash, const char *clave) {
    if(!hash_pertenece(hash, clave)) {
        return NULL;
    }
    size_t posicion_encontrada = busqueda_tabla(hash, clave);
    if(posicion_encontrada == -1) return NULL; // CAMBIAR ESTO PORQUE SI PERTENECE LA DEBERIA ENCONTRAR EN LOS PRIMEROS 3 ESPACIOS
    return hash->tabla[posicion_encontrada].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
    size_t posicion = clave_obtener_posicion(clave, hash->capacidad);

    bool seguir = true, encontrado;
    size_t contador = 0;

    while(posicion < hash->capacidad && contador < MAX_REPOSICIONES && seguir) {
        char* clave_pos = hash->tabla[posicion].clave;
        estado_t estado_pos = hash->tabla[posicion].estado;

        if(estado_pos == VACIO){
            seguir = false;
            encontrado = false;
        }
        else if(estado_pos == OCUPADO && strcmp(clave_pos, clave) == 0) {
            seguir = false;
            encontrado = true;
        }
        else if((estado_pos == OCUPADO && strcmp(clave_pos, clave) != 0) || estado_pos == BORRADO) {
            contador++;
            posicion += contador;
            encontrado = false;
        }
    }

    return encontrado;
}

/* Devuelve la cantidad de elementos del hash.
 * Pre: La estructura hash fue inicializada
 */
size_t hash_cantidad(const hash_t *hash);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida
 */
void hash_destruir(hash_t *hash);


/* *****************************************************************
 *               PRIMITIVA DEL ITERADOR DEL HASH
 * *****************************************************************/

/* Iterador del hash */

// Crea iterador
hash_iter_t *hash_iter_crear(const hash_t *hash);

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter);

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter);

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter);

// Destruye iterador
void hash_iter_destruir(hash_iter_t *iter);