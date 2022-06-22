#ifndef _ALGO2__PUBLICACION_USER_H
#define _ALGO2__PUBLICACION_USER_H

#include <stdlib.h>

/* *****************************************************************
 *                    PRIMITIVAS PUBLICACION_USER
 * *****************************************************************/

/*
 * Pre: publicacion creada previamente
 * Post: La estructura publicacion_user fue creada
 */
publicacion_user_t* publicacion_user_crear(void* publicacion, size_t afinidad);

/*
 * Pre : Publicacion_user creado
 * Post: Obtiene la publicacion del publicacion_user
 */
publicacion_t* publicacion_user_obtener_publicacion(publicacion_user_t* publicacion_user);

/*
 * Pre : Publicacion_user creado
 * Post: Obtiene la afinidad de publicacion_user
 */
size_t publicacion_user_obtener_afinidad(publicacion_user_t* publicacion_user);

/*
 * Pre: La estructura publicacion_user fue creada
 * Post: La estructura publicacion_user fue destruida
 */
void publicacion_user_destruir(publicacion_user_t* publicacion_user);

#endif //_ALGO2__PUBLICACION_USER_H