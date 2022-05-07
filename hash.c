#include "hash.h"
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#define CAPACIDAD_INICIAL 301 //Num_primo
#define MULTIPLICADOR_REDIMENSION 3 //Ir probando
#define FACTOR_DE_CARGA 0.7 //Ir probando

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

struct hash_iter {
    const hash_t* hash;
    size_t pos;
};

/* ******************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

//Funcion de Hashing
unsigned int fnv_hashing(const char* str, size_t length) {
    unsigned int new_length = (unsigned int) length;
	const unsigned int fnv_prime = 0x811C9DC5;
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < new_length; str++, i++)
	{
		hash *= fnv_prime;
		hash ^= (unsigned int)(*str);
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


size_t clave_obtener_posicion(const hash_t* hash, const char *clave) {
    size_t resultado_hash = fnv_hashing(clave, strlen(clave));
    return resultado_hash % hash->capacidad;
}


size_t busqueda_tabla(const hash_t* hash, const char* clave) {
    
    size_t largo_tabla = hash->capacidad;
    size_t pos = clave_obtener_posicion(hash, clave);

    while (pos < hash->capacidad) {
        if (hash->tabla[pos].estado == OCUPADO && strcmp(clave, hash->tabla[pos].clave) == 0) {
            return pos;
        }
        pos++; //lineal
    }

    return 0;
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
    hash->borrados = 0;
    asignar_tabla(hash);

    for (size_t i = 0; i < capacidad_anterior; i++) {
        if (tabla_anterior[i].estado == OCUPADO) {
            hash_guardar(hash, tabla_anterior[i].clave, tabla_anterior[i].dato);
            free(tabla_anterior[i].clave);
        }
    }
    free(tabla_anterior);
    return true;
}

void completar_campo(hash_t* hash, char* clave, void* dato, size_t pos, bool misma_clave) {

    if (!misma_clave) {
        hash->tabla[pos].estado = OCUPADO;
        hash->tabla[pos].clave = clave;
        hash->tabla[pos].dato = dato;
        hash->cantidad++;
    } else {
        free(clave);
        hash->tabla[pos].dato = dato;
    }
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
    
    if (((((float)hash->cantidad + (float)hash->borrados) / (float)hash->capacidad) >= (float) FACTOR_DE_CARGA)) {
        if (!tabla_redimensionar(hash)) {
            return false;
        }
    }

    char* copia = strdup(clave);

    size_t pos = clave_obtener_posicion(hash, copia);

    while (true) {
        //Lugar desocupado
        if (hash->tabla[pos].estado == VACIO) {
            completar_campo(hash, copia, dato, pos, false); //Misma clave == false
            break;
        }
        //Lugar ocupado y misma clave
        if (hash->tabla[pos].estado == OCUPADO && strcmp(clave, hash->tabla[pos].clave) == 0) {
            if(hash->f_destruccion != NULL) {
                hash->f_destruccion(hash->tabla[pos].dato);
            }
            completar_campo(hash, copia, dato, pos, true); //Misma clave == true
            break;
        }

        pos++;
    }
    return true;
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

    void* dato = hash->tabla[posicion_encontrada].dato;

    // Libero la memoria asociada a la clave que se guardo
    free(hash->tabla[posicion_encontrada].clave);

    hash->tabla[posicion_encontrada].estado = BORRADO;
    hash->cantidad--;
    hash->borrados++;

    return dato;
}



void *hash_obtener(const hash_t *hash, const char *clave) {
    if(!hash_pertenece(hash, clave)) {
        return NULL;
    }

    size_t posicion_encontrada = busqueda_tabla(hash, clave);

    return hash->tabla[posicion_encontrada].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
    size_t posicion = clave_obtener_posicion(hash, clave);

    bool seguir = true, encontrado;

    while(posicion < hash->capacidad && seguir) {
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
            posicion++;
            encontrado = false;
        }
    }

    return encontrado;
}

/* Devuelve la cantidad de elementos del hash.
 * Pre: La estructura hash fue inicializada
 */
size_t hash_cantidad(const hash_t *hash) {
    return hash->cantidad;
}

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida
 */
void hash_destruir(hash_t *hash) {
    size_t pos =0;

    while (pos < hash->capacidad) {
        if (hash->tabla[pos].estado == OCUPADO) {
            if (hash->f_destruccion != NULL) {
                hash->f_destruccion(hash->tabla[pos].clave);
                hash->f_destruccion(hash->tabla[pos].dato);
            } else {
                free(hash->tabla[pos].clave);
            }
        }
        pos++;
    }
    free(hash->tabla);
    free(hash);
}


/* *****************************************************************
 *               PRIMITIVA DEL ITERADOR DEL HASH
 * *****************************************************************/

/* Iterador del hash */

// Crea iterador
hash_iter_t *hash_iter_crear(const hash_t *hash) {
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if (!iter) {
        return NULL;
    }
    iter->hash = hash;
    iter->pos = 0;

    bool seguir = true;
    while (iter->pos < iter->hash->capacidad && seguir) {
        if (iter->hash->tabla[iter->pos].estado == OCUPADO) {
            seguir = false;
        } else {
            iter->pos++;
        }
    }

    return iter;
}

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter) {
    if (hash_iter_al_final(iter)) {
        return false;
    }

    bool seguir = true;
    iter->pos++;
    while (iter->pos < iter->hash->capacidad && seguir) {
        if (iter->hash->tabla[iter->pos].estado == OCUPADO) {
            seguir = false;
        } else {
            iter->pos++;
        }
    }
    return true;
}

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter) {

    if (hash_iter_al_final(iter)){
        return NULL;
    }
    size_t pos = iter->pos;
    return iter->hash->tabla[pos].clave;
}

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter) {
    if (iter->pos < iter->hash->capacidad) {
        return false;
    }
    return true;
}

// Destruye iterador
void hash_iter_destruir(hash_iter_t *iter) {
    free(iter);
}