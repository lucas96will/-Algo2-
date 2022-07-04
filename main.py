#!/usr/bin/python3
from recomendify import Recomendify
import sys

ARG_ARCHIVO_PROCESADO = 1
def main(argv):
    if len(argv) != 2:
        return
    recomendify = Recomendify(argv[ARG_ARCHIVO_PROCESADO])
    recomendify.iniciar_recomendify()


if __name__ == "__main__":
    main(sys.argv)