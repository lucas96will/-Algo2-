from biblioteca_recomendify import es_cancion, modelaje_grafos, imprimir_camino_minimo
from grafo_funciones import bfs_origen_destino, bfs_vertices_a_distancia, ciclo_origen_y_largo, reconstruir_camino, imprimir_camino

COMANDO_POR_INPUT = 1

# Comandos disponibles
CAMINO_MAS_CORTO = "camino"
CANCIONES_MAS_IMPORTANTES = "mas_importantes"
RECOMENDACION = "recomendacion"
CICLO = "ciclo"
RANGO = "rango"


class Recomendify:
    """
    Clase Recomendify
    ruta: direccion de archivo tsv
    Post: crea la clase recomendify. Si no existe o no se encuentra
    el archivo, la clase queda inutilizable
    """
    def __init__(self, ruta):
        try:
            self.grafo_usuarios, self._grafo_playlists, self.canciones, self.usuarios = modelaje_grafos(ruta)
            self.inicializacion_correcta = True
            self.funciones = self._hash_funciones()
            self.ranking = []
        except FileNotFoundError:
            print("No se encontro el archivo!")
            self.incializacion_correcta = False

    def _hash_funciones(self):
        """
        Post: devuelve un diccionario donde cada clave es un comando y el valor
        la funcione correspondiente
        """
        funciones = {
            CAMINO_MAS_CORTO: self.camino_mas_corto,
            CANCIONES_MAS_IMPORTANTES: self.canciones_mas_importantes,
            RECOMENDACION: self.recomendacion,
            CICLO: self.ciclo_canciones,
            RANGO: self.todas_en_rango
        }
        return funciones

    def iniciar_recomendify(self):
        """
        Inicia el programa recomendify, tomando por consola el input de comandos
        """
        while self.inicializacion_correcta:
            texto = input()
            if texto == '':
                break
            comando = texto.split(' ')[COMANDO_POR_INPUT]
            if comando not in self.funciones:
                print("")
            else:
                self.funciones[comando](texto)
        return

    def camino_mas_corto(self, cancion_1, cancion_2):
        """
        Imprime una lista con la cual se conecta una canción con otra, en la menor cantidad de pasos
        Pre: Se reciben dos canciones
        Post: Se imprime el camino, de ser posible
        """

        if not es_cancion(self.canciones, cancion_1) or not es_cancion(self.canciones, cancion_2):
            print("Tanto el origen como el destino deben ser canciones")
            return False

        result_bfs = bfs_origen_destino(self.grafo_usuarios, cancion_1, cancion_2)
        if result_bfs is None:
            print("No se encontro recorrido")
            return False

        padres = result_bfs
        camino, aristas = reconstruir_camino(self.grafo_usuarios, padres, cancion_1, cancion_2)

        imprimir_camino_minimo(camino, aristas)
        return True

    """
    """
    def canciones_mas_importantes(self, entrada: str):
        cantidad_ranking_pedida = int(entrada.split(' ')[1])

        if len(self.ranking) < cantidad_ranking_pedida:
            self.ranking = rank_canciones(self._grafo_playlists, cantidad_ranking_pedida)

        for i in range(cantidad_ranking_pedida):
            print(self.ranking[i])
    """
    """
    def recomendacion(self, entrada: str):
        pass
    """
    """
    def ciclo_canciones(self, n, cancion):
        """
        Permite obtener un ciclo de largo n (dentro de la red de canciones) que comience en la canción indicada
        Pre: Se recibe un largo y una cancion
        Post: Imprime el ciclo si existe
        """
        if not es_cancion(self.canciones, cancion) or not n.isnumeric():
            print("Debe pasar un numero y una cancion")
            return False

        camino = ciclo_origen_y_largo(self._grafo_playlists, cancion, n)
        if camino is None:
            print("No se encontro recorrido")
            return False

        imprimir_camino(camino)
        return True

    def todas_en_rango(self, n, cancion):
        """
        Permite obtener la cantidad de canciones que se encuenten a exactamente n saltos desde la cancion pasada por parámetro
        Pre: Recibe el numero de saltos y la cancion
        Post: Imprime la cantidad de canciones encontradas
        """

        if not es_cancion(self.canciones, cancion) or not n.isnumeric():
            print("Debe pasar una cancion")
            return False

        print(bfs_vertices_a_distancia(self._grafo_playlists, cancion, n))

        return True
