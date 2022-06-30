from biblioteca_recomendify import modelaje_grafos

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
            self.grafo_usuarios, self_grafo_playlists = modelaje_grafos(ruta)
            self.inicializacion_correcta = True
            self.funciones = self._hash_funciones()
        except FileNotFoundError:
            print("No se encontro el archivo!")
            self.incializacion_correcta = False

    """
    Post: devuelve un diccionario donde cada clave es un comando y el valor 
    la funcione correspondiente
    """
    def _hash_funciones(self):
        funciones = {
            CAMINO_MAS_CORTO: self.camino_mas_corto,
            CANCIONES_MAS_IMPORTANTES: self.canciones_mas_importantes,
            RECOMENDACION: self.recomendacion,
            CICLO: self.ciclo_canciones,
            RANGO: self.todas_en_rango
        }
        return funciones

    """
    Inicia el programa recomendify, tomando por consola el input de comandos
    """
    def iniciar_recomendify(self):
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

    """
    """
    def camino_mas_corto(self, entrada: str):
        """ejemplo de entrada: camino Don't Go Away - Oasis >>>> Quitter - Eminem
        """

        pass

    """
    """
    def canciones_mas_importantes(self, entrada: str):
        pass

    """
    """
    def recomendacion(self, entrada: str):
        pass

    """
    """
    def ciclo_canciones(self, entrada: str):
        pass

    """
    """
    def todas_en_rango(self, entrada: str):
        pass

