using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.IO.Ports;
using System.IO;



namespace ArdupilotSim
{
    public partial class ArdupilotSim : Form
    {
        SerialPort comPort = new SerialPort();
        UdpClient XplanesSEND;
        Socket SimulatorRECV;
        TcpClient FlightGearSEND;
        SocketAsyncEventArgs recvargs;
        volatile byte[] udpdata = new byte[113 * 9 + 5]; // 113 types - 9 items per type (index+8) + 5 byte header
        volatile float[][] DATA = new float[113][];
        DateTime now = DateTime.Now;
        DateTime lastgpsupdate = DateTime.Now;
        List<string> position = new List<string>();
        int AAAlength = 17 + 3 + 2; // 3 header + 17 data + 2 cr and lf
        int REV_pitch = 1;
        int REV_roll = 1;
        int REV_rudder = 1;
        int GPS_rate = 200;
        int fgsent = 0;
        int lastmode = 0;
        string logdata = "";

        public ArdupilotSim()
        {
            InitializeComponent();

            Control.CheckForIllegalCrossThreadCalls = false;

            recvargs = new SocketAsyncEventArgs();
            recvargs.Completed += new EventHandler<SocketAsyncEventArgs>(Receive);

        }

        private void ArdupilotSim_Load(object sender, EventArgs e)
        {
            Comports.DataSource = SerialPort.GetPortNames();
            GPSrate.SelectedValue = "200";
        }

        private void ConnectComPort_Click(object sender, EventArgs e)
        {
            if (!comPort.IsOpen)
            {
                comPort.BaudRate = 38400;
                comPort.DataBits = 8;
                comPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), "1");
                comPort.Parity = (Parity)Enum.Parse(typeof(Parity), "None");
                comPort.PortName = Comports.SelectedValue.ToString();

                try
                {
                    comPort.Open();
                    OutputLog.AppendText("Opened com port\r\n");
                    comPort.ReceivedBytesThreshold = 4;
                    comPort.DataReceived += new SerialDataReceivedEventHandler(ReceiveSerial);
                }
                catch (Exception) { OutputLog.AppendText("Cant open serial port\r\n"); }

                SetupUDPRecv();

                if (RAD_softXplanes.Checked)
                {
                    SetupUDPXplanes();
                }
                else
                {
                    SetupTcpFlightGear();
                }
            }
            else
            {
                comPort.Close();
                OutputLog.AppendText("Closed com port\r\n");
                SimulatorRECV.Close();

                if (RAD_softXplanes.Checked)
                {
                    XplanesSEND.Close();
                }
                else
                {
                    FlightGearSEND.Close();
                }
            }
        }

        private void SetupUDPRecv()
        {
            // setup receiver
            IPEndPoint ipep = new IPEndPoint(IPAddress.Any, 49005);

            SimulatorRECV = new Socket(AddressFamily.InterNetwork,
                            SocketType.Dgram, ProtocolType.Udp);

            SimulatorRECV.Bind(ipep);

            IPEndPoint sender = new IPEndPoint(IPAddress.Any, 0);

            recvargs.RemoteEndPoint = sender;
            recvargs.SetBuffer(udpdata, 0, udpdata.Length);

            SimulatorRECV.ReceiveFromAsync(recvargs);
        }

        private void SetupUDPXplanes()
        {
            // setup sender
            XplanesSEND = new UdpClient("127.0.0.1", 49000);
        }

        private void SetupTcpFlightGear()
        {
            FlightGearSEND = new TcpClient("127.0.0.1", 49000);
        }

        private void Receive(object sender, SocketAsyncEventArgs e)
        {
            if (e.BytesTransferred > 0)
            {
                IPEndPoint ep = (IPEndPoint)e.RemoteEndPoint;

                RECVprocess(e.Buffer, e.BytesTransferred, comPort);

                e.RemoteEndPoint = ep;
                e.SetBuffer(0, e.Buffer.Length);

                if (SimulatorRECV != null && SimulatorRECV.IsBound)
                    SimulatorRECV.ReceiveFromAsync(e);

            }
        }

        private void ReceiveSerial(object sender, SerialDataReceivedEventArgs e)
        {
            byte[] buffer = readline(comPort);

            if (buffer[0] == 'A' && buffer[1] == 'A' && buffer[2] == 'A')
            {
                processArduPilot(buffer);
            }
            else
            {
                logdata = Encoding.ASCII.GetString(buffer, 0, buffer.Length);
                this.OutputLog.AppendText(logdata);
            }
        }

        private byte[] readline(SerialPort comport)
        {
            byte[] temp = new byte[101];
            int count = 0;
            int step = 0;

            while (true)
            {
                temp[count] = (byte)comport.ReadByte();

                if (temp[0] == 'A' && temp[1] == 'A' && temp[2] == 'A' || step >= 1) // start of data
                {
                    step++;
                }

                if (temp[count] == '\n' && step == 0) // normal line
                {
                    break;
                }

                if (temp[count] == '\n' && step == AAAlength - 1) // dont finish until we have all data
                {
                    break;
                }

                count++;
                if (count == 100)
                    break;
            }

            byte[] temp2 = new byte[count + 1];

            Array.Copy(temp, 0, temp2, 0, count + 1);

            return temp2;
        }

        private void RECVprocess(byte[] data, int receviedbytes, SerialPort comPort)
        {
            if (data[0] == 'D' && data[1] == 'A')
            {
                // Xplanes sends
                // 5 byte header
                // 1 int for the index - numbers on left of output
                // 8 floats - might be useful. or 0 if not
                int count = 5;
                while (count < receviedbytes)
                {
                    int index = BitConverter.ToInt32(data, count);

                    DATA[index] = new float[8];

                    DATA[index][0] = BitConverter.ToSingle(data, count + 1 * 4); ;
                    DATA[index][1] = BitConverter.ToSingle(data, count + 2 * 4); ;
                    DATA[index][2] = BitConverter.ToSingle(data, count + 3 * 4); ;
                    DATA[index][3] = BitConverter.ToSingle(data, count + 4 * 4); ;
                    DATA[index][4] = BitConverter.ToSingle(data, count + 5 * 4); ;
                    DATA[index][5] = BitConverter.ToSingle(data, count + 6 * 4); ;
                    DATA[index][6] = BitConverter.ToSingle(data, count + 7 * 4); ;
                    DATA[index][7] = BitConverter.ToSingle(data, count + 8 * 4); ;

                    count += 36;
                }
            }
            else
            {
                //FlightGear

                DATA[20] = new float[8];

                DATA[18] = new float[8];

                DATA[3] = new float[8];

                string telem = Encoding.ASCII.GetString(data, 0, data.Length);

                try
                {

                    int oldpos = 0;
                    int pos = telem.IndexOf(",");
                    DATA[20][0] = float.Parse(telem.Substring(oldpos, pos - 1));

                    oldpos = pos;
                    pos = telem.IndexOf(",", pos + 1);
                    DATA[20][1] = float.Parse(telem.Substring(oldpos + 1, pos - 1 - oldpos));

                    oldpos = pos;
                    pos = telem.IndexOf(",", pos + 1);
                    DATA[20][2] = float.Parse(telem.Substring(oldpos + 1, pos - 1 - oldpos));

                    oldpos = pos;
                    pos = telem.IndexOf(",", pos + 1);
                    DATA[18][1] = float.Parse(telem.Substring(oldpos + 1, pos - 1 - oldpos));

                    oldpos = pos;
                    pos = telem.IndexOf(",", pos + 1);
                    DATA[18][0] = float.Parse(telem.Substring(oldpos + 1, pos - 1 - oldpos));

                    oldpos = pos;
                    pos = telem.IndexOf(",", pos + 1);
                    DATA[18][2] = float.Parse(telem.Substring(oldpos + 1, pos - 1 - oldpos));

                    oldpos = pos;
                    pos = telem.IndexOf("\n", pos + 1);
                    DATA[3][6] = float.Parse(telem.Substring(oldpos + 1, pos - 1 - oldpos));
                    DATA[3][7] = DATA[3][6];
                }
                catch (Exception) { }
            }

            if (!comPort.IsOpen) { return; }

            // write arduimu to ardupilot



            //convert data	
            int lat = (int)(DATA[20][0] * 10000000);
            int lng = (int)(DATA[20][1] * 10000000);
            int altitude = (int)(DATA[20][2] * 3.048);		// altitude to meters
            int speed = (int)(DATA[3][7] * 044.704);		// spped to m/s * 100
            int airspeed = (int)(DATA[3][6] * 044.704);		// speed to m/s * 100
            int pitch = (int)(DATA[18][0] * 100);
            int roll = (int)(DATA[18][1] * 100);
            int heading = (int)(DATA[18][2] * 100);

            byte[] IMU = new byte[32];

            IMU[0] = (byte)'D';
            IMU[1] = (byte)'I';
            IMU[2] = (byte)'Y';
            IMU[3] = (byte)'d';
            IMU[4] = 8;
            IMU[5] = 4;

            Array.Copy(BitConverter.GetBytes((Int16)roll), 0, IMU, 6, 2);
            Array.Copy(BitConverter.GetBytes((Int16)pitch), 0, IMU, 8, 2);
            Array.Copy(BitConverter.GetBytes((Int16)heading), 0, IMU, 10, 2);
            Array.Copy(BitConverter.GetBytes((Int16)airspeed), 0, IMU, 12, 2);

            byte check_a = 0;
            byte check_b = 0;

            for (int i = 4; i <= 13; i++)
            {
                check_a += IMU[i];
                check_b += check_a;
            }


            IMU[14] = check_a;
            IMU[15] = check_b;

            comPort.Write(IMU, 0, 16);

            TimeSpan gpsspan = DateTime.Now - lastgpsupdate;

            if ((gpsspan.Seconds * 1000) + gpsspan.Milliseconds >= GPS_rate)
            {
                lastgpsupdate = DateTime.Now;

                position.Add(DATA[20][1] + "," + DATA[20][0] + "," + (DATA[20][2] * .3048) + "\r\n");

                for (int i = 0; i < IMU.Length; i++)
                {
                    IMU[i] = 0;
                }

                IMU[0] = (byte)'D';
                IMU[1] = (byte)'I';
                IMU[2] = (byte)'Y';
                IMU[3] = (byte)'d';
                IMU[4] = 14;
                IMU[5] = 3;

                Array.Copy(BitConverter.GetBytes(lng), 0, IMU, 6, 4);
                Array.Copy(BitConverter.GetBytes(lat), 0, IMU, 10, 4);
                Array.Copy(BitConverter.GetBytes((Int16)altitude), 0, IMU, 14, 2);
                Array.Copy(BitConverter.GetBytes((Int16)speed), 0, IMU, 16, 2);
                Array.Copy(BitConverter.GetBytes((Int16)heading), 0, IMU, 18, 2);

                check_a = 0;
                check_b = 0;

                for (int i = 4; i <= 19; i++)
                {
                    check_a += IMU[i];
                    check_b += check_a;
                }

                IMU[20] = check_a;
                IMU[21] = check_b;

                comPort.Write(IMU, 0, 22);

                // write gps packet to file
                //FileStream stream = File.OpenWrite("coords.txt");
                //stream.Write(IMU, 0, 22);
                //stream.Close();
            }
        }

        private void processArduPilot(byte[] line)
        {
            if (line.Length < AAAlength - 1)
                return;


            float roll_out = (float)(Int16)(line[3] + (line[4] << 8)) / 4500;
            float pitch_out = (float)(Int16)(line[5] + (line[6] << 8)) / 4500;
            float throttle_out = (float)((Int16)(line[7] + (line[8] << 8))) / 100;
            float rudder_out = (float)((Int16)(line[9] + (line[10] << 8))) / 4500;
            float wp_distance = (float)(Int16)(line[11] + (line[12] << 8));
            float bearing_error = (float)(Int16)(line[13] + (line[14] << 8)) / 100;
            float alt_error = (float)(Int16)(line[15] + (line[16] << 8)) / 100;
            float eng_error = (float)(Int16)(line[17] + (line[18] << 8)) / 100;
            byte wp_index = line[19];
            byte control_mode = line[20];

            if (roll_out > 1)
            {
                roll_out = 1;
            }
            else if (roll_out < -1)
            {
                roll_out = -1;
            }

            if (pitch_out > 1)
            {
                pitch_out = 1;
            }
            else if (pitch_out < -1)
            {
                pitch_out = -1;
            }

            if (rudder_out > 1)
            {
                rudder_out = 1;
            }
            else if (rudder_out < -1)
            {
                rudder_out = -1;
            }



            if (lastmode != control_mode)
            {
                Console.WriteLine("\nMODE CHANGE - writeing KML");

                FileStream stream = File.Open("flight" + lastmode + ".kml", FileMode.Create);

                System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();

                string header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><kml xmlns=\"http://www.opengis.net/kml/2.2\">\n  <Document>    <name>Paths</name>    <description>Path</description>\n    <Style id=\"yellowLineGreenPoly\">      <LineStyle>        <color>7f00ffff</color>        <width>4</width>      </LineStyle>      <PolyStyle>        <color>7f00ff00</color>      </PolyStyle>    </Style>\n    <Placemark>      <name>Absolute Extruded</name>      <description>Transparent green wall with yellow outlines</description>      <styleUrl>#yellowLineGreenPoly</styleUrl>      <LineString>        <extrude>1</extrude>        <tessellate>1</tessellate>        <altitudeMode>absolute</altitudeMode>        <coordinates>";

                string footer = "      </coordinates>      </LineString>    </Placemark>     <Folder>      <name>Alerts</name>        </Folder>      <Folder>            <name>Mode</name>                 </Folder>      </Document></kml>";

                byte[] temp = encoding.GetBytes(header);

                stream.Write(temp, 0, temp.Length);

                foreach (string pos in position.ToArray())
                {

                    byte[] posbytes = encoding.GetBytes(pos);

                    stream.Write(posbytes, 0, posbytes.Length);
                }

                temp = encoding.GetBytes(footer);

                stream.Write(temp, 0, temp.Length);

                stream.Close();

                position.Clear();

                lastmode = control_mode;
            }



           // Console.Write(string.Format("R {0,6} P {1,6} Rud {2,6} T {3,6} DST {4,5} BE {5,5} AE {6,5} WP {7} M {8} \r", roll_out.ToString("0.000"), pitch_out.ToString("0.000"), rudder_out.ToString("0.000"), throttle_out.ToString("0.000"), wp_distance, bearing_error, alt_error, wp_index, control_mode));
            try
            {
                TXT_lat.Text = DATA[20][0].ToString();
                TXT_long.Text = DATA[20][1].ToString();
                TXT_alt.Text = DATA[20][2].ToString();

                TXT_roll.Text = DATA[18][1].ToString("0.000");
                TXT_pitch.Text = DATA[18][0].ToString("0.000");
                TXT_heading.Text = DATA[18][2].ToString("0.000");

                TXT_wpdist.Text = wp_distance.ToString();
                TXT_alterror.Text = alt_error.ToString();

                TXT_servoroll.Text = roll_out.ToString("0.000");
                TXT_servopitch.Text = pitch_out.ToString("0.000");
                TXT_servorudder.Text = rudder_out.ToString("0.000");
                TXT_servothrottle.Text = throttle_out.ToString("0.000");

            }
            catch (Exception) { }

            // Flightgear

            if (RAD_softFlightGear.Checked)
            {
                fgsent++;

                if (fgsent % 10 == 0) { return; } // short supply buffer.. seems to reduce lag

                string send = "3," + (roll_out * REV_roll) + "," + (pitch_out * REV_pitch * -1) + "," + (rudder_out * REV_rudder) + "," + (throttle_out) + "\r\n";

                byte[] FlightGear = new System.Text.ASCIIEncoding().GetBytes(send);

                try
                {
                    FlightGearSEND.GetStream().Write(FlightGear, 0, FlightGear.Length);
                    //XplanesSEND.Send(FlightGear, FlightGear.Length);  // if FG udp worked......
                }
                catch (Exception) { Console.WriteLine("Socket Write failed, FG closed?"); RAD_softXplanes.Checked = true; }

            }

            // Xplanes

            if (RAD_softXplanes.Checked)
            {
                // sending only 1 packet instead of many.

                byte[] Xplane = new byte[5 + 36 + 36];

                Xplane[0] = (byte)'D';
                Xplane[1] = (byte)'A';
                Xplane[2] = (byte)'T';
                Xplane[3] = (byte)'A';
                Xplane[4] = 0;

                Array.Copy(BitConverter.GetBytes((int)25), 0, Xplane, 5, 4); // packet index

                Array.Copy(BitConverter.GetBytes((float)throttle_out), 0, Xplane, 9, 4); // start data
                Array.Copy(BitConverter.GetBytes((float)throttle_out), 0, Xplane, 13, 4);
                Array.Copy(BitConverter.GetBytes((float)throttle_out), 0, Xplane, 17, 4);
                Array.Copy(BitConverter.GetBytes((float)throttle_out), 0, Xplane, 21, 4);

                Array.Copy(BitConverter.GetBytes((int)-999), 0, Xplane, 25, 4);
                Array.Copy(BitConverter.GetBytes((int)-999), 0, Xplane, 29, 4);
                Array.Copy(BitConverter.GetBytes((int)-999), 0, Xplane, 33, 4);
                Array.Copy(BitConverter.GetBytes((int)-999), 0, Xplane, 37, 4);

                // NEXT ONE - control surfaces

                Array.Copy(BitConverter.GetBytes((int)11), 0, Xplane, 41, 4); // packet index

                Array.Copy(BitConverter.GetBytes((float)(pitch_out * REV_pitch)), 0, Xplane, 45, 4); // start data
                Array.Copy(BitConverter.GetBytes((float)(roll_out * REV_roll)), 0, Xplane, 49, 4);
                Array.Copy(BitConverter.GetBytes((float)(rudder_out * REV_rudder)), 0, Xplane, 53, 4);
                Array.Copy(BitConverter.GetBytes((int)-999), 0, Xplane, 57, 4);

                Array.Copy(BitConverter.GetBytes((float)roll_out * REV_roll * 5), 0, Xplane, 61, 4);
                Array.Copy(BitConverter.GetBytes((int)-999), 0, Xplane, 65, 4);
                Array.Copy(BitConverter.GetBytes((int)-999), 0, Xplane, 69, 4);
                Array.Copy(BitConverter.GetBytes((int)-999), 0, Xplane, 73, 4);

                XplanesSEND.Send(Xplane, Xplane.Length);


                // NEXT ONE - joystick in..... no point
                /*
                Array.Copy(BitConverter.GetBytes((int)8), 0, Xplane, 5, 4); // packet index

                Array.Copy(BitConverter.GetBytes((float)pitch_out), 0, Xplane, 9, 4); // start data
                Array.Copy(BitConverter.GetBytes((float)roll_out), 0, Xplane, 13, 4); // start data
                Array.Copy(BitConverter.GetBytes((float)0), 0, Xplane, 17, 4); // start data
                Array.Copy(BitConverter.GetBytes((int)-999), 0, Xplane, 21, 4); // start data

                Array.Copy(BitConverter.GetBytes((int)-999), 0, Xplane, 25, 4); // start data
                Array.Copy(BitConverter.GetBytes((int)-999), 0, Xplane, 29, 4); // start data
                Array.Copy(BitConverter.GetBytes((int)-999), 0, Xplane, 33, 4); // start data
                Array.Copy(BitConverter.GetBytes((int)-999), 0, Xplane, 37, 4); // start data

                XplanesSEND.Send(Xplane, Xplane.Length);
                */

                // NEXT ONE - brakes
                /*
                if (gpscount == 9)
                {
                    Array.Copy(BitConverter.GetBytes((int)14), 0, Xplane, 5, 4); // packet index

                    Array.Copy(BitConverter.GetBytes((float)1), 0, Xplane, 9, 4); // start data
                    Array.Copy(BitConverter.GetBytes((float)0), 0, Xplane, 13, 4); // start data
                    Array.Copy(BitConverter.GetBytes((float)0), 0, Xplane, 17, 4); // start data
                    Array.Copy(BitConverter.GetBytes((float)0), 0, Xplane, 21, 4); // start data

                    Array.Copy(BitConverter.GetBytes((float)-999), 0, Xplane, 25, 4); // start data
                    Array.Copy(BitConverter.GetBytes((float)-999), 0, Xplane, 29, 4); // start data
                    Array.Copy(BitConverter.GetBytes((float)-999), 0, Xplane, 33, 4); // start data
                    Array.Copy(BitConverter.GetBytes((float)-999), 0, Xplane, 37, 4); // start data

                    XplanesSEND.Send(Xplane, Xplane.Length);
                }
                 */
            }
        }

        private void RAD_softXplanes_CheckedChanged(object sender, EventArgs e)
        {
            if (RAD_softXplanes.Checked)
            {
                RAD_softFlightGear.Checked = false;
            }
        }

        private void RAD_softFlightGear_CheckedChanged(object sender, EventArgs e)
        {
            if (RAD_softFlightGear.Checked)
            {
                RAD_softXplanes.Checked = false;
            }
        }

        private void CHKREV_roll_CheckedChanged(object sender, EventArgs e)
        {
            if (CHKREV_roll.Checked)
            {
                REV_roll = -1;
            }
            else
            {
                REV_roll = 1;
            }
        }

        private void CHKREV_pitch_CheckedChanged(object sender, EventArgs e)
        {
            if (CHKREV_pitch.Checked)
            {
                REV_pitch = -1;
            }
            else
            {
                REV_pitch = 1;
            }
        }

        private void CHKREV_rudder_CheckedChanged(object sender, EventArgs e)
        {
            if (CHKREV_rudder.Checked)
            {
                REV_rudder = -1;
            }
            else
            {
                REV_rudder = 1;
            }
        }

        private void GPSrate_SelectedIndexChanged(object sender, EventArgs e)
        {
            GPS_rate = int.Parse(GPSrate.SelectedItem.ToString());
        }

        private void OutputLog_TextChanged(object sender, EventArgs e)
        {
            OutputLog.SelectionStart = OutputLog.Text.Length;

            OutputLog.ScrollToCaret();

            OutputLog.Refresh();

        }
    }
}
