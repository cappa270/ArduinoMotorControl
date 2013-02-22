using System;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;
using System.IO;

namespace mtkgpstest
{
    class Program
    {
        static List<string> position = new List<string>();

        static void Main(string[] args)
        {
                        int a = 1;
            

            foreach (string str in SerialPort.GetPortNames())
            {
                Console.WriteLine("{0}      {1}", a, str);
                a++;
            }
            Console.WriteLine("Please Enter number Next to COM port: ");

            int port = int.Parse(Console.ReadLine());

            string portName = SerialPort.GetPortNames()[port - 1];

            SerialPort comPort = new SerialPort();

            comPort.BaudRate = 38400;
            comPort.DataBits = 8;
            comPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), "1");
            comPort.Parity = (Parity)Enum.Parse(typeof(Parity), "None");
            comPort.PortName = portName;

            comPort.Open();
            // switch to binary mode
            comPort.Write("$PGCMD,16,0,0,0,0,0*6A\r\n");

            while (true)
            {
                    byte data;
                    int numc;

                    System.Threading.Thread.Sleep(1); 

                    numc = comPort.BytesToRead;
                    if (numc > 0)
                    {
                        for (int i = 0; i < numc; i++)
                        {	// Process bytes received
                            data = (byte)comPort.ReadByte();
                            switch (GPS_step)
                            {		 //Normally we start from zero. This is a state machine
                                case 0:
                                    if (data == 0xB5)	// MTK sync char 1
                                        GPS_step++;	 //ooh! first data packet is correct. Jump to the next step.
                                    break;
                                case 1:
                                    if (data == 0x62)
                                    {	// MTK sync char 2
                                        GPS_step++;	 //ooh! The second data packet is correct, jump to the step 2
                                    }
                                    else
                                    {
                                        GPS_step = 0;	 //Nope, incorrect. Restart to step zero and try again.
                                    }
                                    break;
                                case 2:
                                    MTK_class = data;
                                    checksum(MTK_class);
                                    GPS_step++;
                                    break;
                                case 3:
                                    MTK_id = data;
                                    GPS_step++;
                                    MTK_payload_counter = 0;
                                    checksum(MTK_id);
                                    break;

                                case 4:
                                    if (MTK_payload_counter < MTK_payload_length_hi)
                                    {  // We stay in this state until we reach the payload_length
                                        MTK_buffer[MTK_payload_counter] = data;
                                        checksum(data);
                                        MTK_payload_counter++;
                                        if (MTK_payload_counter == MTK_payload_length_hi)
                                        {
                                            GPS_step++;
                                        }
                                    }
                                    break;

                                case 5:
                                    MTK_ck_a = data;   // First checksum byte
                                    GPS_step++;
                                    break;

                                case 6: // Payload data read...
                                    MTK_ck_b = data;   // Second checksum byte

                                    // We end the GPS read...
                                    if ((ck_a == MTK_ck_a) && (ck_b == MTK_ck_b))
                                    {   	// Verify the received checksum with the generated checksum.. 
                                        //Serial.println(" ");
                                        //GPS_join_data();               					// Parse the new GPS packet

                                        int j; // our Byte Offset
                                        //Verifing if we are in class 1, you can change this "IF" for a "Switch" in case you want to use other MTK classes.. 
                                        //In this case all the message im using are in class 1, to know more about classes check PAGE 60 of DataSheet.
                                        /*
                                        if (MTK_class == 0x01)
                                        {
                                            switch (MTK_id)
                                            {
                                                case 0x05: //ID Custom
                                                    current_loc.lat = join_4_bytes(&MTK_buffer[0]) * 10;
                                                    current_loc.lng = join_4_bytes(&MTK_buffer[4]) * 10;
                                                    current_loc.alt = join_4_bytes(&MTK_buffer[8]); //alt_MSL M*100
                                                    ground_speed = join_4_bytes(&MTK_buffer[12]); //M*100		
                                                    ground_course = join_4_bytes(&MTK_buffer[16]) / 10000; // Heading 2D
                                                    NumSats = MTK_buffer[20];
                                                    GPS_fix = MTK_buffer[21];
                                                    iTOW = join_4_bytes(&MTK_buffer[22]);

                                                    if (GPS_fix > 0x01)
                                                    {
                                                        GPS_fix = VALID_GPS;
                                                        GPS_new_data = true;
                                                    }
                                                    else
                                                    {
                                                        GPS_fix = BAD_GPS;
                                                        GPS_new_data = false;
                                                    }
                                                    break;
                                            }
                                        }
                                         */

                                        double ddlat = convertend(MTK_buffer, 0) / 1000000;
                                        int d = (int)ddlat;
                                        int m = (int)((ddlat - d) * 60);
                                        double s = ((((ddlat - d) * 60)-m) * 60);

                                        double ddlng = convertend(MTK_buffer, 4) / 1000000;
                                        int d2 = (int)ddlng;
                                        int m2 = (int)((ddlng - d2) * 60);
                                        double s2 = ((((ddlng - d2) * 60) - m2) * 60);

                                        Console.Write("{0}.{1}'{2}\"  {3}.{4}'{5}\"\n",d,Math.Abs(m),Math.Abs(s).ToString("0.000"),d2,Math.Abs(m2),Math.Abs(s2).ToString("0.000"));

                                        position.Add(string.Format("{1},{0},{2}\r\n", ddlat, ddlng, convertend(MTK_buffer, 8) / 100));

                                        writeKML();

                                        Console.WriteLine("Lat {2} Long {3} Alt {4}\nGrspd {5} Grcrs {6} Sat {7} Fix {8} itow {9}", MTK_class, MTK_id, convertend(MTK_buffer, 0) / 1000000, convertend(MTK_buffer, 4) / 1000000, convertend(MTK_buffer, 8) / 100, convertend(MTK_buffer, 12), convertend(MTK_buffer, 16) / 1000000, MTK_buffer[20], MTK_buffer[21],convertend(MTK_buffer, 22));
                                        //GPS_timer = millis(); //Restarting timer...
                                    }
                                    else
                                    {
                                        //Serial.println("failed Checksum");
                                        //Serial.flush();
                                        GPS_step = 0;
                                    }
                                    // Variable initialization
                                    GPS_step = 0;
                                    ck_a = 0;
                                    ck_b = 0;
                                    break;
                            }
                        }
                    }
                
            }
        }

        static private void writeKML()
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

        static double convertend(byte[] buffer, int pos)
        {
            int answer = 0;

            answer += buffer[pos+0] << 24;
            answer += buffer[pos+1] << 16;
            answer += buffer[pos+2] << 8;
            answer += buffer[pos+3] << 0;

            return (double)answer;
        }

        static byte GPS_step = 0; /// intresting

        static byte ck_a = 0;
        static byte ck_b = 0;
        static byte[] MTK_buffer = new byte[60];

        //MediaTek Checksum
        static byte MTK_class = 0;
        static byte MTK_id = 0;
        static byte MTK_payload_length_hi = 26;
        static byte MTK_payload_counter = 0;
        static byte MTK_ck_a = 0;
        static byte MTK_ck_b = 0;

        static void checksum(byte data)
        {
            //Serial.print(data, HEX);
            //Serial.print(",");
            ck_a += data;
            ck_b += ck_a;
        }
    }
}
