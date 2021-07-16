using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms; 
using System.IO.Ports;
using System.Threading;

namespace remoteIRArduino
{
    public partial class Form1 : Form
    {
        static SerialPort arduinoSerialPort;
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            arduinoSerialPort.Write("0");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            arduinoSerialPort.Write("1");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            arduinoSerialPort.Write("2");
        }

        private void button4_Click(object sender, EventArgs e)
        {
            arduinoSerialPort.Write("3");
        }

        private void button5_Click(object sender, EventArgs e)
        {
            arduinoSerialPort.Write("4");
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            arduinoSerialPort = new SerialPort();

            // Allow the user to set the appropriate properties.
            arduinoSerialPort.PortName = "com11";
            arduinoSerialPort.BaudRate = 9600;
            arduinoSerialPort.Open();
        }

    }
}
