from grafo import Grafo
from cola import Cola
from pila import Pila
from heapq import heappush, heappop
from union_find import UnionFind
import random
GRUPO_1 = 1
GRUPO_2 = 2

""" 
*****************************************************************
                     RECORRIDOS DEL GRAFO
*****************************************************************
"""


def bfs(grafo):
    """
    Recorrido BFS
    Pre: recibe un grafo
    Post: devuelve el orden y los padres de los vertices de un grafo
    """
    visitados = set()
    padres = {}
    orden = {}
    for v in grafo.obtener_vertices():
        if v not in visitados:
            visitados.add(v)
            padres, orden = _bfs(grafo, v, visitados)
    return padres, orden


def _bfs(grafo, v, visitados):
    padres = {}
    orden = {}
    q = Cola()
    q.encolar(v)
    padres[v] = None
    orden[v] = 0
    while not q.esta_vacia():
        v = q.desencolar()
        for w in grafo.adyacentes(v):
            if w not in visitados:
                padres[w] = v
                orden[w] = orden[v] + 1
                visitados.add(w)
                q.encolar(w)
    return padres, orden


def bfs_origen_destino(grafo, origen, destino):
    """
    Recorrido BFS desde un origen hasta llegar a un destino
    Pre: recibe un grafo
    Post: los padres de un grafo (desde origen hasta destino como maximo). None si no es conexo origen con destino 
    """
    visitados = set()
    padres = {}
    padres[origen] = None
    visitados.add(origen)

    q = Cola()
    q.encolar(origen)
    while not q.esta_vacia():
        v = q.desencolar()
        for w in grafo.adyacentes(v):
            if w not in visitados:
                padres[w] = v
                visitados.add(w)
                q.encolar(w)
                if w == destino:
                    return padres

    return None


def bfs_vertices_a_distancia(grafo, origen, n):
    """
    Recorrido BFS desde un origen, viendo distancias
    Pre: recibe un grafo
    Post: la cantidad de vertices a n distancia del origen
    """
    visitados = set()
    orden = {}
    orden[origen] = 0
    visitados.add(origen)
    contador = 0

    q = Cola()
    q.encolar(origen)
    while not q.esta_vacia():
        v = q.desencolar()
        for w in grafo.adyacentes(v):
            if w not in visitados:
                orden[w] = orden[v] + 1
                visitados.add(w)
                if orden[w] == n:
                    contador += 1
                if orden[w] > n:
                    break
                q.encolar(w)

    return contador


def dfs(grafo):
    """
    Recorrido DFS
    Pre: recibe un grafo
    Post: devuelve el orden y los padres de los vertices de un grafo
    """
    visitados = set()
    padres = {}
    orden = {}
    for v in grafo.obtener_vertices():
        if v not in visitados:
            visitados.add(v)
            padres[v] = None
            orden[v] = 0
            _dfs(grafo, v, visitados, padres, orden)
    return padres, orden


def _dfs(grafo, v, visitados, padres, orden):
    for w in grafo.adyacentes(v):
        if w not in visitados:
            visitados.add(w)
            padres[w] = v
            orden[w] = orden[v] + 1
            _dfs(grafo, w, visitados, padres, orden)
    return


""" 
*****************************************************************
                         GRADOS DEL GRAFO
*****************************************************************
"""


def grados(grafo):
    """
    Grados grafo no dirigido
    Pre: recibe un grafo no dirigido
    Post: devuelve los grados de los vertices
    """
    grado = {}
    for v in grafo:
        grado[v] = 0
    for v in grafo:
        grado[v] = len(grafo.adyacentes(v))
    return grado


def grados_entrada(grafo):
    """
    Grados de entrada grafo dirigido
    Pre: recibe un grafo dirigido
    Post: devuelve los grados de entrada de los vertices
    """
    g_entrada = {}
    for v in grafo:
        g_entrada[v] = 0
    for v in grafo:
        for w in grafo.adyacentes(v):
            g_entrada[w] += 1
    return g_entrada


def grados_salida(grafo):
    """
    Grados de salida grafo dirigido
    Pre: recibe un grafo dirigido
    Post: devuelve los grados de sakida de los vertices
    """
    g_salida = {}
    for v in grafo:
        g_salida[v] = 0
    for v in grafo:
        g_salida[v] = len(grafo.adyacentes(v))
    return g_salida


""" 
*****************************************************************
                    ORDEN TOPOLOGICO DEL GRAFO
*****************************************************************
"""


def topologico_grados(grafo):
    """
    Orden topologico de un grafo, segun algoritmo tipo BFS
    Pre: recibe un grafo
    Post: devuelve el orden topologico del grafo
    """
    g_ent = grados_entrada(grafo)
    q = Cola()
    for v in grafo:
        if g_ent[v] == 0:
            q.encolar(v)
    resultado = []
    while not q.esta_vacia():
        v = q.desencolar()
        resultado.append(v)
        for w in grafo.adyacentes(v):
            g_ent[w] -= 1
            if g_ent[w] == 0:
                q.encolar(w)
    return resultado


def topologico_dfs(grafo):
    """
    Orden topologico de un grafo, segun algoritmo tipo DFS
    Pre: recibe un grafo
    Post: devuelve el orden topologico del grafo
    """
    visitados = set()
    pila = Pila()
    for v in grafo:
        if v not in visitados:
            _topologico_dfs(grafo, v, visitados, pila)

    return pila_a_lista(pila)


def _topologico_dfs(grafo, v, visitados, pila):
    visitados.add(v)
    for w in grafo.adyacentes(v):
        if w not in visitados:
            _topologico_dfs(grafo, w, visitados, pila)
    pila.apilar(v)


def pila_a_lista(pila):
    lista = []

    while not pila.esta_vacia():
        lista.append(pila.desapilar())
    
    return lista


""" 
*****************************************************************
                    CAMINOS MINIMOS DEL GRAFO
*****************************************************************
"""


def camino_minimo_dijkstra(grafo, origen):
    """
    Camino minimo de un grafo para un vertice hacia los demas, segun algoritmo de Dijkstra
    Pre: recibe un grafo
    Post: devuelve el camino minimo de un vertice hacia todos los demas
    """
    dist = {}
    padre = {}
    for v in grafo:
        dist[v] = float("inf")
    dist[origen] = 0
    q = []  # Sera un heap
    heappush(q, (0, origen))
    while len(q) > 0:
        _, v = heappop(q)
        for w in grafo.adyacentes(v):
            distancia_por_aca = dist[v] + grafo.peso(v, w)
            if distancia_por_aca < dist[w]:
                dist[w] = distancia_por_aca
                padre[w] = v
                heappush(q, (dist[w], w))
    return padre, dist


def camino_minimo_bf(grafo, origen):
    """
    Camino minimo de un grafo para un vertice hacia los demas, segun algoritmo de Bellman-Ford
    Pre: recibe un grafo
    Post: devuelve el camino minimo de un vertice hacia todos los demas
    """
    distancia = {}
    padre = {}
    for v in grafo:
        distancia[v] = float("inf")
    distancia[origen] = 0
    padre[origen] = None
    aristas = grafo.obtener_aristas()
    for i in range(len(grafo)):
        cambio = False
        for origen, destino, peso in aristas:
            if distancia[origen] + peso < distancia[destino]:
                cambio = True
                padre[destino] = origen
                distancia[destino] = distancia[origen] + peso
            if not cambio:
                break

    for v, w, peso in aristas:
        if distancia[v] + peso < distancia[w]:
            return None   # Hay ciclo negativo

    return padre, distancia


""" 
*****************************************************************
                CENTRALIDAD DE UN GRAFO (DE BETWEENNESS)
*****************************************************************
"""


def centralidad(grafo, camino_minimo):
    """
    Centralidad de betweenness de un grafo
    Pre: recibe un grafo y el algoritmo de camino minimo a usar
    Post: devuelve la centralidad de betweenness de todos los vertices
    """
    cent = {}
    for v in grafo:
        cent[v] = 0

    for v in grafo:
        padre, distancia = camino_minimo(grafo, v)
        for w in grafo or padre[w] is None:
            if v == w:
                continue
            
            actual = padre[w]

            while actual != v:
                cent[actual] += 1
                actual = padre[actual]
    
    return cent


""" 
*****************************************************************
            ARBOLES DE TENDIDO MINIMO DE UN GRAFO
*****************************************************************
"""


def mst_prim(grafo):
    """
    Arbol de tendido minimo de un grafo, segun algoritmo de Prim
    Pre: recibe un grafo
    Post: devuelve el arbool de tendido minimo del grafo
    """
    v = grafo.vertice_aleatorio()
    visitados = set()
    visitados.add(v)
    q = []  # heap
    for w in grafo.adyacentes(v):
        heappush(q, (grafo.peso_arista(v, w), (v, w)))

    arbol = Grafo()
    for v in grafo:
        arbol.agregar_vertice(v)
    
    while len(q) == 0:
        peso, v, w = heappop(q)
        if w in visitados:
            continue
        arbol.agregar_arista(v, w, peso)
        visitados.add(w)
        for x in grafo.adyacentes(w):
            if x not in visitados:
                heappush(q, (grafo.peso_arista(w, x), (w, x)))
    return arbol


def mst_kruskal(grafo):
    """
    Arbol de tendido minimo de un grafo, segun algoritmo de Kruskal
    Pre: recibe un grafo
    Post: devuelve el arbool de tendido minimo del grafo
    """
    conjunto = UnionFind()
    conjunto.make_set(grafo.obtener_vertices())
    aristas = grafo.obtener_aristas()
    aristas.sort(key=lambda x: x[1])
    arbol = Grafo()
    for v in grafo:
        arbol.agregar_vertice(v)
    for a in aristas:
        v, w, peso = a
        if conjunto.find(v) == conjunto.find(w):
            continue
        arbol.agregar_arista(v, w, peso)
        conjunto.union(v, w)

    return arbol


""" 
*****************************************************************
                CAMINO HAMILTONEANO DE UN GRAFO
*****************************************************************
"""


def camino_hamiltoneano(grafo):
    """
    Camino Hamiltoneano
    Pre: recibe un grafo no dirigido
    Post: devuelve un camno hamiltoneano, si es que hay
    """
    camino = []
    visitados = set()

    for v in grafo:
        if camino_hamiltoneano_dfs(grafo, v, visitados, camino):
            return camino
    return None


def camino_hamiltoneano_dfs(grafo, v, visitados, camino):
    visitados.add(v)
    camino.append(v)
    if len(visitados) == len(grafo):
        return True
    
    for w in grafo.adyacentes(v):
        if w not in visitados:
            if camino_hamiltoneano_dfs(grafo, w, visitados, camino):
                return True
    
    visitados.remove(v)
    camino.pop()
    return False


""" 
*****************************************************************
            PUNTOS DE ARTICULACION DE UN GRAFO
*****************************************************************
"""


def puntos_articulacion(grafo):
    """
    Algoritmo de Tarjan para la deteccion de puntos de articulacion
    Pre: recibe un grafo no dirigido
    Post: devuelve una set con los puntos de articulacion, si es que hay
    """
    origen = grafo.vertice_aleatorio()
    resul = set()

    dfs_puntos_articulacion(grafo, origen, {origen}, {origen: None}, {origen: 0}, {}, resul, True)
    
    return resul


def dfs_puntos_articulacion(grafo, v, visitados, padre, orden, mas_bajo, ptos, es_raiz):
    
    hijos = 0
    mas_bajo[v] = orden[v]
    for w in grafo.adyacentes(v):
        if w not in visitados:
            hijos += 1
            orden[w] = orden[v] + 1
            padre[w] = v
            visitados.add(w)
            dfs_puntos_articulacion(grafo, w, visitados, padre, orden, mas_bajo, ptos, False)

            if mas_bajo[w] >= orden[v]:
                ptos.add(v)

        if padre[v] != w:
            mas_bajo[v] = min(mas_bajo[v], mas_bajo[w])

    if es_raiz and hijos > 1:
        ptos.add(v)


""" 
*****************************************************************
        COMPONENTES FUERTEMENTE CONEXAS DE UN GRAFO
*****************************************************************
"""


def cfcs_grafo(grafo):
    """
    Algoritmo de Tarjan para la deteccion de componentes fuertemente conexas
    Pre: recibe un grafo no dirigido
    Post: devuelve una lista con las componentes fuertemente conexas, si es que hay
    """
    resultados = []
    visitados = set()
    for v in grafo:
        if v not in visitados:
            dfs_cfc(grafo, v, visitados, {}, {}, Pila(), set(), resultados, [0])

    return resultados


def dfs_cfc(grafo, v, visitados, orden, mas_bajo, pila, apilados, cfcs, contador_global):

    orden[v] = mas_bajo[v] = contador_global[0]
    contador_global[0] += 1
    visitados.add(v)
    pila.apilar(v)
    apilados.add(v)

    for w in grafo.adyacentes(v):
        if w not in visitados:
            dfs_cfc(grafo, w, visitados, orden, mas_bajo, pila, apilados, cfcs, contador_global)

        if w in apilados:
            mas_bajo[v] = min(mas_bajo[v], mas_bajo[w])

    if orden[v] == mas_bajo[v]:
        nueva_cfc = []
        while True:
            w = pila.desapilar()
            apilados.remove(w)
            nueva_cfc.append(w)
            if w == v:
                break

        cfcs.append(nueva_cfc)


""" 
*****************************************************************
                        OTRAS FUNCIONES
*****************************************************************
"""


def obtener_ciclo_bfs(grafo, dirigido=True):
    """
    Obtiene el ciclo de un grafo, usando un recorrido tipo BFS
    Pre: recibe un grafo
    Post: devuelve una lista de vertices que forman el ciclo
    """
    visitados = {}
    for v in grafo:
        if v not in visitados:
            ciclo = _obtener_ciclo_bfs(grafo, v, visitados, dirigido)
            if ciclo is not None:
                return ciclo
    return None


def _obtener_ciclo_bfs(grafo, v, visitados, dirigido):
    visitados[v] = True
    q = Cola()
    q.encolar(v)
    padre = {v: None}

    while not q.esta_vacia():
        v = q.desencolar()
        for w in grafo.adyacentes(v):
            if w in visitados:
                if dirigido is False and padre[v] == w:
                    continue
                return reconstruir_camino(padre, v, w)
            else:
                q.encolar(w)
                visitados[w] = True
                padre[w] = v
    return None


def reconstruir_camino(grafo, padre, inicio, fin):
    """
    Reconstruye el ciclo a partir del padre, el inicio y el fin
    Pre: hay un ciclo, cada vertice tiene un padre
    Post: devuelve una lista de vertices que forman el ciclo
    """
    v = fin
    camino = []
    aristas = []
    while v != inicio:
        camino.append(v)
        v = padre[v]
    camino.append(inicio)
    camino.reverse()
    for i in range(0, len(camino) - 1):
        aristas.append(grafo.peso_arista(camino[i], camino[i + 1]))

    return camino, aristas


def es_bipartito(grafo):
    """
    Determina si un grafo es bipartito, o no
    Pre: recibe un grafo
    Post: Devuelve True si es bipartito, false en caso contrario
    """
    grupos = {}
    for v in grafo:
        if v not in grupos:
            if not _esbipartito(grafo, v, grupos):
                return False
    
    return True


def _esbipartito(grafo, v, grupos):
    grupos[v] = GRUPO_1

    cola = Cola()
    cola.encolar(v)

    while not cola.esta_vacia():
        v = cola.desencolar()
        for w in grafo.adyacentes(v):
            if w in grupos:
                if grupos[w] == grupos[v]:
                    return False
            else:
                grupos[w] = GRUPO_1 if grupos[v] == GRUPO_2 else GRUPO_2
                cola.encolar(w)

    return True


def ciclo_origen_y_largo(grafo, origen, n):
    """
    Halla un ciclo desde el origen de largo n
    Pre: Recibe un grafo, un origen y un numero n
    Post: Devuelve el ciclo de ser hallado
    """
    camino = []
    visitados = set()
    if _ciclo_origen_y_largo(grafo, origen, n, camino, visitados, origen):
        return camino

    return None


def _ciclo_origen_y_largo(grafo, v, n, camino, visitados, origen):
    visitados.add(v)
    camino.append(v)

    for w in grafo.adyacentes(v):
        if w == origen and len(camino) == n:
            camino.append(origen)
            return True

        if w not in visitados and len(camino) < n:
            if _ciclo_origen_y_largo(grafo, w, n, camino, visitados, origen):
                return True

    visitados.remove(v)
    camino.pop()
    return False


def imprimir_camino(camino):
    """
    Imprime un camino
    Pre: Recibe los vertices que componen el camino
    Post: Se imprimio el camino
    """
    cadena = ''
    for i in range(len(camino) - 1):
        cadena += camino[i] + ' --> '

    cadena += camino[-1]
    print(cadena)


def random_walk(grafo, v_actual, largo_recorrido, iteraciones):
    """
    Algoritmo del random_walk (personalizado)
    Empezando desde un vertice, visito un vecino aleatorio y le sumo un valor dado por la cantidad
    de adyacentes del vertice inicial, repitiendo esto por el largo del recorrido y
    por la cantidad de iteraciones. Por cada random walk que se ejecuta, guardo los valores en un
    diccionario de valor acumulado y en otro diccionario la cantidad de veces que aparece ese vertice.
    Al terminar todas las iteraciones promedio los valores acumulados
    Pre: Grafo no dirigido, v_actual vertice pertenece al grafo, largo_recorrido > 0 , iteraciones >>> 0
    Post: devuelve un hash de clave: vertices y valor: valor acumulado (int)
    """

    valor_acumulado = {}
    contador = {}

    for _ in range(iteraciones):
        vertice_inicial = v_actual
        resultado = {v_actual: 0}
        for n in range(largo_recorrido):
            v_inicial_adyacentes = grafo.adyacentes(vertice_inicial)
            v_aleatorio = random.choice(v_inicial_adyacentes)

            if v_aleatorio not in resultado:
                resultado[v_aleatorio] = 0

            if v_aleatorio not in valor_acumulado:
                valor_acumulado[v_aleatorio] = 0
            if v_aleatorio not in contador:
                contador[v_aleatorio] = 0

            if resultado[vertice_inicial] != 0:
                resultado[v_aleatorio] += resultado[vertice_inicial] / len(v_inicial_adyacentes)
            else:
                resultado[v_aleatorio] += 1/len(v_inicial_adyacentes)

            valor_acumulado[v_aleatorio] += resultado[v_aleatorio]
            contador[v_aleatorio] += 1

            vertice_inicial = v_aleatorio

    for vertice in valor_acumulado:
        valor_acumulado[vertice] /= contador[vertice]

    return valor_acumulado