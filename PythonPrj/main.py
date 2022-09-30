from tkinter import *
import serial as ser
import turtle
import time


#quit from the menu
def quit(tim):
    turtle.bye()
    root.destroy()

def clear_info(): #clear the info of the angles or steps that appears on screen
    clearinfo.grid_forget()
    counterlabel.grid_forget()
    anglelabel.grid_forget()
    anglelrigthabel.grid_forget()
    anglelleftabel.grid_forget()
    dinamicangle.grid_forget()
    acknowledge.grid_forget()

def clear(tim): #clear the draw screen
    tim.clear()

def firstTask(): #rotate the stepper motor through the joystick
    closeTabs()
    global state,bool_third
    bool_third = 0
    state = 1
    # clear buffers
    s.reset_input_buffer()
    s.reset_output_buffer()
    inChar = "1"
    bytesChar = bytes(inChar, 'ascii')
    s.write(bytesChar)
    startmovebutton.grid(row=8, column=1)
    pass

def startmove(): #start moving the stepper motor
    inChar = "5"
    bytesChar = bytes(inChar, 'ascii')
    s.write(bytesChar)
    startmovebutton.grid_forget()
    stopmovebutton.grid(row=8, column=1)

def stoptmove(): #stop moving the stepper motor
    b = 0
    inChar = "6"
    global bool_third,counterlabel,anglelabel
    bytesChar = bytes(inChar, 'ascii')
    s.write(bytesChar)
    stopmovebutton.grid_forget()
    if(bool_third == 1):
        bool_third = 2
        startmovebutton.grid(row=8, column=1)
    elif(bool_third == 2):
        s.reset_input_buffer()
        s.reset_output_buffer()
        while (b==0):
            string1 = ""
            while (s.in_waiting > 0):  # while the input buffer isn't empty
                char = s.read(size=1)  # read 1 char from the input buffer
                string1 = string1 + char.decode("ascii")
            time.sleep(0.05)
            a = string1.replace("\n","")
            if (a!= ""):
                b = int(a)
                a = str(int(a))
        counterlabel = Label(root,text="the counter is "+ a +" steps" ,padx=40,pady=20,bg="#7F7F7F",font=("helvetica", 12))
        anglelabel = Label(root, text="the angle is " + str(round(360.0/b,4)) + " degrees", padx=40, pady=20,bg="#7F7F7F", font=("helvetica", 12))
        clearinfo.grid(row=10, column=2, rowspan=2,columnspan=2)
        counterlabel.grid(row=10,column=1)
        anglelabel.grid(row=11, column=1)

def secondTask(string,tim,mode): #start the draw function through the joystick
    closeTabs()
    global state, angle
    angle = 0
    state = 2
    # clear buffers
    s.reset_input_buffer()
    s.reset_output_buffer()
    inChar = "2"
    bytesChar = bytes(inChar, 'ascii')
    s.write(bytesChar)
    time.sleep(0.05)
    while(state ==2):
        while (s.in_waiting > 0):  # while the input buffer isn't empty
            char = s.read(size = 1)  # read 1 char from the input buffer
            string = string + char.decode("ascii")
            root.update()
        st = string[0:3]
        time.sleep(0.08)
        if (st != ""):
            if ("\n" not in st):
                intst = int(st)
                if (intst == 500):
                    if(mode ==0):
                        mode = 1
                        tim.pencolor("white")
                        tim.pensize(30)
                        tim.shapesize(1.3)
                    elif(mode ==1):
                        mode = 2
                        tim.penup()
                        tim.shapesize(0.5)
                    elif(mode ==2):
                        mode = 0
                        tim.pensize(1.5)
                        tim.shapesize(0.5)
                        tim.pencolor("red")
                        tim.pendown()
                elif (intst != 0):
                    tim.right(angle-(270-intst))
                    tim.forward(4)
                    angle = 270 -intst
        string = ""
        root.update()
    tim.right(angle)

def thirdTask(): #calibrate the stepper motor and print the number of steps for one lap and the angle of one step
    closeTabs()
    global state,bool_third
    state = 3
    # clear buffers
    bool_third = 1
    s.reset_input_buffer()
    s.reset_output_buffer()
    inChar = "3"
    bytesChar = bytes(inChar, 'ascii')
    s.write(bytesChar)
    startmovebutton.grid(row=8, column=1)

def closeTabs(): #close all the open tabs / windows
    startmovebutton.grid_forget()
    stopmovebutton.grid_forget()
    script1button.grid_forget()
    script2button.grid_forget()
    script3button.grid_forget()
    clearinfo.grid_forget()
    counterlabel.grid_forget()
    anglelabel.grid_forget()
    anglelleftabel.grid_forget()
    anglelrigthabel.grid_forget()
    dinamicangle.grid_forget()
    acknowledge.grid_forget()

def fourthTask(): #start the script menu and load the script to the flash
    closeTabs()
    global state,firstTime
    state = 4
    # clear buffers
    s.reset_input_buffer()
    s.reset_output_buffer()
    inChar = "4"
    bytesChar = bytes(inChar, 'ascii')
    s.write(bytesChar)

    if (firstTime == 1):
        firstTime = 0
        with open('Script1.txt') as f:
            lines1 = f.read()
        with open('Script2.txt') as f:
            lines2 = f.read()
        with open('Script3.txt') as f:
            lines3 = f.read()
        lines1 = lines1 + "!"
        lines2 = lines2 + "!"
        lines3 = lines3 + "!"
        bytesChar = bytes(lines1, 'ascii')
        time.sleep(0.1)
        s.write(bytesChar)
        time.sleep(0.1)
        bytesChar = bytes(lines2, 'ascii')
        s.write(bytesChar)
        time.sleep(0.1)
        bytesChar = bytes(lines3, 'ascii')
        s.write(bytesChar)
        acknowledge.grid(row=12, column=1)

    script1button.grid(row=11, column=1)
    script2button.grid(row=11, column=2)
    script3button.grid(row=11, column=3,padx=20)

def readScript(scriptNum): #send the selected script to the Embedded system
    clear_info()
    root.update()
    global anglelabel,anglelleftabel,anglelrigthabel,first_arg,dinamicangle
    # clear buffers
    task = ""
    s.reset_input_buffer()
    s.reset_output_buffer()
    inChar = scriptNum
    bytesChar = bytes(inChar, 'ascii')
    s.write(bytesChar)
    s.reset_input_buffer()
    s.reset_output_buffer()
    while (task != "!"):
        string1 = ""
        kelet = 0
        while (kelet ==0):
            while (s.in_waiting > 0):  # while the input buffer isn't empty
                char = s.read(size=1)  # read 1 char from the input buffer
                string1 = string1 + char.decode("ascii")
                time.sleep(0.02)
                kelet = 1
        time.sleep(0.05)
        a = string1.replace("\n", "")
        task = a[0]
        if(task == "6"):
            b = str(int(a[1:4])) #cut the zeros
            anglelabel.grid_forget()
            anglelabel = Label(root, text="the angle after task 6 is " + b, padx=40, pady=20,bg="#7F7F7F", font=("helvetica", 12))
            dinamicangle.grid_forget()
            dinamicangle = Label(root, text="the dynamic angle " + b, padx=40, pady=20, bg="#7F7F7F",font=("helvetica", 12))
            dinamicangle.grid(row=13, column=2)
            anglelabel.grid(row=13, column=1)
            root.update()
            time.sleep(0.5)
        if(task =="7"):
            if (first_arg==1):
                b = str(int(a[1:4]))  # cut the zeros
                anglelleftabel.grid_forget()
                anglelleftabel = Label(root, text="the left angle is " + b, padx=40, pady=20,bg="#7F7F7F", font=("helvetica", 12))
                dinamicangle.grid_forget()
                dinamicangle = Label(root, text="the dynamic angle " + b, padx=40, pady=20, bg="#7F7F7F",font=("helvetica", 12))
                dinamicangle.grid(row=13, column=2)
                anglelleftabel.grid(row=14, column=1)
                root.update()
                first_arg=0
            else:
                a = string1.replace("\n", "")
                b2 = str(int(a[1:4]))  # cut the zeros
                anglelrigthabel.grid_forget()
                anglelrigthabel = Label(root, text="the rigth angle is  " + b2, padx=40, pady=20,bg="#7F7F7F", font=("helvetica", 12))
                dinamicangle.grid_forget()
                dinamicangle = Label(root, text="the dynamic angle " + b2, padx=40, pady=20, bg="#7F7F7F",font=("helvetica", 12))
                dinamicangle.grid(row=13, column=2)
                anglelrigthabel.grid(row=15, column=1)
                root.update()
                first_arg = 1

    clearinfo.grid(row=14, column=2, rowspan=2,columnspan=2)

root = Tk()
root.config(bg="#7F7F7F")
tim = turtle.Turtle()
tim.color("red")
tim.pensize(1.5)
tim.shapesize(0.5)
tim.shape("circle")
tim.speed(0)


angle = 0 #angle delta
mode = 0 #paint mode
bool_third = 0
state = 0
string = ""
firstTime = 1
first_arg = 1

s = ser.Serial('COM3', baudrate=9600, bytesize=ser.EIGHTBITS,
               parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
               timeout=1)  # timeout of 1 sec where the read and write operations are blocking,

menulabel = Label(root,bg="#7F7F7F",text="Project Menu", font=("helvetica", 30)).grid(row=1, column=1,columnspan=4,pady= 20)
counterlabel = Label(root)
anglelabel = Label(root)
anglelleftabel = Label(root)
anglelrigthabel = Label(root)
dinamicangle = Label(root)
acknowledge = Label(root, text="finished sending the scripts" , padx=40, pady=20, bg="#7F7F7F",font=("helvetica", 12))

firstbutton = Button(root,padx=20,pady=10, text="First task", command=firstTask, font=("helvetica", 20),background="#1E90FF")
firstbutton.grid(padx=30,pady=20,row=2, column=1)
secondtbutton = Button(root, padx=13,pady=10,text="Second task", command=lambda :secondTask(string,tim,mode), font=("helvetica", 20), background="#1E90FF")
secondtbutton.grid(row=2, column=2, columnspan=2, padx=70)
thirdtbutton = Button(root, padx=17,pady=10,text="Third task", command=thirdTask, font=("helvetica", 20),background="#1E90FF")
thirdtbutton.grid(padx=10,row=3, column=1,pady=30)
fourthbutton = Button(root,padx=20,pady=10,text="Fourth task", command=fourthTask, font=("helvetica", 20), background="#1E90FF")
fourthbutton.grid(row=3, column=2,padx=4,columnspan=2)

startmovebutton = Button(root, padx=20,pady=10,text="start rotate",command = startmove,font=("helvetica", 20), background="#6A5ACD")
stopmovebutton = Button(root, padx=20,pady=10,text="stop rotate",command = stoptmove,font=("helvetica", 20), background="#6A5ACD")

exitbutton = Button(root,text = "Quit", padx=20,pady=10,font=("helvetica", 20),fg="#D9D9D9",background="#030303",command=lambda:quit(tim)).grid(row=30, column=1,pady=20)
clearbutton = Button(root,text = "Clear\ndraw", padx=20,pady=10,font=("helvetica", 18),bg="#EBEBEB",command=lambda:clear(tim)).grid(row=30, column=2, columnspan=2)
clearinfo = Button(root, text = "clear info",padx=20,pady=10,font=("helvetica", 20),command=clear_info, background="#8F8F8F")

script1button = Button(root,text = "script 1", padx=20,pady=10,font=("helvetica", 20), background="#6A5ACD",command=lambda:readScript("A"))
script2button = Button(root,text = "script 2", padx=20,pady=10,font=("helvetica", 20), background="#6A5ACD",command=lambda:readScript("B"))
script3button = Button(root,text = "script 3", padx=20,pady=10,font=("helvetica", 20), background="#6A5ACD",command=lambda:readScript("C"))

root.mainloop()





