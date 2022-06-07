#include "hash.h"
#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#define CAPACIDAD_INICIAL 101 // 276 excelente, 292 muy bueno, 301 bueno
#define MULTIPLICADOR_REDIMENSION 3 //
#define FACTOR_DE_CARGA 0.7 // con 40k elementos: 0.5 lento, 0.6 maso menos, 0.7 rapido

#define NO_ENCONTRADO -1

typedef enum {VACIO, BORRADO, OCUPADO} estado_t;

typedef struct campo {
    estado_t estado;
    char* clave;
    void* dato;
} campo_t;

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

// Funcion de Hashing fnv
// http://www.isthe.com/chongo/tech/comp/fnv/
unsigned int fnv_hashing(const char* str, size_t length);

// Post: reserva memoria para un vector de campo_t del tamanio
// de la capacidad
campo_t* crear_tabla(size_t capacidad);

// Pre: hash creado y la tabla creada (arreglo de campo_t)
// Post: se inicializan todos los valores del campo en
// estado VACIO clave NULL y dato NULL
void asignar_tabla(hash_t* hash);

// Se usa la funcion de hash con la clave y luego en funcion
// de la capacidad del hash se devuelve la posicion
// Pre: hash creado
// Post: Devuelve la posicion de la clave dentro del hash
size_t clave_obtener_posicion(const hash_t* hash, const char *clave);

// Pre: hash creado, la clave se encuentra dentro del hash
// Post: Devuelve la posicion de la clave dentro del hash
// si no se encuentra devuelve NO_ENCONTRADO  (-1)
int busqueda_tabla(const hash_t* hash, const char* clave);

// Pre: hash creado
// Post: el hash incrementa su capacidad, la cantidad de borrados
// se vuelve 0 y se guardan los elementos segun la nueva capacidad de la tabla
bool tabla_redimensionar(hash_t* hash);


// Completa el campo en la posicion pos con el par (clave, dato)
// si es la misma clave entonces se aplica la funcion de destruccion correspondiente
// y luego se guarda el par (clave, dato)
// Pre: hash creado, pos < hash->capacidad
// Post: se guarda el par (clave, dato) en la posicion pos dentro del hash
void completar_campo(hash_t* hash, char* clave, void* dato, size_t pos, bool misma_clave);


// Se guarda el par(clave = copia, dato) en la posicion pos
// si no se puede guardar en la posicion pos, se busca , en este caso
// linealmente, aquella posicion donde si se puede guardar
// Pre: hash creado, pos < hash->capacidad
// Post: se guarda el par (copia, dato) en una posicion vacia del hash
bool guardar_par(hash_t* hash, char* copia, void* dato, size_t pos);

// Pre: hash creado
// Post: devuelve true si se necesita redimensionar
// false en otro caso
bool necesita_redimension(hash_t* hash);


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

int busqueda_tabla(const hash_t* hash, const char* clave) {
    int pos = (int)clave_obtener_posicion(hash, clave);
    while (pos < hash->capacidad) {
        if (hash->tabla[pos].estado == OCUPADO && strcmp(clave, hash->tabla[pos].clave) == 0) {
            return pos;
        }
        pos++; //lineal
    }

    return NO_ENCONTRADO;
}

bool tabla_redimensionar(hash_t* hash) {
    // Creo mi nueva tabla con mi nueva capacidad
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
    // Asigno los valores de mi nueva tabla que tiene el hash
    asignar_tabla(hash);

    // Guardo los elementos de la tabla anterior en el hash con mi nueva tabla
    for (size_t i = 0; i < capacidad_anterior; i++) {
        if (tabla_anterior[i].estado == OCUPADO) {
            hash_guardar(hash, tabla_anterior[i].clave, tabla_anterior[i].dato);
            free(tabla_anterior[i].clave);
        }
    }

    // Libero la memoria de mi tabla anterior
    free(tabla_anterior);

    return true;
}

void completar_campo(hash_t* hash, char* clave, void* dato, size_t pos, bool misma_clave) {
    if (!misma_clave) {
        hash->tabla[pos].estado = OCUPADO;
        hash->tabla[pos].clave = clave;
        hash->tabla[pos].dato = dato;
        hash->cantidad++;
    } else { // mismo elemento
        // Uso la funcion de destruccion indicada en los elementos (si existe) y
        // si no libero la clave en la posicion y guardo la generada (ambas son copias)
        if(hash->f_destruccion != NULL) {
            hash->f_destruccion(hash->tabla[pos].dato);
        }
        free(hash->tabla[pos].clave);
        hash->tabla[pos].clave = clave;
        hash->tabla[pos].dato = dato;
    }
}

bool guardar_par(hash_t* hash, char* copia, void* dato, size_t pos) {

    // Recorremos la estructura hasta hallar un espacio vacio u el mismo elemento
    while(pos < hash->capacidad) {
        if (hash->tabla[pos].estado == VACIO) {
            completar_campo(hash, copia, dato, pos, false);
            return true;
        }
        else if(hash->tabla[pos].estado == OCUPADO && strcmp(copia, hash->tabla[pos].clave) == 0) {
            completar_campo(hash, copia, dato, pos, true);
            return true;
        }
        pos++;
    }

    return false;
}

bool necesita_redimension(hash_t* hash) {
    bool alpha = (( (float)(hash->cantidad + hash->borrados) / (float)hash->capacidad) >= (float) FACTOR_DE_CARGA);
    return alpha;
}


/* *****************************************************************
 *                    PRIMITIVAS DEL HASH
 * *****************************************************************/


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


bool hash_guardar(hash_t *hash, const char *clave, void *dato) { 
    if (necesita_redimension(hash)) {
        if (!tabla_redimensionar(hash)) {
            return false;
        }
    }
    // Tomamos como caso general reservar memoria para la copia de la clave
    char* copia = strdup(clave);

    // Obtenemos la posicion (segun nuestra funcion hash)
    size_t pos = clave_obtener_posicion(hash, copia);

    // Si no se puede guardar (por colision o etc) redimensiono y guardo
    while(guardar_par(hash, copia, dato, pos) == false) {
        tabla_redimensionar(hash);
        pos = clave_obtener_posicion(hash, copia);
    }
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
    if(!hash_pertenece(hash, clave)) {
        return NULL;
    }
    // Si la clave pertenece, entonces busco su posicion y el dato
    int posicion_encontrada = busqueda_tabla(hash, clave);
    void* dato = hash->tabla[posicion_encontrada].dato;

    // Libero la memoria asociada a la clave que se guardo
    free(hash->tabla[posicion_encontrada].clave);

    // Cambio el estado a borrado
    hash->tabla[posicion_encontrada].estado = BORRADO;
    hash->cantidad--;
    hash->borrados++;

    return dato;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
    if(!hash_pertenece(hash, clave)) {
        return NULL;
    }

    int posicion_encontrada = busqueda_tabla(hash, clave);
    return hash->tabla[posicion_encontrada].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
    int posicion = busqueda_tabla(hash, clave);
    if(posicion == NO_ENCONTRADO) {
        return false;
    }
    return true;
}

size_t hash_cantidad(const hash_t *hash) {
    return hash->cantidad;
}

void hash_destruir(hash_t *hash) {
    size_t pos =0;

    while (pos < hash->capacidad) {
        // Si la posicion esta ocupada, libero la memoria
        // usando free o la funcion de destruccion indicada
        if (hash->tabla[pos].estado == OCUPADO) {
            if (hash->f_destruccion != NULL) {
                hash->f_destruccion(hash->tabla[pos].dato);
            }
            free(hash->tabla[pos].clave);
        }
        pos++;
    }
    free(hash->tabla);
    free(hash);
}
/* ******************************************************************
 *                  FUNCIONES AUXILIARES DEL ITERADOR
 * *****************************************************************/

// Pre: iterador creado
// Post: posiciona el iterador en la siguiente posicion
// con el estado OCUPADO
void iter_buscar_ocupado(hash_iter_t* iter) {
    while (iter->pos < iter->hash->capacidad) {
        if (iter->hash->tabla[iter->pos].estado == OCUPADO) {
            return;
        } else {
            iter->pos++;
        }
    }
}



/* *****************************************************************
 *               PRIMITIVA DEL ITERADOR DEL HASH
 * *****************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash) {
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if (!iter) {
        return NULL;
    }
    iter->hash = hash;
    iter->pos = 0;

    // Posiciono el iterador en el primer elemento ocupado
    // (si no hay lugar ocupado se posiciona al final)
    iter_buscar_ocupado(iter);

    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
    if (hash_iter_al_final(iter)) {
        return false;
    }

    iter->pos++;
    iter_buscar_ocupado(iter);
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {

    if (hash_iter_al_final(iter)){
        return NULL;
    }
    size_t pos = iter->pos;
    return iter->hash->tabla[pos].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
    return (iter->pos >= iter->hash->capacidad);
}

void hash_iter_destruir(hash_iter_t *iter) {
    free(iter);
}


//Ejs parcial
/*Sabiendo que se tiene un hash en que los valores son cadenas, implementar la función hash_t*
hash_invertir(const hash_t*); que devuelva un nuevo hash cuyas claves sean los valores del original
y sus valores asociados sean listas con las claves que tenían dichos valores en el primero. Indicar la
complejidad de la función*/


hash_t* hash_invertir(const hash_t* hash) {
    
    hash_t* res = hash_crear(hash->f_destruccion);

    hash_iter_t* iter = hash_iter_crear(hash);
    
    while (!hash_iter_al_final(iter)){
        const char* clave = hash_iter_ver_actual(iter);
        void* dato = hash_obtener(hash, clave);
        void* clave_guardar = strdup(clave);
        hash_guardar(res, (const char*) dato, clave_guardar);
        hash_iter_avanzar(iter);
    }
    hash_iter_destruir(iter);
    return res;
}