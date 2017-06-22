import serial
import sys
import threading

def serial_ports():
    """ Lists serial port names
        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = {}
    for port in ports:
        try:
            with serial.Serial(port,9600,timeout=100) as ser:
                name = ser.readline().decode('utf-8').rstrip()
                if name!=None:
                    result[name]=port
        except (OSError, serial.SerialException):
            pass
    return result

class TysonModule:
    def __init__(self, moduleName):
        self.moduleName = moduleName
        self.connected = False
        self.port = ''
        self.baud = '9600'
        self.serial_port = []
        self.errors = []
        self.encoderData = []
        self.maxPosition = 1000
        self.minPosition = -1000
        self.__readWorker = threading.Thread(
                target=self.read_from_port,
                name="Module"+moduleName+"_SerialReader")

    def connect(self):
        puertos = serial_ports()
        if self.moduleName in puertos:
            self.port = puertos[self.moduleName]
            self.serial_port = serial.Serial(puertos[self.moduleName])
            self.connected = True
            self.__readWorker.start()
        else:
            print('Error : ',self.moduleName,'no conectado')
        
    def disconnect(self):
        self.connected = False
    
    def handle_data(self,data):
        #TODO:
        # readEncoder
        # readSensorR
        # readSensorL
        # readError
        print(data)
        
    def read_from_port(self):
        while not self.connected:
            pass
        while self.connected:
           reading = self.serial_port.readline().decode()
           self.handle_data(reading)
           
    def motor_rotate_left(self):
        if self.connected:
            self.serial_port.write(b"MOTOR ROTATE LEFT;")
        else:
            print("Module ",self.moduleName," not conected")

    def motor_rotate_right(self):
        if self.connected:
            self.serial_port.write(b"MOTOR ROTATE RIGHT;")
        else:
            print("Module ",self.moduleName," not conected")
            
    def motor_set_speed(self,value):
        pass
            
    def encoder_reset():
        pass
    
    def encoder_log_counter(self,option):
        pass

    def encoder_log_looper(self,option):
        pass

    def set_max_position(self,value):
        pass

    def set_min_position(self,value):
        pass

    def get_encoder_position(self):
        pass

    def calibrate_motor(self):
        pass
    
    
class TysonReal:
    def __init__(self):
        self.BRAZO = TysonModule('BRAZO')
        self.BASE = TysonModule('BASE')
        self.ALTURA = TysonModule('ALTURA')

    def conectar(self):
        print('Conectado a tyson...')
        if not self.BRAZO.connected:
            self.BRAZO.connect()
        if not self.BASE.connected:
            self.BASE.connect()
        if not self.ALTURA.connected:
            self.ALTURA.connect()
    
    def calibrar(self):
        pass

    def sacarBrazo(self):
        pass

    def meterBrazo(self):
        pass

    def rotarBaseDer(self):
        pass

    def rotarBaseIzq(self):
        pass

    def levantarAltura(self):
        pass

    def bajarAltura(self):
        pass


if __name__ == '__main__':
    t = TysonReal()
    t.conectar()
    
