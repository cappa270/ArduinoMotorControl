using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.IO.Ports;
using System.IO;
using System.Xml; // config file
using System.Runtime.InteropServices; // dll imports
using System.Speech.Synthesis; // TTS

using ZedGraph; // Graphs
using AvionicsInstrumentControlDemo; // Displays
using EARTHLib; // Google earth

// Written by Michael Oborne

namespace ArdupilotSim
{
    public partial class ArdupilotSim : Form
    {
        SerialPort comPort = new SerialPort();
        UdpClient XplanesSEND;
        Socket SimulatorRECV;
        TcpClient FlightGearSEND;
        byte[] udpdata = new byte[113 * 9 + 5]; // 113 types - 9 items per type (index+8) + 5 byte header
        float[][] DATA = new float[113][];
        DateTime now = DateTime.Now;
        DateTime lastgpsupdate = DateTime.Now;
        List<string> position = new List<string>();
        int AAAlength = 17 + 3 + 2; // 3 header + 17 data + 2 cr and lf
        int REV_pitch = 1;
        int REV_roll = 1;
        int REV_rudder = 1;
        int GPS_rate = 200;
        bool displayfull = false;
        int packetssent = 0;
        string logdata = "";
        string[] modes = { "Manual", "Circle", "Stabalize", "", "", "FBW A", "FBW B", "", "", "", "Auto", "RTL", "Loiter", "Takeoff", "Land" };
        int tickStart = 0;
        static int threadrun = 1;

        SpeechSynthesizer talk = new SpeechSynthesizer();

        // for servo graph
        RollingPointPairList list = new RollingPointPairList(1200);
        RollingPointPairList list2 = new RollingPointPairList(1200);
        RollingPointPairList list3 = new RollingPointPairList(1200);
        RollingPointPairList list4 = new RollingPointPairList(1200);

        [DllImport("user32.dll")]
        private static extern int SetParent(
        int hWndChild,
        int hWndParent);

        [DllImport("user32.dll")]
        private static extern bool ShowWindowAsync(
        int hWnd,
        int nCmdShow);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool PostMessage(
        int hWnd,
        uint Msg,
        int wParam,
        int lParam);

        [DllImport("user32.dll", EntryPoint = "SetWindowPos")]
        private static extern bool SetWindowPos(
        int hWnd,
        int hWndInsertAfter,
        int X,
        int Y,
        int cx,
        int cy,
        uint uFlags);

        [DllImport("user32.dll")]
        private static extern int SendMessage(
        int hWnd,
        uint Msg,
        int wParam,
        int lParam);


        public ArdupilotSim()
        {
            InitializeComponent();

            Control.CheckForIllegalCrossThreadCalls = false; // so can update display from another thread

            // this was for async socket reads..... but was using more cpu than single threaded
            //recvargs = new SocketAsyncEventArgs();
            //recvargs.Completed += new EventHandler<SocketAsyncEventArgs>(Receive);

        }

        private EARTHLib.ApplicationGE ge = null;


        private void ArdupilotSim_Load(object sender, EventArgs e)
        {
            Comports.DataSource = SerialPort.GetPortNames();
            GPSrate.SelectedValue = "200";

            xmlconfig(false);

            talk.SpeakAsync("Hello.");


            if (displayfull)
            {
                //1076, 795
                this.Width = 1076;
                this.Height = 795;

                zg1.Visible = true;
                CreateChart(zg1);

                ge = new ApplicationGEClass();

                ShowWindowAsync(ge.GetMainHwnd(), 0);
                SetParent(ge.GetRenderHwnd(), Googleearthpanel.Handle.ToInt32());
                ResizeGoogleControl();
            }
            else
            {
                //651, 457
                this.Width = 651;
                this.Height = 457;

                timer1.Stop();
                zg1.Visible = false;

                airSpeedIndicatorInstrumentControl1.Visible = false;
                attitudeIndicatorInstrumentControl1.Visible = false;
                altimeterInstrumentControl1.Visible = false;
                headingIndicatorInstrumentControl1.Visible = false;
            }

        }

        private const int HWND_TOP = 0x0;
        private const int WM_COMMAND = 0x0112;
        private const int WM_QT_PAINT = 0xC2DC;
        private const int WM_PAINT = 0x000F;
        private const int WM_SIZE = 0x0005;
        private const int SWP_FRAMECHANGED = 0x0020;
        private const int WM_CLOSE = 0x0010;


        private void ResizeGoogleControl()
        {
            SendMessage(ge.GetMainHwnd(), WM_COMMAND, WM_PAINT, 0);
            PostMessage(ge.GetMainHwnd(), WM_QT_PAINT, 0, 0);

            SetWindowPos(
            ge.GetMainHwnd(),
            HWND_TOP,
            0,
            0,
            Googleearthpanel.Width,
            Googleearthpanel.Height, // height
            SWP_FRAMECHANGED);

            SendMessage(ge.GetRenderHwnd(), WM_COMMAND, WM_SIZE, 0);
        }

        private void ConnectComPort_Click(object sender, EventArgs e)
        {
            if (!comPort.IsOpen)
            {
                OutputLog.Clear();
                comPort.BaudRate = 38400;
                comPort.DataBits = 8;
                comPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), "1");
                comPort.Parity = (Parity)Enum.Parse(typeof(Parity), "None");

                try
                {
                    comPort.PortName = Comports.SelectedValue.ToString();
                    comPort.DtrEnable = true;
                    comPort.Open();
                    OutputLog.AppendText("Opened com port\r\n");
                }
                catch (Exception) { OutputLog.AppendText("Cant open serial port\r\n"); return; }

                SetupUDPRecv();

                if (RAD_softXplanes.Checked)
                {
                    SetupUDPXplanes();
                }
                else
                {
                    SetupTcpFlightGear();
                }

                System.Threading.Thread t11 = new System.Threading.Thread(delegate() { mainloop(); });
                t11.Start();
            }
            else
            {
                threadrun = 0;
                System.Threading.Thread.Sleep(100); // make sure thread closes
                comPort.Close();
                OutputLog.AppendText("Closed com port\r\n");
                SimulatorRECV.Close();
                position.Clear();

                if (RAD_softXplanes.Checked)
                {
                    XplanesSEND.Close();
                }
                else
                {
                    System.Threading.Thread.Sleep(100);
                    FlightGearSEND.Close();
                }
            }
        }

        private void xmlconfig(bool write)
        {
            if (write || !File.Exists("config.xml"))
            {
                XmlTextWriter xmlwriter = new XmlTextWriter("config.xml", Encoding.ASCII);
                xmlwriter.WriteStartDocument();

                xmlwriter.WriteStartElement("Config");

                xmlwriter.WriteElementString("comport", Comports.Text);

                xmlwriter.WriteElementString("REV_roll", CHKREV_roll.Checked.ToString());
                xmlwriter.WriteElementString("REV_pitch", CHKREV_pitch.Checked.ToString());
                xmlwriter.WriteElementString("REV_rudder", CHKREV_rudder.Checked.ToString());
                xmlwriter.WriteElementString("GPSrate", GPSrate.Text);
                xmlwriter.WriteElementString("Xplanes", RAD_softXplanes.Checked.ToString());

                xmlwriter.WriteElementString("rollgain", TXT_rollgain.Text);
                xmlwriter.WriteElementString("pitchgain", TXT_pitchgain.Text);
                xmlwriter.WriteElementString("ruddergain", TXT_ruddergain.Text);
                xmlwriter.WriteElementString("throttlegain", TXT_throttlegain.Text);

                xmlwriter.WriteElementString("CHKdisplayall", CHKdisplayall.Checked.ToString());

                xmlwriter.WriteEndElement();

                xmlwriter.WriteEndDocument();
                xmlwriter.Close();
            }
            else
            {
                using (XmlTextReader xmlreader = new XmlTextReader("config.xml"))
                {
                    while (xmlreader.Read())
                    {
                        xmlreader.MoveToElement();
                        switch (xmlreader.Name)
                        {
                            case "comport":
                                Comports.Text = xmlreader.ReadString();
                                break;
                            case "REV_roll":
                                CHKREV_roll.Checked = bool.Parse(xmlreader.ReadString());
                                break;
                            case "REV_pitch":
                                CHKREV_pitch.Checked = bool.Parse(xmlreader.ReadString());
                                break;
                            case "REV_rudder":
                                CHKREV_rudder.Checked = bool.Parse(xmlreader.ReadString());
                                break;
                            case "GPSrate":
                                GPSrate.Text = xmlreader.ReadString();
                                break;
                            case "Xplanes":
                                RAD_softXplanes.Checked = bool.Parse(xmlreader.ReadString());
                                break;
                            case "rollgain":
                                TXT_rollgain.Text = xmlreader.ReadString();
                                break;
                            case "pitchgain":
                                TXT_pitchgain.Text = xmlreader.ReadString();
                                break;
                            case "ruddergain":
                                TXT_ruddergain.Text = xmlreader.ReadString();
                                break;
                            case "throttlegain":
                                TXT_throttlegain.Text = xmlreader.ReadString();
                                break;
                            case "CHKdisplayall":
                                CHKdisplayall.Checked = bool.Parse(xmlreader.ReadString());
                                displayfull = CHKdisplayall.Checked;
                                break;
                            default:
                                break;
                        }
                    }
                }
            }



        }

        private void mainloop()
        {
            threadrun = 1;
            EndPoint Remote = (EndPoint)(new IPEndPoint(IPAddress.Any, 0));

            while (threadrun == 1)
            {
                if (SimulatorRECV.Available > 0)
                {
                    udpdata = new byte[udpdata.Length];

                    int recv = SimulatorRECV.ReceiveFrom(udpdata, ref Remote);

                    RECVprocess(udpdata, recv, comPort);
                }
                if (comPort.IsOpen == false) { break; }
                if (comPort.BytesToRead >= 4)
                {
                    byte[] buffer = readline(comPort);

                    if (threadrun == 0) { return; } // here because there could be a delay reading serial data

                    if (buffer[0] == 'A' && buffer[1] == 'A' && buffer[2] == 'A')
                    {
                        processArduPilot(buffer);
                    }
                    else
                    {
                        logdata = Encoding.ASCII.GetString(buffer, 0, buffer.Length);
                        OutputLog.AppendText(logdata);

                        if (logdata.Contains("MSG"))
                        {
                            if (logdata.Contains("CUR") || logdata.Contains("NWP") || logdata.Contains("wp #") || logdata.Contains("holding") || logdata.Contains("Radio in") || logdata.Contains("home:"))
                            {

                            }
                            else
                            {
                                logdata = logdata.Replace("WP", " waypoint ");
                                logdata = logdata.Replace("MSG", "");
                                talk.SpeakAsync(logdata);
                            }
                        }

                    }
                }
                System.Threading.Thread.Sleep(1); // try minimise delays                    
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

        private byte[] readline(SerialPort comport)
        {
            byte[] temp = new byte[101];
            int count = 0;
            int step = 0;

            while (true)
            {
                try
                {
                    temp[count] = (byte)comport.ReadByte();
                }
                catch (Exception) { }

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
                    // should convert this to regex.... or just leave it.
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
            IMU[4] = 8; // payload length
            IMU[5] = 4; // packet id

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
                IMU[4] = 14; // payload length
                IMU[5] = 3; // packet id

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


        private void writeKML()
        {
            FileStream stream = File.Open("flight.kml", FileMode.Create);

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

            //position.Clear();
        }

        private void processArduPilot(byte[] line)
        {
            if (line.Length < AAAlength - 1)
                return;

            // set defaults
            int rollgain = 6000;
            int pitchgain = 6000;
            int ruddergain = 6000;
            int throttlegain = 100;

            try
            {
                rollgain = int.Parse(TXT_rollgain.Text);
                pitchgain = int.Parse(TXT_pitchgain.Text);
                ruddergain = int.Parse(TXT_ruddergain.Text);
                throttlegain = int.Parse(TXT_throttlegain.Text);
            }
            catch (Exception) { OutputLog.AppendText("Bad Gains!!!\n"); }

            float roll_out = (float)(Int16)(line[3] + (line[4] << 8)) / rollgain;
            float pitch_out = (float)(Int16)(line[5] + (line[6] << 8)) / pitchgain;
            float throttle_out = (float)((Int16)(line[7] + (line[8] << 8))) / throttlegain;
            float rudder_out = (float)((Int16)(line[9] + (line[10] << 8))) / ruddergain;
            float wp_distance = (float)(Int16)(line[11] + (line[12] << 8));
            float bearing_error = (float)(Int16)(line[13] + (line[14] << 8)) / 100;
            float alt_error = (float)(Int16)(line[15] + (line[16] << 8)) / 100;
            float eng_error = (float)(Int16)(line[17] + (line[18] << 8)) / 100;
            byte wp_index = line[19];
            byte control_mode = line[20];

            roll_out = Constrain(roll_out, -1, 1);
            pitch_out = Constrain(pitch_out, -1, 1);
            rudder_out = Constrain(rudder_out, -1, 1);
            throttle_out = Constrain(throttle_out, -1, 1);

            if (packetssent % 150 == 0) // assuming 50hz
            {
                writeKML();
                if (displayfull)
                {
                    try
                    {
                        string path = Directory.GetCurrentDirectory() + "\\flight.kml";
                        ge.OpenKmlFile(path, -1);
                    }
                    catch (Exception) { Console.WriteLine("DAMN"); }
                }
            }

            // Console.Write(string.Format("R {0,6} P {1,6} Rud {2,6} T {3,6} DST {4,5} BE {5,5} AE {6,5} WP {7} M {8} \r", roll_out.ToString("0.000"), pitch_out.ToString("0.000"), rudder_out.ToString("0.000"), throttle_out.ToString("0.000"), wp_distance, bearing_error, alt_error, wp_index, control_mode));
            try
            {
                if (displayfull)
                {
                    double time = (Environment.TickCount - tickStart) / 1000.0;

                    if (CHKgraphroll.Checked)
                    {
                        list.Add(time, roll_out);
                    }
                    else { list.Clear(); }
                    if (CHKgraphpitch.Checked)
                    {
                        list2.Add(time, pitch_out);
                    }
                    else { list2.Clear(); }
                    if (CHKgraphrudder.Checked)
                    {
                        list3.Add(time, rudder_out);
                    }
                    else { list3.Clear(); }
                    if (CHKgraphthrottle.Checked)
                    {
                        list4.Add(time, throttle_out);
                    }
                    else { list4.Clear(); }
                }

                if (packetssent % 10 == 0) // reduce cpu usage
                {
                    TXT_servoroll.Text = roll_out.ToString("0.000");
                    TXT_servopitch.Text = pitch_out.ToString("0.000");
                    TXT_servorudder.Text = rudder_out.ToString("0.000");
                    TXT_servothrottle.Text = throttle_out.ToString("0.000");

                    TXT_bererror.Text = bearing_error.ToString("0.00");
                    TXT_wpdist.Text = wp_distance.ToString("0");
                    TXT_alterror.Text = alt_error.ToString("0.00");
                    TXT_WP.Text = wp_index.ToString();
                    TXT_control_mode.Text = modes[int.Parse(control_mode.ToString())];

                    TXT_lat.Text = DATA[20][0].ToString("0.00000");
                    TXT_long.Text = DATA[20][1].ToString("0.00000");
                    TXT_alt.Text = (DATA[20][2] * .3048).ToString("0.00");

                    TXT_roll.Text = DATA[18][1].ToString("0.000");
                    TXT_pitch.Text = DATA[18][0].ToString("0.000");
                    TXT_heading.Text = DATA[18][2].ToString("0.000");

                    if (displayfull)
                    {
                        attitudeIndicatorInstrumentControl1.SetAttitudeIndicatorParameters(DATA[18][0], (DATA[18][1] * -1));
                        headingIndicatorInstrumentControl1.SetHeadingIndicatorParameters((int)DATA[18][2]);
                        airSpeedIndicatorInstrumentControl1.SetAirSpeedIndicatorParameters((int)DATA[3][6]);
                        altimeterInstrumentControl1.SetAlimeterParameters((int)DATA[20][2]);
                    }

                }
            }
            catch (Exception) { }

            // Flightgear

            packetssent++;

            if (RAD_softFlightGear.Checked)
            {
                if (packetssent % 10 == 0) { return; } // short supply buffer.. seems to reduce lag

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

                try
                {

                    XplanesSEND.Send(Xplane, Xplane.Length);

                }
                catch (Exception) { }

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
            if (RAD_softXplanes.Checked && RAD_softFlightGear.Checked)
            {
                RAD_softFlightGear.Checked = false;
            }
        }

        private void RAD_softFlightGear_CheckedChanged(object sender, EventArgs e)
        {
            if (RAD_softFlightGear.Checked && RAD_softXplanes.Checked)
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
            // auto scroll
            OutputLog.SelectionStart = OutputLog.Text.Length;

            OutputLog.ScrollToCaret();

            OutputLog.Refresh();

        }

        private void Comports_MouseClick(object sender, MouseEventArgs e)
        {
            // update com port list if user start program before attaching ftdi
            Comports.DataSource = SerialPort.GetPortNames();
        }

        private float Constrain(float value, float min, float max)
        {
            if (value > max) { value = max; }
            if (value < min) { value = min; }
            return value;
        }


        public void CreateChart(ZedGraphControl zgc)
        {
            GraphPane myPane = zgc.GraphPane;

            // Set the titles and axis labels
            myPane.Title.Text = "Servo Output";
            myPane.XAxis.Title.Text = "Time";
            myPane.YAxis.Title.Text = "Output";

            LineItem myCurve;

            myCurve = myPane.AddCurve("Roll", list, Color.Red, SymbolType.None);

            myCurve = myPane.AddCurve("Pitch", list2, Color.Blue, SymbolType.None);

            myCurve = myPane.AddCurve("Rudder", list3, Color.Green, SymbolType.None);

            myCurve = myPane.AddCurve("Throttle", list4, Color.Orange, SymbolType.None);


            // Show the x axis grid
            myPane.XAxis.MajorGrid.IsVisible = true;

            myPane.XAxis.Scale.Min = 0;
            myPane.XAxis.Scale.Max = 5;

            // Make the Y axis scale red
            myPane.YAxis.Scale.FontSpec.FontColor = Color.Red;
            myPane.YAxis.Title.FontSpec.FontColor = Color.Red;
            // turn off the opposite tics so the Y tics don't show up on the Y2 axis
            myPane.YAxis.MajorTic.IsOpposite = false;
            myPane.YAxis.MinorTic.IsOpposite = false;
            // Don't display the Y zero line
            myPane.YAxis.MajorGrid.IsZeroLine = true;
            // Align the Y axis labels so they are flush to the axis
            myPane.YAxis.Scale.Align = AlignP.Inside;
            // Manually set the axis range
            //myPane.YAxis.Scale.Min = -1;
            //myPane.YAxis.Scale.Max = 1;

            // Fill the axis background with a gradient
            myPane.Chart.Fill = new Fill(Color.White, Color.LightGray, 45.0f);

            // Sample at 50ms intervals
            timer1.Interval = 50;
            timer1.Enabled = true;
            timer1.Start();


            // Calculate the Axis Scale Ranges
            zgc.AxisChange();

            tickStart = Environment.TickCount;


        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            // Make sure that the curvelist has at least one curve
            if (zg1.GraphPane.CurveList.Count <= 0)
                return;

            // Get the first CurveItem in the graph
            LineItem curve = zg1.GraphPane.CurveList[0] as LineItem;
            if (curve == null)
                return;

            // Get the PointPairList
            IPointListEdit list = curve.Points as IPointListEdit;
            // If this is null, it means the reference at curve.Points does not
            // support IPointListEdit, so we won't be able to modify it
            if (list == null)
                return;

            // Time is measured in seconds
            double time = (Environment.TickCount - tickStart) / 1000.0;

            // Keep the X scale at a rolling 30 second interval, with one
            // major step between the max X value and the end of the axis
            Scale xScale = zg1.GraphPane.XAxis.Scale;
            if (time > xScale.Max - xScale.MajorStep)
            {
                xScale.Max = time + xScale.MajorStep;
                xScale.Min = xScale.Max - 30.0;
            }

            // Make sure the Y axis is rescaled to accommodate actual data
            zg1.AxisChange();
            // Force a redraw
            zg1.Invalidate();

        }

        private void ArdupilotSim_FormClosing(object sender, FormClosingEventArgs e)
        {
            threadrun = 0;
            System.Threading.Thread.Sleep(100); // make sure thread will exit
            if (comPort.IsOpen)
            {
                ConnectComPort_Click(sender, e);
            }

            if (displayfull)
            {
                // close GE
                SendMessage(ge.GetMainHwnd(), WM_CLOSE, 0, 0);
            }
        }

        private void SaveSettings_Click(object sender, EventArgs e)
        {
            xmlconfig(true);
        }
    }
}
