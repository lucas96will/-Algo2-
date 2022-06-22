from grafo import Grafo


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
            _bfs(grafo, v, visitados, padres, orden)
    return padres, orden


def _bfs(grafo, v, visitados, padres, orden):
    q = cola_crear()
    visitados.add(v)
    orden[v] = len(visitados)
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
            _dfs(grafo, v, visitados, padres, orden)
    return padres, orden


def _dfs(grafo, v, visitados, padres, orden):
    visitados.add(v)
    for w in grafo.adyacentes(v):
        if w not in visitados:
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
                visitados[v] = True
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
