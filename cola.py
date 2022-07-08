class Cola:
    def __init__(self):
        """Crea una cola vacia"""
        self.primero = None
        self.ultimo = None

    def encolar(self, x):
        """Encola el elemento x"""
        nuevo = _Nodo(x)
        if self.ultimo is not None:
            self.ultimo.prox = nuevo
            self.ultimo = nuevo
        else:
            self.primero = nuevo
            self.ultimo = nuevo

    def desencolar(self):
        """Desencola el primer elemento y devuelve su valor.
        Si la cola esta vacia, levanta ValueError"""
        if self.primero is None:
            raise ValueError("La cola esta vacia")
        valor = self.primero.dato
        self.primero = self.primero.prox
        if not self.primero:
            self.ultimo = None

        return valor

    def esta_vacia(self):
        """Devuelve True si la cola esta vacia, False si no"""
        return self.primero is None

    def ver_frente(self):
        if not self.primero:
            raise ValueError("La cola esta vacia")
        return self.primero.dato


class _Nodo:
    def __init__(self, dato, prox=None):
        self.dato = dato
        self.prox = prox