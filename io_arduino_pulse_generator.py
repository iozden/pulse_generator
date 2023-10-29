
import Tkinter
from Tkinter import *
import serial
import time


#%%

root=Tkinter.Tk()
root.title("Pulse  Generator")
root.geometry('{}x{}'.format(600,300))

class my_class:
    def __init__(self, master):
        self.master = master

        self.frequency8=1;
        self.duration8=500;
        self.pulsedelay8=0
        self.trainduration8=0;
        self.traininterval8=0;
        self.numtrain8=0;

        self.frequency0=1;
        self.duration0=500;
        self.pulsedelay0=0
        self.trainduration0=0;
        self.traininterval0=0;
        self.numtrain0=0;			

        Label(master, text="timer 8").place(relx=0.05,rely=0.04, anchor="c")
        Label(master, text="freq").place(relx=0.1,rely=0.10, anchor="c")
        self.f8= Entry(master,text='freq8')
        self.f8.place(relx=.1, rely=.20, width=30, height=20, anchor="c")
        self.f8.insert(10,'10')

        Label(master, text="dur (ms)").place(relx=0.20,rely=0.1, anchor="c")
        self.dur8= Entry(master,text='dur8')
        self.dur8.place(relx=.20, rely=.2, width=40, height=20, anchor="c")
        self.dur8.insert(10,'10')

        Label(master, text="delay (ms)").place(relx=0.32,rely=0.1, anchor="c")
        self.delay8= Entry(master,text='delay8')
        self.delay8.place(relx=.32, rely=.2, width=40, height=20, anchor="c")
        self.delay8.insert(10,'0')

        Label(master, text="train dur (s)").place(relx=0.45,rely=0.1, anchor="c")
        self.train_dur8= Entry(master,text='train_dur8')
        self.train_dur8.place(relx=.45, rely=.2, width=40, height=20, anchor="c")
        self.train_dur8.insert(10,'0')

        Label(master, text="train int(s)").place(relx=0.58,rely=0.1, anchor="c")
        self.train_int8= Entry(master,text='train_int8')
        self.train_int8.place(relx=.58, rely=.2, width=40, height=20, anchor="c")
        self.train_int8.insert(10,'0')

        Label(master, text="n train").place(relx=0.7,rely=0.1, anchor="c")
        self.ntrain8= Entry(master,text='ntrain8')
        self.ntrain8.place(relx=.7, rely=.2, width=40, height=20, anchor="c")
        self.ntrain8.insert(10,'0')


        Label(master, text="timer 0").place(relx=0.05,rely=0.3, anchor="c")
        Label(master, text="freq").place(relx=0.1,rely=0.36, anchor="c")
        self.f0= Entry(master,text='freq0')
        self.f0.place(relx=.1, rely=.46, width=30, height=20, anchor="c")
        self.f0.insert(10,'1')

        Label(master, text="dur (ms)").place(relx=0.20,rely=0.36, anchor="c")
        self.dur0= Entry(master,text='dur0')
        self.dur0.place(relx=.20, rely=.46, width=40, height=20, anchor="c")
        self.dur0.insert(10,'10')

        Label(master, text="delay (ms)").place(relx=0.32,rely=0.36, anchor="c")
        self.delay0= Entry(master,text='delay0')
        self.delay0.place(relx=.32, rely=.46, width=40, height=20, anchor="c")
        self.delay0.insert(10,'0')

        Label(master, text="train dur (s)").place(relx=0.45,rely=0.36, anchor="c")
        self.train_dur0= Entry(master,text='train_dur0')
        self.train_dur0.place(relx=.45, rely=.46, width=40, height=20, anchor="c")
        self.train_dur0.insert(10,'0')

        Label(master, text="train int(s)").place(relx=0.58,rely=0.36, anchor="c")
        self.train_int0= Entry(master,text='train_int0')
        self.train_int0.place(relx=.58, rely=.46, width=40, height=20, anchor="c")
        self.train_int0.insert(10,'0')

        Label(master, text="n train").place(relx=0.7,rely=0.36, anchor="c")
        self.ntrain0= Entry(master,text='ntrain0')
        self.ntrain0.place(relx=.7, rely=.46, width=40, height=20, anchor="c")
        self.ntrain0.insert(10,'0')

        self.button_set = Button(master, text="Set", width=10, command=self.set)
        self.button_set.place(relx=.10, rely=.6, anchor="c")
				
        self.exttrig_flag = IntVar(value=0)
        self.exttrig = Checkbutton(master,text="ext trig",variable=self.exttrig_flag).place(relx=0.66,rely=0.57)				

        self.button_start = Button(master, text="Start", width=10, command=self.start)
        self.button_start.place(relx=.30, rely=.6, anchor="c")
        self.oc=self.button_start.cget("background")

        self.button_stop = Button(master, text="Stop", width=10, command=self.stop)
        self.button_stop.place(relx=.5, rely=.6, anchor="c")

        self.port_num= Entry(master,text='arduino port')
        self.port_num.place(relx=.55, rely=.9, width=60, height=20, anchor="c")
        self.port_num.insert(10,'COM11')

        self.button_quit = Button(master, text="QUIT", width=10, command=self.close)
        self.button_quit.place(relx=.75, rely=.9, anchor="c")

        global ser
        nPort=self.port_num.get()
        ser=serial.Serial(port=nPort,baudrate=115200)


    def start(self):
        ser.write('1')
        self.button_start.config(text="Running",bg='green')
				
    def stop(self):
        ser.write('0')
        self.button_start.config(text="Start",bg=self.oc)
        time.sleep(0.1)
				

    def set(self):

        self.frequency8=self.f8.get()
        self.duration8=self.dur8.get()
        self.pulsedelay8=self.delay8.get()
        self.trainduration8=self.train_dur8.get()
        self.traininterval8=self.train_int8.get()
        self.numtrain8=self.ntrain8.get()

        self.frequency0=self.f0.get()
        self.duration0=self.dur0.get()
        self.pulsedelay0=self.delay0.get()
        self.trainduration0=self.train_dur0.get()
        self.traininterval0=self.train_int0.get()
        self.numtrain0=self.ntrain0.get()
				
        self.exttrig=str(self.exttrig_flag.get())				

        self.dataout='p'+self.frequency8+'s'+self.duration8+'s'+self.pulsedelay8+'s'+self.traininterval8+'s'+self.trainduration8+'s'+self.numtrain8+'s'+self.frequency0+'s'+self.duration0+'s'+self.pulsedelay0+'s'+self.traininterval0+'s'+self.trainduration0+'s'+self.numtrain0+'s'+self.exttrig +'e'+'\n'
        ser.write(self.dataout)		

    def close(self):
        ser.close()
        self.master.destroy()


mygui=my_class(root)
root.mainloop()

