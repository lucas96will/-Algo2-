#!/usr/bin/python3
from recomendify import Recomendify
import sys

ARG_ARCHIVO_PROCESADO = 1


if __name__ == "__main__":
    recomendify = Recomendify(sys.argv[1])
    recomendify.iniciar_recomendify()
