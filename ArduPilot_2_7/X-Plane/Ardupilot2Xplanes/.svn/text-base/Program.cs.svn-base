using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO.Ports;
using System.IO;

namespace Ardupilot2Sim
{
    class Program
    {
        static int lastmode = 0;
        static List<string> position = new List<string>();
        static string software = "X";
        static TcpClient FlightGearSEND;
        static int REV_pitch = 1;
        static int REV_roll = 1;
        static int REV_rudder = 1;
        static int GPS_update = 200;
        static DateTime lastgpsupdate = DateTime.Now;
        static DateTime FGnow = DateTime.Now;
        static int AAAlength = 17 + 3 + 2; // 3 header + 17 data + 2 cr and lf

        public static void readconfig()
        {
            if (File.Exists("config.txt"))
            {
                StreamReader sr = new StreamReader("config.txt");
                REV_pitch = int.Parse(sr.ReadLine());
                REV_roll = int.Parse(sr.ReadLine());
                GPS_update = int.Parse(sr.ReadLine());
                REV_rudder = int.Parse(sr.ReadLine());
                sr.Close();
            }
            else {
                StreamWriter sw = new StreamWriter("config.txt");
                sw.WriteLine("1");
                sw.WriteLine("1");
                sw.WriteLine("200");
                sw.WriteLine("1");
                sw.WriteLine("1");
                sw.WriteLine("1");
                sw.WriteLine("1");
                sw.WriteLine("1");
                sw.WriteLine("1");
                sw.WriteLine("1");
                sw.WriteLine("1");
                sw.WriteLine("1");
                sw.WriteLine("1");
                sw.WriteLine("1");
                sw.WriteLine("1");
                sw.WriteLine("1");
                sw.WriteLine("1");
                sw.WriteLine("line1 is pitch reverse either -1 or 1");
                sw.WriteLine("line2 is roll reverse either -1 or 1");
                sw.WriteLine("line3 is gps update rate in ms 200 = 5 hz");
                sw.WriteLine("line4 is rudder reverse either -1 or 1");
                sw.Close();
            }
        }

        public static byte[] readline(SerialPort comport)
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

                if (temp[count] == '\n' && step == AAAlength-1) // dont finish until we have all data
                {
                    break;
                }

                count++;
                if (count == 100)
                    break;
            }

            byte[] temp2 = new byte[count+1];

            Array.Copy(temp, 0, temp2, 0, count+1);

            return temp2;
        }
        
        static void Main(string[] args)
        {
            Console.WriteLine("Ardupilot2Xplanes&FlightGear V0.44\n\nConfig file has changed please delete yours and reconfig\nFlight gear users the protocol xml file has changed, please update.\n");
            int a = 1;
            foreach (string str in SerialPort.GetPortNames())
            {
                Console.WriteLine("{0}      {1}", a, str);
                a++;
            }
            Console.WriteLine("Please Enter number Next to COM port: ");

            int port = int.Parse(Console.ReadLine());

            string portName = SerialPort.GetPortNames()[port - 1];

            Console.WriteLine("X = Xplanes or F = FlightGear");

            software = Console.ReadLine();


            SerialPort comPort = new SerialPort();

            comPort.BaudRate = 38400;
            comPort.DataBits = 8;
            comPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), "1");
            comPort.Parity = (Parity)Enum.Parse(typeof(Parity), "None");
            comPort.PortName = portName;

            comPort.Open();

            readconfig();

            UdpClient XplanesSEND = new UdpClient("127.0.0.1", 49000);

            if (software.ToUpper().Equals("F"))
            {
                Console.WriteLine("Start FG First with these settings\n\n--generic=socket,out,50,127.0.0.1,49005,udp,ardupilot \n--generic=socket,in,50,127.0.0.1,49000,tcp,ardupilot\nOne thing i have noticed... always let FG start the simulation before starting this prog.. else you will notice a delay.");
                try
                {
                    FlightGearSEND = new TcpClient("127.0.0.1", 49000);
                }
                catch (Exception) { Console.WriteLine("Failed to connect to flightgear, falling back to Xplanes"); software = "X"; }
            }
            else { software = "X"; }


            IPEndPoint ipep = new IPEndPoint(IPAddress.Any, 49005);

            Socket XplanesRECV = new Socket(AddressFamily.InterNetwork,
                            SocketType.Dgram, ProtocolType.Udp);

            XplanesRECV.Bind(ipep);

            int recv;
            byte[] udpdata = new byte[113 * 9 + 5]; // 113 types - 9 items per type (index+8) + 5 byte header

            IPEndPoint sender = new IPEndPoint(IPAddress.Any, 0);
            EndPoint Remote = (EndPoint)(sender);

            //FileStream stream = File.OpenWrite("comportR.txt");

            while (true)
            { 
                if (XplanesRECV.Available > 0)
                {
                    udpdata = new byte[udpdata.Length];

                    recv = XplanesRECV.ReceiveFrom(udpdata, ref Remote);

                    XplanesRECVprocess(udpdata, recv, comPort);
                }

                if (comPort.BytesToRead >= 4)
                {
                    byte[] buffer = readline(comPort);

                    if (buffer[0] == 'A' && buffer[1] == 'A' && buffer[2] == 'A')
                    {
                        //stream.Write(buffer, 0, buffer.Length);
                        //stream.Flush();

                        //System.Threading.Thread t11 = new System.Threading.Thread(delegate() { processArduPilot(buffer, XplanesSEND, FlightGearSEND); });
                        //t11.Start();

                        processArduPilot(buffer, XplanesSEND, FlightGearSEND);
                    }
                    else
                    {
                        //stream.Write(buffer, 0, buffer.Length);
                        //stream.Flush();
                        //Console.WriteLine("test {0}     ", comPort.BytesToRead);
                        //comPort.DiscardInBuffer();
                        Console.Write("                                                                     \r");
                        Console.Write(Encoding.ASCII.GetString(buffer, 0, buffer.Length));
                    }
                }
                System.Threading.Thread.Sleep(1); // try minimise delays
            }

        }

        static float[][] DATA = new float[113][];
        static DateTime now = DateTime.Now;
        static int udpcount = 0;

        public static void XplanesRECVprocess(byte[] data, int receviedbytes,SerialPort comPort) {
            udpcount++;
            TimeSpan duration = DateTime.Now - now;
            if (duration.Seconds >= 1)
            {
                //Console.WriteLine(udpcount);
                udpcount = 0;
                now = DateTime.Now;
            }

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



            // write arduimu to ardupilot



            	//convert data	
	int lat 		= (int)(DATA[20][0] * 10000000);
	int lng 		= (int)(DATA[20][1] * 10000000);
	int altitude 	= (int)(DATA[20][2] * 3.048);		// altitude to meters
	int speed 		= (int)(DATA[3][7] * 044.704);		// spped to m/s * 100
	int airspeed 	= (int)(DATA[3][6] * 044.704);		// speed to m/s * 100
	int pitch 		= (int)(DATA[18][0] * 100);
	int roll 		= (int)(DATA[18][1] * 100);
	int heading 	= (int)(DATA[18][2] * 100);

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

            byte check_a=0;
            byte check_b=0;

            	for(int i = 4;i <= 13; i++ ){     
		                check_a += IMU[i];
		                check_b += check_a;
                }


            IMU[14] = check_a;
            IMU[15] = check_b;

            comPort.Write(IMU, 0, 16);

            TimeSpan gpsspan = DateTime.Now - lastgpsupdate;

            if ((gpsspan.Seconds * 1000) + gpsspan.Milliseconds >= GPS_update)
            {
                lastgpsupdate = DateTime.Now;

                position.Add(DATA[20][1] + "," + DATA[20][0] + "," + (DATA[20][2] * .3048)  + "\r\n");

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

        static int fgsent = 0;

        static void processArduPilot(byte[] line, UdpClient XplanesSEND, TcpClient FlightGearSEND)
        {
            if (line.Length < AAAlength-1)
                return;


            float roll_out = (float)(Int16)(line[3] + (line[4] << 8)) / 4500 ;
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



            if (lastmode != control_mode) {
                Console.WriteLine("\nMODE CHANGE - writeing KML");

                FileStream stream = File.Open("flight" + lastmode + ".kml",FileMode.Create);

                System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();

                string header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><kml xmlns=\"http://www.opengis.net/kml/2.2\">\n  <Document>    <name>Paths</name>    <description>Path</description>\n    <Style id=\"yellowLineGreenPoly\">      <LineStyle>        <color>7f00ffff</color>        <width>4</width>      </LineStyle>      <PolyStyle>        <color>7f00ff00</color>      </PolyStyle>    </Style>\n    <Placemark>      <name>Absolute Extruded</name>      <description>Transparent green wall with yellow outlines</description>      <styleUrl>#yellowLineGreenPoly</styleUrl>      <LineString>        <extrude>1</extrude>        <tessellate>1</tessellate>        <altitudeMode>absolute</altitudeMode>        <coordinates>";

                string footer = "      </coordinates>      </LineString>    </Placemark>     <Folder>      <name>Alerts</name>        </Folder>      <Folder>            <name>Mode</name>                 </Folder>      </Document></kml>";

                byte[] temp = encoding.GetBytes(header);

                stream.Write(temp, 0, temp.Length);

                foreach (string pos in position.ToArray()) {
                    
                    byte[] posbytes = encoding.GetBytes(pos);

                    stream.Write(posbytes, 0, posbytes.Length);
                }

                temp = encoding.GetBytes(footer);

                stream.Write(temp, 0, temp.Length);

                stream.Close();

                position.Clear();

                lastmode = control_mode;
            }



            Console.Write(string.Format("R {0,6} P {1,6} Rud {2,6} T {3,6} DST {4,5} BE {5,5} AE {6,5} WP {7} M {8} \r", roll_out.ToString("0.000"), pitch_out.ToString("0.000"), rudder_out.ToString("0.000"),throttle_out.ToString("0.000"), wp_distance, bearing_error, alt_error, wp_index, control_mode));

            //t
            //j
            //c

            // Flightgear

            if (software.ToUpper().Equals("F"))
            {
                fgsent++;
                TimeSpan duration = DateTime.Now - FGnow;
                if (duration.Seconds >= 1)
                {
                    //Console.WriteLine(duration.Seconds + " second gone "+fgsent + " sent " + (50 - fgsent) + " short     ");
                    fgsent = 0;
                    FGnow = DateTime.Now;
                }

                if (fgsent % 10 == 0) { return; } // short supply buffer.. seems to reduce lag

                string send = "3," + (roll_out * REV_roll) + "," + (pitch_out * REV_pitch * -1) + "," + (rudder_out * REV_rudder) + "," + (throttle_out) + "\r\n";

                byte[] FlightGear = new System.Text.ASCIIEncoding().GetBytes(send);

                try
                {
                    FlightGearSEND.GetStream().Write(FlightGear, 0, FlightGear.Length);
                    //XplanesSEND.Send(FlightGear, FlightGear.Length);  // if FG udp worked......
                }
                catch (Exception) { Console.WriteLine("Socket Write failed, FG closed?"); software = "X"; }

            }

            // Xplanes

            if (software.ToUpper().Equals("X"))
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
    }
}
