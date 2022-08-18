import sys
from PyQt5.QtWidgets import QApplication
from gui import Ui_MainWindow
from PyQt5.QtCore import Qt, QThread, pyqtSignal
import time
import serial
arduino=serial.Serial("COM4",baudrate=9600,timeout=1)


from PySide2.QtWidgets import *

class MyThread_2(QThread):
    change_value_2 = pyqtSignal(list)

    def run(self):
        while(1):
            readArd = arduino.readline().decode("ascii")
            cnt = readArd.split()
            self.change_value_2.emit(cnt)
            time.sleep(0.002)
class MyThread(QThread):
    change_value = pyqtSignal(list)

    def run(self):
        while(1):
            mesafe = arduino.readline().decode("ascii")
            cnt = mesafe.split()
            self.change_value.emit(cnt)
            time.sleep(0.002)

class ui_windows(QMainWindow):
    def __init__(self):
        super(ui_windows, self).__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.thread = MyThread()
        self.thread_2 = MyThread_2()

        #ComboBox
        self.ui.comboBox.addItem("Mesafe")
        self.ui.comboBox.addItem("Servo")

        #Buttons
        self.ui.startBtnMan.clicked.connect(self.startThread)
        self.ui.stopBtnMan.clicked.connect(self.stopThread)
        self.ui.sendBtn.clicked.connect(self.sendValue)
        self.ui.startBtnOto.clicked.connect(self.otomatikStart)
        #Slider
        self.ui.servo1.valueChanged.connect(self.servoValue)
        self.ui.servo2.valueChanged.connect(self.servoValue2)

        #Action
        self.ui.actionOtomatik.triggered.connect(self.otomatik)
        self.ui.actionManuel.triggered.connect(self.manuel)

    def servoValue(self):
        data = str(self.ui.servo1.value() + 10)
        arduino.write(data.encode())
    def servoValue2(self):
        data = str(self.ui.servo2.value() + 190)
        arduino.write(data.encode())

    def startThread(self):
        arduino.write("5000".encode())
        self.thread.start()
        self.thread.change_value.connect(self.setNum)
    def stopThread(self):
        self.thread.terminate()
    def setNum(self, val):
        self.ui.mesafeLabel.setNum(int(val[0]))
        self.ui.mesafeLabel2.setNum(int(val[1]))

    def otomatik(self):
        arduino.write("6000".encode())
        self.ui.stackedWidget.setCurrentIndex(0)

    def otomatikStart(self):
        self.thread.terminate()
        self.thread_2.start()
        self.thread_2.change_value_2.connect(self.otomatikValue)

    def otomatikValue(self, val):
        self.ui.mesafeLabelOto.setNum(int(val[0]))
        self.ui.mesafeLabelOto2.setNum(int(val[1]))
        self.ui.servoLabel.setNum(int(val[2]))
    def manuel(self):
        self.thread_2.terminate()
        self.ui.stackedWidget.setCurrentIndex(1)
        arduino.write("5000".encode())

    def sendValue(self):
        if self.ui.comboBox.currentText() == "Mesafe":
            arduino.write("1000".encode())
        elif self.ui.comboBox.currentText() == "Servo":
            arduino.write("2000".encode())





def control_win():
    app = QApplication(sys.argv)
    win = ui_windows()

    win.show()
    sys.exit(app.exec_())


control_win()
