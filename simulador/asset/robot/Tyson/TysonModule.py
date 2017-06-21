import TysonReal
import TysonVirtual

class Tyson:
    def __init__(self):
        self.realEnable = True
        self.virtual = TysonVirtual()
        self.real = TysonReal()
        self.real.conectar()

    def conectar(self):
        if self.realEnable:
            self.real.conectar()
        if self.virtualEnable:
            self.virtual.conectar()
