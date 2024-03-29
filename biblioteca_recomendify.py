from glob import escape
from grafo import Grafo
from grafo_funciones import random_walk

RECORRIDOS_PRANK_PERSONALIZADO = 10000
PRANK_LARGO_RECORRIDO = 10

""" 
*****************************************************************
                MODELAJE Y CREACION DE ESTRUCTURAS
*****************************************************************
"""
def modelaje_grafos(ruta):
    """
    Dada una ruta de un archivo, procesa los datos creando un grafo para el modelaje de Recomendify
    Pre: Recibe una ruta existente
    Post: Devuelve un grafo, un set de usuarios y otro de canciones
    """
    usuarios_canciones = Grafo()
    canciones = set()
    usuarios = set()

    with open(ruta, encoding="utf-8") as archivo:
        linea = archivo.readline()  # Primer linea no contiene informacion
        linea = archivo.readline()

        while linea != '':
            datos = linea.rstrip("\n").split("\t")
            _, usuario, cancion, artista, _, playlist, _ = datos
            cancion_artista = cancion + " - " + artista
            canciones.add(cancion_artista)
            usuarios.add(usuario)
            confeccion_grafo_usuarios_canciones(usuarios_canciones, usuario, cancion_artista, playlist)
            linea = archivo.readline()
    return usuarios_canciones, canciones, usuarios 

def proyeccion_grafo_bipartito(grafo, canciones):
    """"
    Dado un grafo bipartito, realiza otro que sera su proyeccion
    Pre: Recibe un grafo y un set de canciones
    Post: Devuelve un nuevo grafo
    """
    grafo_proyeccion = Grafo()
    for cancion in canciones:
        grafo_proyeccion.agregar_vertice(cancion)

    for v in grafo:
        if v in canciones:
            continue
        for w in grafo.adyacentes(v):
            for w2 in grafo.adyacentes(v):
                if not grafo_proyeccion.estan_unidos(w, w2):
                    grafo_proyeccion.agregar_arista(w, w2)
    return grafo_proyeccion


def confeccion_grafo_usuarios_canciones(grafo, usuario, cancion, playlist):
    """
    Dado un grafo, un usuario, una cancion y una playlist crea vertices y aristas para el grafo pasado
    """
    if usuario not in grafo:
        grafo.agregar_vertice(usuario)
    
    if cancion not in grafo:
        grafo.agregar_vertice(cancion)

    if not grafo.estan_unidos(usuario, cancion):
        grafo.agregar_arista(usuario, cancion, playlist)

""" 
*****************************************************************
                    VALIDACIONES DE DATOS
*****************************************************************
"""
def es_cancion(canciones, cancion):
    """
    Devuelve True si la cancion pasada por parametro es efectivamente una cancion del grafo, False en caso contrario
    """
    if cancion in canciones:
        return True
    return False


def es_usuario(usuarios, usuario):
    """
    Devuelve True si el usuario pasado por parametro es efectivamente un usuario del grafo, False en caso contrario
    """
    if usuario in usuarios:
        return True
    return False

""" 
*****************************************************************
                    FUNCIONES PARA COMANDOS
*****************************************************************
"""
def calculo_pagerank(grafo, interacciones, d, grados):
    
    """Dado un grafo, un numero de interacciones, un coeficiente de amortiguacion y un diccionario de grados devuelve el PageRank de cada vertice
    Pre: Recibe un grafo, un numero(interacciones), un numero(coeficiente de amortiguacion) y un diccionario de grados del grafo
    Post: Devuelve un diccionario con los PageRank calculados
    """
    pagerank = {}
    cant_vertices = len(grafo)
    valor_inicial = 1 / cant_vertices
    for v in grafo:
        pagerank[v] = valor_inicial

    for _ in range(interacciones):
        nuevo_pagerank = {}
        for v in grafo:
            suma = 0
            nuevo_pagerank[v] = pagerank[v]
            for w in grafo.adyacentes(v):
                if v == w:
                    continue
                suma += pagerank[w] / grados[w]
            nuevo_pagerank[v] += suma * d
        
        pagerank = nuevo_pagerank

    return pagerank


def calcular_recomendados(grafo_usuarios, lista_canciones):
    """
    Dado un grafo, una lista de canciones favoritas, se calcula el random_walk
    para todas las canciones en la lista que se encuentren dentro del grafo y se devuelve
    una lista de las canciones / usuarios recomendados
    Pre: recibe un grafo, una cantidad y una lista de canciones
    Post: devuelve una lista de canciones / usuarios recomendados
    """

    pesos_acumulados = {}  # diccionario de diccionarios
    suma_total = {}
    contador = {}
    promedio = {}

    for cancion in lista_canciones:
        pesos_acumulados[cancion] = random_walk(grafo_usuarios, cancion, PRANK_LARGO_RECORRIDO, RECORRIDOS_PRANK_PERSONALIZADO)
    sumar_valores_recomendados(pesos_acumulados, suma_total, contador)

    for vertice in suma_total:
        promedio[vertice] = suma_total[vertice] / contador[vertice]

    promedios_ordenados = sorted(promedio.items(), key=lambda x: x[1], reverse=True)
    return promedios_ordenados


def sumar_valores_recomendados(random_walks, suma_total, contador):
    """
    Permite sumar valores recomendados, resultado de los random walks de diferentes canciones.
    Pre: random_walks es un diccionario de diccionarios, clave: canciones, valor: diccionario
    suma_total es un diccionario con clave: cancion, valor: int
    contador es un diccionario con clave: cancion, valor: int
    Post: suma_total y contador son actualizados con nuevos valores
    """
    for cancion in random_walks:
        for vertice in random_walks[cancion]:

            if vertice not in contador:
                contador[vertice] = 0
            if vertice not in suma_total:
                suma_total[vertice] = 0

            contador[vertice] += 1
            suma_total[vertice] += random_walks[cancion][vertice]


def obtener_lista_recomendados(vertices_resultados, usuarios, canciones, cantidad, opcion, lista_canciones):
    """
    Permite obtener una lista de vertices recomendados. Si la opcion es canciones, la lista sera
    de canciones, lo mismo si la opcion es de usuarios.
    Pre: vertices_resultados es un diccionario con clave: vertice, valor: valor de los random walks de
    usuarios y canciones sets, cantidad > 0
    Post: devuelve una lista con la cantidad pedida de usuarios o canciones recomendadas
    """
    recomendados = []

    for vertice in vertices_resultados:
        if len(recomendados) < cantidad:
            if opcion == "canciones":
                if vertice[0] in canciones and vertice[0] not in lista_canciones:
                    recomendados.append(vertice[0])
                else:
                    continue
            else:
                if vertice[0] in usuarios:
                    recomendados.append(vertice[0])
                else:
                    continue
        else:
            break

    return recomendados

""" 
*****************************************************************
                FUNCIONES PROCESAMIENTO DE DATOS
*****************************************************************
"""

def procesamiento_entrada_camino_minimo(entrada):
    """
    Recibe la entrada para el comando de camino minimo y devuelve una tupla de ambas canciones, o None si la entrada es incorrecta
    """
    datos = entrada.split(" >>>> ")
    if len(datos) != 2:
        return None

    cancion_1, cancion_2 = datos[0], datos[1]

    return cancion_1, cancion_2


def procesamiento_entrada_numero_cancion(entrada):
    """
    Recibe la entrada que deberia contener un numero y una cancion, y devuelve una tupla de ambas cosas, o None si la entrada es incorrecta
    """
    datos = entrada.split(" ")
    numero = datos[0]
    cancion = ""
    for i in range(1, len(datos) - 1):
        cancion += datos[i] + " "

    cancion += datos[-1]
    if cancion == "":
        return None

    return numero, cancion


def procesamiento_recomendacion(entrada, canciones_disponibles):
    """
    Procesa la entrada por consola de la funcion recomendacion
    Si la opcion elegida o la cantidad no son validos, eleva un SyntaxError
    """
    opcion_elegida = entrada.split(' ')[0]
    cantidad = entrada.split(' ')[1]
    opciones_validas = ["canciones", "usuarios"]

    if not cantidad.isnumeric() or opcion_elegida not in opciones_validas:
        raise ValueError

    canciones = entrada[len(cantidad + opcion_elegida) + 2:]
    lista_canciones = canciones.split(" >>>> ")
    lista_canciones_a_recomendar = []
    for cancion in lista_canciones:
        if cancion in canciones_disponibles:
            lista_canciones_a_recomendar.append(cancion)
    return opcion_elegida, int(cantidad), lista_canciones_a_recomendar


""" 
*****************************************************************
                    FUNCIONES PARA IMPRIMIR
*****************************************************************
"""

def imprimir_camino_minimo(camino, aristas):
    """
    Imprime un camino (de canciones) y las aristas (playlists) que lo conecta
    Pre: Recibe el camino y la lista de aristas
    Post: Imprime en formato
    """
    cadena = ''
    for i, vertice in enumerate(camino):
        if i == len(camino) - 1:
            break

        if i % 2 == 0:  # Canciones
            cadena += vertice + ' --> aparece en playlist --> ' + aristas[i] + ' --> de --> '
        
        else:  # Usuarios
            cadena += vertice + ' --> tiene una playlist --> ' + aristas[i] + ' --> donde aparece --> '
    
    cadena += camino[-1]
    
    print(cadena)


def imprimir_mas_importantes(pagerank, n):
    """
    Imprime las n canciones mas importantes del grafo, segun pagerank
    Pre: Recibe el pagerank ordenado de menor a mayor y un numero
    Post: Imprime en formato
    """
    cadena = ""
    largo = len(pagerank) - 1
    for i in range(n - 1):
        cadena += pagerank[i][0] + "; "

    cadena += pagerank[n - 1][0]
    print(cadena)


def imprimir_recomendaciones(recomendados):
    """
    Dada una lista de recomendados, imprime segun el formato establecido en la consigna
    Pre: Recibe la lista de recomendados
    Post: Imprime en formato
    """
    cadena = ""
    for i in range(len(recomendados) - 1):
        cadena += recomendados[i] + "; "

    cadena += recomendados[-1]
    print(cadena)