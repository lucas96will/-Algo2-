#!/usr/bin/python3
from biblioteca_recomendify import * #Cambiarlo
import sys

# Comandos disponibles
CAMINO_MAS_CORTO = "camino"
CANCIONES_MAS_IMPORTANTES = "mas_importantes"
RECOMENDACION = "recomendacion"
CICLO = "ciclo"
RANGO = "rango"


def camino_mas_corto():
    pass


def canciones_mas_importantes():
    pass


def recomendacion():
    pass


def ciclo_canciones():
    pass


def todas_en_rango():
    pass


def hash_funciones():
    funciones = {
        CAMINO_MAS_CORTO: camino_mas_corto,
        CANCIONES_MAS_IMPORTANTES: canciones_mas_importantes,
        RECOMENDACION: recomendacion,
        CICLO: ciclo_canciones,
        RANGO: todas_en_rango
    }
    return funciones


def ejecutar_comando(funciones, comando):
    if comando in funciones:
        return funciones[comando]()
    else:
        return False


def iniciar_recomendify(argv):
    grafo_usuarios, grafo_playlists = modelaje_grafos(argv[1])
    f_disponibles = hash_funciones()
    while True:
        comando = input()
        if comando == '':
            break
        if not ejecutar_comando(f_disponibles, comando):
            print("")


if __name__ == "__main__":
    iniciar_recomendify(sys.argv)