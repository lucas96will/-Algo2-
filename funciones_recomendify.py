from grafo import Grafo

def modelaje_grafos(ruta):
    usuarios_canciones = Grafo()
    playlists_canciones = Grafo()
    diccionario_playlists = {}

    with open(ruta) as archivo:
        linea = archivo.readline() #Primer linea no contiene informacion
        linea = archivo.readline()

        while linea != '':
            datos = linea.rstrip("\n").split("\t")
            _, usuario, cancion, artista, id_playlist, _, _ = datos
            cancion_artista = cancion + " - " + artista
            confeccion_grafo_usuarios_canciones(usuarios_canciones, usuario, cancion_artista)
            playlists_a_diccionario(diccionario_playlists, id_playlist, cancion_artista)
            linea = archivo.readline()

    confeccion_grafo_playlists_canciones(playlists_canciones, diccionario_playlists)
    return usuarios_canciones, playlists_canciones
    
def confeccion_grafo_usuarios_canciones(grafo, usuario, cancion):
    if not usuario in grafo:
        grafo.agregar_vertice(usuario)
    
    if not cancion in grafo:
        grafo.agregar_vertice(cancion)

    if not grafo.estan_unidos(usuario, cancion):
        grafo.agregar_arista(usuario, cancion)

def playlists_a_diccionario(diccionario, id_playlist, cancion):
    if not id_playlist in diccionario:
        diccionario[id_playlist] = set()
    
    diccionario[id_playlist].add(cancion)

def confeccion_grafo_playlists_canciones(grafo, diccionario):
    for canciones in diccionario.values():
        for cancion_1 in canciones:
            if not cancion_1 in grafo:
                    grafo.agregar_vertice(cancion_1)
            
            for cancion_2 in canciones:
                if cancion_1 == cancion_2:
                    continue

                if not cancion_2 in grafo:
                    grafo.agregar_vertice(cancion_2)
                
                if not grafo.estan_unidos(cancion_1, cancion_2):
                    grafo.agregar_arista(cancion_1, cancion_2)


ruta = "archivo.tsv"
print(modelaje_grafos(ruta))