from glob import escape
from grafo import Grafo
from grafo_funciones import bfs_origen_destino, reconstruir_camino, imprimir_camino_minimo, bfs_vertices_a_distancia


def modelaje_grafos(ruta):
    usuarios_canciones = Grafo()
    playlists_canciones = Grafo()
    diccionario_playlists = {}
    canciones = set()
    usuarios = set()


    with open(ruta) as archivo:
        linea = archivo.readline() # Primer linea no contiene informacion
        linea = archivo.readline()

        while linea != '':
            datos = linea.rstrip("\n").split("\t")
            _, usuario, cancion, artista, id_playlist, playlist, _ = datos
            cancion_artista = cancion + " - " + artista
            canciones.add(cancion_artista)
            usuarios.add(usuario)
            confeccion_grafo_usuarios_canciones(usuarios_canciones, usuario, cancion_artista, playlist)
            playlists_a_diccionario(diccionario_playlists, id_playlist, cancion_artista)
            linea = archivo.readline()

    confeccion_grafo_playlists_canciones(playlists_canciones, diccionario_playlists)
    return usuarios_canciones, playlists_canciones, canciones, usuarios

def confeccion_grafo_usuarios_canciones(grafo, usuario, cancion, playlist):
    if usuario not in grafo:
        grafo.agregar_vertice(usuario)
    
    if cancion not in grafo:
        grafo.agregar_vertice(cancion)

    if not grafo.estan_unidos(usuario, cancion):
        grafo.agregar_arista(usuario, cancion, playlist)

def playlists_a_diccionario(diccionario, id_playlist, cancion):
    if id_playlist not in diccionario:
        diccionario[id_playlist] = set()
    
    diccionario[id_playlist].add(cancion)

def confeccion_grafo_playlists_canciones(grafo, diccionario):
    for canciones in diccionario.values():
        for cancion_1 in canciones:
            if cancion_1 not in grafo:
                grafo.agregar_vertice(cancion_1)
            
            for cancion_2 in canciones:
                if cancion_1 == cancion_2:
                    continue

                if cancion_2 not in grafo:
                    grafo.agregar_vertice(cancion_2)
                
                if not grafo.estan_unidos(cancion_1, cancion_2):
                    grafo.agregar_arista(cancion_1, cancion_2)
    return

def es_cancion(canciones, cancion):
    if cancion in canciones:
        return True
    return False

def es_usuario(usuarios, usuario):
    if usuario in usuarios:
        return True
    return False

