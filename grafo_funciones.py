from grafo import Grafo




# grados_entrada
def grados_entrada(grafo):
    g_ent = {}
    for v in grafo:
        g_ent[v] = 0
    for v in grafo:
        for w in grafo.adyacentes(v):
            g_ent[w] += 1
    return g_ent


def topologico_grados(grafo):
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




# dijkstra
def camino_minimo_dijkstra(grafo, origen):
    dist = {}
    padre = {}
    for v in grafo:
        dist[v] = float("inf")
    dist[origen] = 0
    q = Heap()
    q.encolar((0, origen))
    while not q.esta_vacio():
        _, v = q.desencolar()
        for w in grafo.adyacentes(v):
            distancia_por_aca = dist[v] + grafo.peso(v, w)
            if distancia_por_aca < dist[w]
                dist[w] = distancia_por_aca
                padre[w] = v
                q.encolar((dist[w], w))
    return padre, dist

# bellman-ford
def camino_minimo_bf(grafo, origen):
    distancia = {}
    padre = {}
    for v in grafo:
        distancia[v] = float("inf")
    distancia[origen] = 0
    padre[origen] = None
    aristas = obtener_aristas(grafo) # O(V+E)
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
            return None # hay un ciclo negativo

    return padre, distancia


# camino minimo prim
# pre: -
# post: devuelve el camino minimo de un grafo
def mst_prim(grafo):
    v = grafo.vertice_aleatorio()
    visitados = set()
    visitados.add(v)
    q = heap_crear()
    for w in grafo.adyacentes(v):
        q.encolar((v, w), grafo.peso_arista(v, w))
    arbol = grafo(grafo.obtener_vertices())
    while not q.esta_vacia():
        (v, w), peso = q.desencolar()
        if w in visitados:
            continue
        arbol.agregar_arista(v, w, peso)
        visitados.add(w)
        for x in grafo.adyacentes(w):
            if x not in visitados:
                q.encolar((w, x), grafo.peso(w, u))
    return arbol


# camino minimo_kruskal
# pre: -
# post: devuelve el camino minimo de un grafo
def mst_kruskal(grafo):
    conjuntos = conjuntos_disjuntos_crear(grafo.obtener_vertices())
    aristas = sort(obtener_aristas(grafo))
    arbol = grafo(grafo.obtener_vertices())
    for a in aristas:
        v, w, peso = a
        if conjuntos.find(v) == conjuntos.find(w):
            continue
        arbol.agregar_arista(v, w, peso)
        conjuntos.union(v, w)
    return arbol


# pre: -
# post: devuelve el orden y los padres de los vertices de un grafo
def bfs(grafo):
    visitados = set()
    padres = {}
    orden = {}
    for v in grafo.obtener_vertices():
        if v not in visitados:
            visitados.add(v)
            padres[v] = None
            orden[v] = 0
            _bfs(grafo, v, visitados, padres, orden)
    return padres, orden


def _bfs(grafo, v, visitados, padres, orden):
    q = cola_crear()
    while not q.esta_vacia():
        v = cola.desesencolar()
        for w in grafo.adyacentes(v):
            if w not in visitados:
                padres[w] = v
                orden[w] = orden[v] + 1
                visitados.add(w)
                cola.encolar(w)
    return


# pre: -
# post: devuelve el orden y los padres de los vertices de un grafo
def dfs(grafo):
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


def obtener_ciclo_bfs(grafo, dirigido=True):
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
                # si el grafo es no dirigido, entonces descontamos
                # el caso de que el ciclo sea formado solo por el padre
                # de un vertice y el vertice mismo
                if dirigido is False and padre[v] == w:
                    continue
                # si el vertice ya fue visitado entonces es un ciclo
                # (en este caso trabajamos con un grafo dirigido)
                return reconstruir_ciclo(padre, v, w)
            else:
                q.encolar(w)
                visitados[w] = True
                padre[w] = v

    return None


# reconstruye el ciclo a partir del padre, el inicio y el fin
# pre: hay un ciclo, cada vertice tiene un padre
# post: devuelve una lista de vertices que forman el ciclo
def reconstruir_ciclo(padre, inicio, fin):
    v = fin
    camino = []
    while v != inicio:
        camino.append(v)
        v = padre[v]
    camino.append(inicio)
    return camino.reverse()