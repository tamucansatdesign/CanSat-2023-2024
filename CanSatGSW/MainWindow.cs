using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using System.IO.Ports;
using System.Collections.Generic;
using System.IO;

namespace CanSat_GSW
{
    public partial class MainWindow : Form
    {
        [DllImport("DwmApi")]
        private static extern int DwmSetWindowAttribute(IntPtr hwnd, int attr, int[] attrValue, int attrSize);

        protected override void OnHandleCreated(EventArgs e)
        {
            if (DwmSetWindowAttribute(Handle, 19, new[] { 1 }, 4) != 0)
                DwmSetWindowAttribute(Handle, 20, new[] { 1 }, 4);
        }

        List<string> allPackets;
        Random r;

        string MISSION_TIME = "00:00:00.00";
        int PACKET_COUNT = 0;
        string MODE = "F";
        string TP_RELEASED = "N";
        string GPS_TIME = "00:00:00.00";
        string CONTAINER_STATE = "INITIALZATION";
        string CMD_ECHO = "None";
        string PAYLOAD_STATE = "INITIALZATION";

        public MainWindow()
        {
            InitializeComponent();

            r = new Random();
            altGraph.setTitle("Altitude");
            posGraph.setTitle("GPS Position");
            satsGraph.setTitle("GPS Sats");
            tempGraph.setTitle("Temperature");
            vbatGraph.setTitle("Battery Voltage");
            gyroGraph.setTitle("Payload Gyroscope");
            accelGraph.setTitle("Payload Acceleration");
            magGraph.setTitle("Payload Magnetometer");
            pointingGraph.setTitle("Payload Pointing Error");

            altGraph.AddChart("UTC Time", "Height", "GPS", SeriesChartType.FastLine);
            altGraph.AddLineToChart("chart0", "Container", SeriesChartType.FastLine);
            altGraph.AddLineToChart("chart0", "Payload", SeriesChartType.FastLine);

            posGraph.AddChart("Degrees", "Degrees", "GPS Position", SeriesChartType.FastLine);

            satsGraph.AddChart("UTC Time", "Number of Satellites", "GPS Sats", SeriesChartType.FastLine);

            tempGraph.AddChart("UTC Time", "Celsuis", "Container", SeriesChartType.FastLine);
            tempGraph.AddLineToChart("chart0", "Payload", SeriesChartType.FastLine);

            vbatGraph.AddChart("UTC Time", "Voltage", "Container", SeriesChartType.FastLine);
            vbatGraph.AddLineToChart("chart0", "Payload", SeriesChartType.FastLine);

            gyroGraph.AddChart("UTC Time", "Rad / s", "X", SeriesChartType.FastLine);
            gyroGraph.AddLineToChart("chart0", "Y", SeriesChartType.FastLine);
            gyroGraph.AddLineToChart("chart0", "Z", SeriesChartType.FastLine);

            accelGraph.AddChart("UTC Time", "m / s^2", "X", SeriesChartType.FastLine);
            accelGraph.AddLineToChart("chart0", "Y", SeriesChartType.FastLine);
            accelGraph.AddLineToChart("chart0", "Z", SeriesChartType.FastLine);

            magGraph.AddChart("UTC Time", "Tesla", "X", SeriesChartType.FastLine);
            magGraph.AddLineToChart("chart0", "Y", SeriesChartType.FastLine);
            magGraph.AddLineToChart("chart0", "Z", SeriesChartType.FastLine);

            pointingGraph.AddChart("UTC Time", "Degrees", "Error", SeriesChartType.FastLine);

            allPackets = new List<string>();
        }

        private bool updatePorts()
        {
            string[] ports = SerialPort.GetPortNames();
            for (int i = 0; i < SerialPort.GetPortNames().Length; i++)
            {
                if (!serialPorts.Items.Contains(ports[i]))
                    return true;
            }
            return false;
        }

        private void checkRadio(object sender, EventArgs e)
        {
            if (!serialPort.IsOpen)
            {
                if (updatePorts())
                {
                    serialPorts.Items.Clear();

                    foreach (string p in SerialPort.GetPortNames())
                    {
                        serialPorts.Items.Add(p);
                    }
                }
                return;
            }

            try
            {
                //serialPort.BytesToRead > 0
                //time % 500 == 0
                while (serialPort.BytesToRead > 0)
                {
                    string packet = serialPort.ReadLine();

                    Console.WriteLine(packet);

                    allPackets.Add(packet);

                    string[] items = packet.Split(',');

                    AddNewData(packet, items);
                    //AddRandomData();
                }
            }
            catch (Exception) { }
        }

        private void serialConnect_Click(object sender, EventArgs e)
        {
            if (!serialPort.IsOpen)
            {
                serialPort.PortName = serialPorts.SelectedItem.ToString();
                serialPort.Open();
                serialConnect.Text = "Disconnect";

                allPackets.Clear();
                altGraph.Clear();
                posGraph.Clear();
                satsGraph.Clear();
                tempGraph.Clear();
                vbatGraph.Clear();
                gyroGraph.Clear();
                accelGraph.Clear();
                magGraph.Clear();
                pointingGraph.Clear();
                statusBox.Clear();
                telemetryBox.Clear();
            }
            else
            {
                serialPort.Close();
                serialConnect.Text = "Connect";
            }
        }

        private void AddNewData(string packet, string[] data)
        {
            try
            {
                int TEAM_ID = Convert.ToInt32(data[0]);
                MISSION_TIME = data[1];
                PACKET_COUNT = Convert.ToInt32(data[2]);
                string PACKET_TYPE = data[3];

                if (TEAM_ID != 1051 && TEAM_ID != 6051) return;

                telemetryBox.AppendText(packet + "\r\n");

                if (PACKET_TYPE == "C")
                {
                    MODE = data[4];
                    TP_RELEASED = data[5];
                    altGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[6]), "Container");
                    tempGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[7]), "Container");
                    vbatGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[8]), "Container");
                    GPS_TIME = data[9];
                    Console.WriteLine(Convert.ToDouble(data[10]));
                    Console.WriteLine(Convert.ToDouble(data[11]));
                    posGraph.AddPoint(Convert.ToDouble(data[10]), Convert.ToDouble(data[11]), "GPS Position");
                    altGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[12]), "GPS");
                    satsGraph.AddPoint(MISSION_TIME, Convert.ToInt32(data[13]), "GPS Sats");
                    CONTAINER_STATE = data[14];
                    CMD_ECHO = data[15];
                }
                else if (PACKET_TYPE == "T")
                {
                    altGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[4]), "Payload");
                    tempGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[5]), "Payload");
                    vbatGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[6]), "Payload");
                    gyroGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[7]), "X");
                    gyroGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[8]), "Y");
                    gyroGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[9]), "Z");
                    accelGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[10]), "X");
                    accelGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[11]), "Y");
                    accelGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[12]), "Z");
                    magGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[13]), "X");
                    magGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[14]), "Y");
                    magGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[15]), "Z");
                    pointingGraph.AddPoint(MISSION_TIME, Convert.ToDouble(data[16]), "Error");
                    PAYLOAD_STATE = data[17];
                }

                statusBox.Clear();
                statusBox.AppendText("MISSION_TIME: " + MISSION_TIME + "\r\n");
                statusBox.AppendText("PACKET_COUNT: " + Convert.ToInt32(PACKET_COUNT) + "\r\n");
                statusBox.AppendText("MODE: " + MODE + "\r\n");
                statusBox.AppendText("TP_RELEASED: " + TP_RELEASED + "\r\n");
                statusBox.AppendText("GPS_TIME: " + GPS_TIME + "\r\n");
                statusBox.AppendText("CONTAINER_STATE: " + CONTAINER_STATE + "\r\n");
                statusBox.AppendText("CMD_ECHO: " + CMD_ECHO + "\r\n");
                statusBox.AppendText("PAYLOAD_STATE: " + PAYLOAD_STATE + "\r\n");
            }
            catch { }
        }

        /*private void AddRandomData()
        {
            try
            {
                altGraph.AddPoint(time, Convert.ToDouble(r.NextDouble()), "GPS");
                altGraph.AddPoint(time, Convert.ToDouble(r.NextDouble()), "Press");

                posGraph.AddPoint(time, Convert.ToDouble(r.NextDouble()), "Lat");
                posGraph.AddPoint(time, Convert.ToDouble(r.NextDouble()), "Long");

                gyroChart.AddPoint(time, Convert.ToDouble(r.NextDouble()), "X");
                gyroChart.AddPoint(time, Convert.ToDouble(r.NextDouble()), "Y");
                gyroChart.AddPoint(time, Convert.ToDouble(r.NextDouble()), "Z");

                accelGraph.AddPoint(time, Convert.ToDouble(r.NextDouble()), "X");
                accelGraph.AddPoint(time, Convert.ToDouble(r.NextDouble()), "Y");
                accelGraph.AddPoint(time, Convert.ToDouble(r.NextDouble()), "Z");

                tempGraph.AddPoint(time, Convert.ToDouble(r.NextDouble()), "Temp");

                vbatGraph.AddPoint(time, Convert.ToDouble(r.NextDouble()), "Vbat");
            }
            catch { }
        }*/

        private void saveButton_Click(object sender, EventArgs e)
        {
            File.WriteAllLines(outputName.Text + ".csv", allPackets);
        }

        private void MainWindow_FormClosed(object sender, FormClosedEventArgs e)
        {
            serialPort.Close();
        }

        private void MainWindow_Leave(object sender, EventArgs e)
        {
            serialPort.Close();
        }

        private void startTelemetry_Click(object sender, EventArgs e)
        {
            if (startTelemetry.Text == "Start Telemetry")
            {
                serialPort.WriteLine("0\r");
                startTelemetry.Text = "Stop Telemetry";
            }
            else
            {
                serialPort.WriteLine("1\r");
                startTelemetry.Text = "Start Telemetry";
            }
        }

        private void loadData(object sender, EventArgs e)
        {
            allPackets.Clear();
            altGraph.Clear();
            posGraph.Clear();
            satsGraph.Clear();
            tempGraph.Clear();
            vbatGraph.Clear();
            gyroGraph.Clear();
            accelGraph.Clear();
            magGraph.Clear();
            pointingGraph.Clear();
            statusBox.Clear();
            telemetryBox.Clear();

            string[] data = File.ReadAllLines(outputName.Text + ".csv");

            foreach (string line in data)
            {
                AddNewData(line, line.Split(','));
            }
        }
    }
}
