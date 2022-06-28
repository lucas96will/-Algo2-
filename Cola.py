from collections import deque


class Node:
    def __init__(self, item = None):
        self.item = item
        self.sig = None
        self.anterior = None


class Cola:
    def __init__(self):
        self.largo = 0
        self.primero = None
        self.ultimo = None

    def encolar(self, value):
        nuevo_nodo = Node(value)
        if self.primero is None:
            self.primero = self.ultimo = nuevo_nodo
        else:
            self.ultimo.sig = nuevo_nodo
            nuevo_nodo.anterior = self.ultimo
            self.ultimo = nuevo_nodo
        self.largo += 1

    def desencolar(self):
        item = self.primero.item
        self.primero = self.primero.sig
        self.largo -= 1
        if self.largo == 0:
            self.ultimo = None
        return item

    def esta_vacia(self):
        return self.largo == 0

    def ver_primero(self):
        return self.primero

