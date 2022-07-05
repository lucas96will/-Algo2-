from random import choice
NO_EXISTE = None


class Grafo:
    """
    Clase Grafo
    Por defecto no es dirigido, pero podria serlo
    Implementada como diccionario de diccionarios
    """
    def __init__(self, dirigido=False):
        self.grafo = {}  # Diccionario de diccionarios
        self.dirigido = dirigido

    def agregar_vertice(self, v):
        """
        Agrega un vertice al grafo. Devuelve True si se agrego, False si ya estaba en el grafo
        Pre: El grafo fue creado
        Post: Devuele True si se agrego el vertice al grafo. False en caso contrario
        """
        if v in self.grafo:
            return False
        self.grafo[v] = {}
        return True

    def borrar_vertice(self, v):
        """
        Borra el vertice del grafo si es que fue agregado antes
        Pre: El grafo fue creado
        Post: Devuelve True si el vertice pudo ser borrado del grafo. False en caso contrario
        """
        try:
            self.grafo.pop(v)
        except KeyError:
            return False
        
        for w in self.grafo:
            if v in self.grafo[w]:
                self.grafo[w].pop(v)

        return True

    def agregar_arista(self, v, w, peso=1):
        """
        Agrega una arista entre dos vertices del grafo. Por defecto el peso sera de 1
        Pre: El grafo fue creado
        Post: Devuelve True si se pudo aregar la arista. Si ya existia se reemplazara su peso. Devuelve False en caso de error
        """
        try:
            if w not in self.grafo:
                raise KeyError

            self.grafo[v][w] = peso
            if not self.dirigido:
                self.grafo[w][v] = peso
        except KeyError:
            return False
        
        return True
    
    def borrar_arista(self, v, w):
        """
        Borra una arista entre vertices del grafo
        Pre: El grafo fue creado
        Post: Devuelve True si se pudo borrar la arista. Devuelve False en caso de error
        """
        try:
            self.grafo[v].pop(w)
            if not self.dirigido:
                self.grafo[w].pop(v)
        except KeyError:
            return False
        return True

    def estan_unidos(self, v, w):
        """
        Indica si w  esta unido a v
        Pre: El grafo fue creado
        Post: Devuelve True si estan unidos. False en caso contrario
        """
        return w in self.grafo[v]
    
    def peso_arista(self, v, w):
        """
        Devuelve el peso de la arista desde v hasta w
        Pre: El grafo fue creado
        Post: Devuelve el peso de la arista. None si esta no existe
        """
        if not self.estan_unidos(v, w):
            return NO_EXISTE
        return self.grafo[v][w]

    def obtener_vertices(self):
        """
        Devuelve una lista con todos los vertices del grafo
        Pre: El grafo fue creado
        Post: Devuelve la lista, vacia si es que no hay vertices
        """
        return list(self.grafo)

    def obtener_aristas(self):
        """
        Devuelve una lista con todos las aristas del grafo
        Pre: El grafo fue creado
        Post: Devuelve la lista, vacia si es que no hay aristas
        """
        aristas = []
        visitados = set()
        for v in self.grafo:
            for w in self.grafo.adyacentes(v):
                if w not in visitados:
                    peso = self.grafo.peso_arista(v, w)
                    aristas.append((v,w), peso)
            if not self.dirigido:
                visitados.add(v)

        return aristas

    def vertice_aleatorio(self):
        """
        Se elije un vertice del grafo al azar para ser devuelto
        Pre: El grafo fue creado
        Post: Devuelve vertice aleatorio o None si el grafo no tiene vertices
        """
        if len(self.grafo) == 0:
            return None
        
        return choice(list(self.grafo))

    def adyacentes(self, v): 
        """
        Se devuelve una lista con los adyacentes al vertice
        Pre: El grafo fue creado
        Post: Devuelve la lista con los adyacentes, o None si el vertice no esta en el grafo
        """
        try:
            result = list(self.grafo[v])
        except KeyError:
            return NO_EXISTE

        return result

    def __contains__(self, v):
        """
        Devuelve si v esta en el grafo
        Pre: El grafo fue creado
        Post: Devuelve True si v esta en el grafo, False en otro caso
        """
        return v in self.grafo

    def __str__(self):
        """
        Imprime el grafo por pantalla, representado como un diccionario de diccionarios
        Pre: El grafo fue creado
        Post: Se imprimio por pantalla el grafo
        """
        return f"{self.grafo}"

    def __len__(self):
        """
        Devuelve la cantidad de vertices del grafo
        Pre: El grafo fue creado
        Post: Se devolvio la cantidad de vertices
        """
        return len(self.grafo)

    def __iter__(self):
        """Iterador del grafo
        Pre: El grafo fue creado
        Post: Se itera el grafo por vertices
        """
        return iter(self.grafo)