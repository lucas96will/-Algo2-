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
    """
    def __init__(self, ruta):
        try:
            self.grafo_usuarios, self_grafo_playlists = modelaje_grafos(ruta)
            self.inicializacion_correcta = True
        except FileNotFoundError:
            print("No se encontro el archivo!")
            self.incializacion_correcta = False
        self.funciones = self._hash_funciones()

    """
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

