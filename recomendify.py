from biblioteca_recomendify import obtener_lista_recomendados, calcular_recomendados, procesamiento_recomendacion, calculo_pagerank, es_cancion, imprimir_mas_importantes, modelaje_grafos, imprimir_camino_minimo, procesamiento_entrada_camino_minimo, procesamiento_entrada_numero_cancion
from grafo_funciones import bfs_origen_destino, bfs_vertices_a_distancia, ciclo_origen_y_largo, reconstruir_camino, imprimir_camino, grados

COMANDO_POR_INPUT = 0

# Comandos disponibles
CAMINO_MAS_CORTO = "camino"
CANCIONES_MAS_IMPORTANTES = "mas_importantes"
RECOMENDACION = "recomendacion"
CICLO = "ciclo"
RANGO = "rango"

INTERACCIONES = 20
COEF_AMORTIGUACION = 0.85


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
                inicio_parametros = len(comando) + 1
                entrada = texto[inicio_parametros:]
                self.funciones[comando](entrada)
        return

    def camino_mas_corto(self, entrada):
        """
        Imprime una lista con la cual se conecta una canción con otra, en la menor cantidad de pasos
        Pre: Recibe datos de entrada
        Post: Se imprime el camino, de ser posible
        """
        canciones = procesamiento_entrada_camino_minimo(entrada)
        if not canciones:
            return False
        cancion_1, cancion_2 = canciones
        
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

    def canciones_mas_importantes(self, entrada):
        """
        Imprime las canciones mas importantes del grafo
        Pre: Recibe una entrada
        Post: Devuelve las n canciones mas importantes (segun cuantas quiera ver el usuario)
        """
        cantidad_ranking_pedida = int(entrada.split(' ')[0])

        if self.ranking == []:
            grados_grafo = grados(self._grafo_playlists)
            pagerank = calculo_pagerank(self._grafo_playlists, INTERACCIONES, COEF_AMORTIGUACION, grados_grafo)
            self.ranking = sorted(pagerank.items(), key=lambda x: x[1])

        imprimir_mas_importantes(self.ranking, cantidad_ranking_pedida)

    def recomendacion(self, entrada: str):
        """
        Permite obtener una recomendacion de canciones/usuarios a partir de un listado de n canciones favoritas
        Pre: la entrada indica si se quiere recomendar usuarios o canciones, la cantidad de canciones a recomendar
        y las canciones favoritas
        Ej: usuarios 10 Love Story - Taylor Swift >>>> Toxic - Britney Spears >>>> I wanna be yours - Arctic Monkeys
        Post: Devuelve una lista de usuarios o canciones recomendados, de largo pedido
        """

        datos = None
        try:
            datos = procesamiento_recomendacion(entrada)
        except ValueError:
            print("La cantidad pedida o la opcion a recomendar no es valida!")
            return False

        opcion, cantidad, lista_canciones = datos
        vertices_resultados = calcular_recomendados(self.grafo_usuarios, lista_canciones)
        recomendados = obtener_lista_recomendados(vertices_resultados, self.usuarios, self.canciones, cantidad, opcion)

        for elemento in recomendados:
            print(elemento, end="; ")

    def ciclo_canciones(self, entrada):
        """
        Permite obtener un ciclo de largo n (dentro de la red de canciones) que comience en la canción indicada
        Pre: Recibe datos de entrada
        Post: Imprime el ciclo si existe
        """
        datos = procesamiento_entrada_numero_cancion(entrada)
        if not datos:
            return False

        n, cancion = datos
        
        if not es_cancion(self.canciones, cancion) or not n.isnumeric():
            print("Debe pasar un numero y una cancion")
            return False
        n = int(n)

        camino = ciclo_origen_y_largo(self._grafo_playlists, cancion, n)
        if camino is None:
            print("No se encontro recorrido")
            return False

        imprimir_camino(camino)
        return True

    def todas_en_rango(self, entrada):
        """
        Permite obtener la cantidad de canciones que se encuenten a exactamente n saltos desde la cancion pasada por parámetro
        Pre: Recibe datos de entrada
        Post: Imprime la cantidad de canciones encontradas
        """
        datos = procesamiento_entrada_numero_cancion(entrada)
        if not datos:
            return False

        n, cancion = datos

        if not es_cancion(self.canciones, cancion) or not n.isnumeric():
            print("Debe pasar una cancion")
            return False
        n = int(n)
        print(bfs_vertices_a_distancia(self._grafo_playlists, cancion, n))

        return True
