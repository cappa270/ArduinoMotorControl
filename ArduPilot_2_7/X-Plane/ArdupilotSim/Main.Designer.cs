namespace ArdupilotSim
{
    partial class ArdupilotSim
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.Comports = new System.Windows.Forms.ComboBox();
            this.CHKREV_roll = new System.Windows.Forms.CheckBox();
            this.CHKREV_pitch = new System.Windows.Forms.CheckBox();
            this.CHKREV_rudder = new System.Windows.Forms.CheckBox();
            this.GPSrate = new System.Windows.Forms.ComboBox();
            this.ConnectComPort = new System.Windows.Forms.Button();
            this.OutputLog = new System.Windows.Forms.RichTextBox();
            this.TXT_roll = new System.Windows.Forms.TextBox();
            this.TXT_pitch = new System.Windows.Forms.TextBox();
            this.TXT_heading = new System.Windows.Forms.TextBox();
            this.TXT_wpdist = new System.Windows.Forms.TextBox();
            this.TXT_bererror = new System.Windows.Forms.TextBox();
            this.TXT_alterror = new System.Windows.Forms.TextBox();
            this.TXT_lat = new System.Windows.Forms.TextBox();
            this.TXT_long = new System.Windows.Forms.TextBox();
            this.TXT_alt = new System.Windows.Forms.TextBox();
            this.SaveSettings = new System.Windows.Forms.Button();
            this.RAD_softXplanes = new System.Windows.Forms.RadioButton();
            this.RAD_softFlightGear = new System.Windows.Forms.RadioButton();
            this.TXT_servoroll = new System.Windows.Forms.TextBox();
            this.TXT_servopitch = new System.Windows.Forms.TextBox();
            this.TXT_servorudder = new System.Windows.Forms.TextBox();
            this.TXT_servothrottle = new System.Windows.Forms.TextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.label11 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.panel3 = new System.Windows.Forms.Panel();
            this.label16 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.panel4 = new System.Windows.Forms.Panel();
            this.label20 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.TXT_control_mode = new System.Windows.Forms.TextBox();
            this.TXT_WP = new System.Windows.Forms.TextBox();
            this.label18 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.panel5 = new System.Windows.Forms.Panel();
            this.zg1 = new ZedGraph.ZedGraphControl();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.panel6 = new System.Windows.Forms.Panel();
            this.label28 = new System.Windows.Forms.Label();
            this.label29 = new System.Windows.Forms.Label();
            this.label27 = new System.Windows.Forms.Label();
            this.label25 = new System.Windows.Forms.Label();
            this.TXT_throttlegain = new System.Windows.Forms.TextBox();
            this.label24 = new System.Windows.Forms.Label();
            this.label23 = new System.Windows.Forms.Label();
            this.label22 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.TXT_ruddergain = new System.Windows.Forms.TextBox();
            this.TXT_pitchgain = new System.Windows.Forms.TextBox();
            this.TXT_rollgain = new System.Windows.Forms.TextBox();
            this.label26 = new System.Windows.Forms.Label();
            this.Googleearthpanel = new System.Windows.Forms.Panel();
            this.CHKdisplayall = new System.Windows.Forms.CheckBox();
            this.CHKgraphroll = new System.Windows.Forms.CheckBox();
            this.CHKgraphpitch = new System.Windows.Forms.CheckBox();
            this.CHKgraphrudder = new System.Windows.Forms.CheckBox();
            this.CHKgraphthrottle = new System.Windows.Forms.CheckBox();
            this.headingIndicatorInstrumentControl1 = new AvionicsInstrumentControlDemo.HeadingIndicatorInstrumentControl();
            this.altimeterInstrumentControl1 = new AvionicsInstrumentControlDemo.AltimeterInstrumentControl();
            this.airSpeedIndicatorInstrumentControl1 = new AvionicsInstrumentControlDemo.AirSpeedIndicatorInstrumentControl();
            this.attitudeIndicatorInstrumentControl1 = new AvionicsInstrumentControlDemo.AttitudeIndicatorInstrumentControl();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.panel4.SuspendLayout();
            this.panel5.SuspendLayout();
            this.panel6.SuspendLayout();
            this.SuspendLayout();
            // 
            // Comports
            // 
            this.Comports.FormattingEnabled = true;
            this.Comports.Location = new System.Drawing.Point(24, 5);
            this.Comports.Name = "Comports";
            this.Comports.Size = new System.Drawing.Size(128, 21);
            this.Comports.TabIndex = 0;
            this.Comports.Text = "Com Port";
            this.Comports.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Comports_MouseClick);
            // 
            // CHKREV_roll
            // 
            this.CHKREV_roll.AutoSize = true;
            this.CHKREV_roll.Location = new System.Drawing.Point(213, 10);
            this.CHKREV_roll.Name = "CHKREV_roll";
            this.CHKREV_roll.Size = new System.Drawing.Size(87, 17);
            this.CHKREV_roll.TabIndex = 1;
            this.CHKREV_roll.Text = "Reverse Roll";
            this.CHKREV_roll.UseVisualStyleBackColor = true;
            this.CHKREV_roll.CheckedChanged += new System.EventHandler(this.CHKREV_roll_CheckedChanged);
            // 
            // CHKREV_pitch
            // 
            this.CHKREV_pitch.AutoSize = true;
            this.CHKREV_pitch.Location = new System.Drawing.Point(299, 10);
            this.CHKREV_pitch.Name = "CHKREV_pitch";
            this.CHKREV_pitch.Size = new System.Drawing.Size(93, 17);
            this.CHKREV_pitch.TabIndex = 2;
            this.CHKREV_pitch.Text = "Reverse Pitch";
            this.CHKREV_pitch.UseVisualStyleBackColor = true;
            this.CHKREV_pitch.CheckedChanged += new System.EventHandler(this.CHKREV_pitch_CheckedChanged);
            // 
            // CHKREV_rudder
            // 
            this.CHKREV_rudder.AutoSize = true;
            this.CHKREV_rudder.Location = new System.Drawing.Point(398, 10);
            this.CHKREV_rudder.Name = "CHKREV_rudder";
            this.CHKREV_rudder.Size = new System.Drawing.Size(104, 17);
            this.CHKREV_rudder.TabIndex = 3;
            this.CHKREV_rudder.Text = "Reverse Rudder";
            this.CHKREV_rudder.UseVisualStyleBackColor = true;
            this.CHKREV_rudder.CheckedChanged += new System.EventHandler(this.CHKREV_rudder_CheckedChanged);
            // 
            // GPSrate
            // 
            this.GPSrate.FormattingEnabled = true;
            this.GPSrate.Items.AddRange(new object[] {
            "100",
            "200",
            "250",
            "500",
            "1000",
            "2000",
            "30000"});
            this.GPSrate.Location = new System.Drawing.Point(538, 36);
            this.GPSrate.Name = "GPSrate";
            this.GPSrate.Size = new System.Drawing.Size(92, 21);
            this.GPSrate.TabIndex = 4;
            this.GPSrate.Text = "200";
            this.GPSrate.SelectedIndexChanged += new System.EventHandler(this.GPSrate_SelectedIndexChanged);
            // 
            // ConnectComPort
            // 
            this.ConnectComPort.Location = new System.Drawing.Point(24, 29);
            this.ConnectComPort.Name = "ConnectComPort";
            this.ConnectComPort.Size = new System.Drawing.Size(128, 23);
            this.ConnectComPort.TabIndex = 5;
            this.ConnectComPort.Text = "Connect/Disconnect";
            this.ConnectComPort.UseVisualStyleBackColor = true;
            this.ConnectComPort.Click += new System.EventHandler(this.ConnectComPort_Click);
            // 
            // OutputLog
            // 
            this.OutputLog.Location = new System.Drawing.Point(197, 66);
            this.OutputLog.Name = "OutputLog";
            this.OutputLog.Size = new System.Drawing.Size(433, 208);
            this.OutputLog.TabIndex = 6;
            this.OutputLog.Text = "";
            this.OutputLog.TextChanged += new System.EventHandler(this.OutputLog_TextChanged);
            // 
            // TXT_roll
            // 
            this.TXT_roll.Location = new System.Drawing.Point(67, 22);
            this.TXT_roll.Name = "TXT_roll";
            this.TXT_roll.ReadOnly = true;
            this.TXT_roll.Size = new System.Drawing.Size(100, 20);
            this.TXT_roll.TabIndex = 7;
            // 
            // TXT_pitch
            // 
            this.TXT_pitch.Location = new System.Drawing.Point(67, 48);
            this.TXT_pitch.Name = "TXT_pitch";
            this.TXT_pitch.ReadOnly = true;
            this.TXT_pitch.Size = new System.Drawing.Size(100, 20);
            this.TXT_pitch.TabIndex = 8;
            // 
            // TXT_heading
            // 
            this.TXT_heading.Location = new System.Drawing.Point(67, 74);
            this.TXT_heading.Name = "TXT_heading";
            this.TXT_heading.ReadOnly = true;
            this.TXT_heading.Size = new System.Drawing.Size(100, 20);
            this.TXT_heading.TabIndex = 9;
            // 
            // TXT_wpdist
            // 
            this.TXT_wpdist.Location = new System.Drawing.Point(75, 24);
            this.TXT_wpdist.Name = "TXT_wpdist";
            this.TXT_wpdist.ReadOnly = true;
            this.TXT_wpdist.Size = new System.Drawing.Size(100, 20);
            this.TXT_wpdist.TabIndex = 10;
            // 
            // TXT_bererror
            // 
            this.TXT_bererror.Location = new System.Drawing.Point(75, 50);
            this.TXT_bererror.Name = "TXT_bererror";
            this.TXT_bererror.ReadOnly = true;
            this.TXT_bererror.Size = new System.Drawing.Size(100, 20);
            this.TXT_bererror.TabIndex = 11;
            // 
            // TXT_alterror
            // 
            this.TXT_alterror.Location = new System.Drawing.Point(75, 76);
            this.TXT_alterror.Name = "TXT_alterror";
            this.TXT_alterror.ReadOnly = true;
            this.TXT_alterror.Size = new System.Drawing.Size(100, 20);
            this.TXT_alterror.TabIndex = 12;
            // 
            // TXT_lat
            // 
            this.TXT_lat.Location = new System.Drawing.Point(67, 23);
            this.TXT_lat.Name = "TXT_lat";
            this.TXT_lat.ReadOnly = true;
            this.TXT_lat.Size = new System.Drawing.Size(100, 20);
            this.TXT_lat.TabIndex = 13;
            // 
            // TXT_long
            // 
            this.TXT_long.Location = new System.Drawing.Point(67, 49);
            this.TXT_long.Name = "TXT_long";
            this.TXT_long.ReadOnly = true;
            this.TXT_long.Size = new System.Drawing.Size(100, 20);
            this.TXT_long.TabIndex = 14;
            // 
            // TXT_alt
            // 
            this.TXT_alt.Location = new System.Drawing.Point(67, 75);
            this.TXT_alt.Name = "TXT_alt";
            this.TXT_alt.ReadOnly = true;
            this.TXT_alt.Size = new System.Drawing.Size(100, 20);
            this.TXT_alt.TabIndex = 15;
            // 
            // SaveSettings
            // 
            this.SaveSettings.Location = new System.Drawing.Point(576, 280);
            this.SaveSettings.Name = "SaveSettings";
            this.SaveSettings.Size = new System.Drawing.Size(54, 34);
            this.SaveSettings.TabIndex = 16;
            this.SaveSettings.Text = "Save Settings";
            this.SaveSettings.UseVisualStyleBackColor = true;
            this.SaveSettings.Click += new System.EventHandler(this.SaveSettings_Click);
            // 
            // RAD_softXplanes
            // 
            this.RAD_softXplanes.AutoSize = true;
            this.RAD_softXplanes.Checked = true;
            this.RAD_softXplanes.Location = new System.Drawing.Point(197, 40);
            this.RAD_softXplanes.Name = "RAD_softXplanes";
            this.RAD_softXplanes.Size = new System.Drawing.Size(63, 17);
            this.RAD_softXplanes.TabIndex = 17;
            this.RAD_softXplanes.TabStop = true;
            this.RAD_softXplanes.Text = "Xplanes";
            this.RAD_softXplanes.UseVisualStyleBackColor = true;
            this.RAD_softXplanes.CheckedChanged += new System.EventHandler(this.RAD_softXplanes_CheckedChanged);
            // 
            // RAD_softFlightGear
            // 
            this.RAD_softFlightGear.AutoSize = true;
            this.RAD_softFlightGear.Location = new System.Drawing.Point(266, 40);
            this.RAD_softFlightGear.Name = "RAD_softFlightGear";
            this.RAD_softFlightGear.Size = new System.Drawing.Size(73, 17);
            this.RAD_softFlightGear.TabIndex = 18;
            this.RAD_softFlightGear.Text = "FlightGear";
            this.RAD_softFlightGear.UseVisualStyleBackColor = true;
            this.RAD_softFlightGear.CheckedChanged += new System.EventHandler(this.RAD_softFlightGear_CheckedChanged);
            // 
            // TXT_servoroll
            // 
            this.TXT_servoroll.Location = new System.Drawing.Point(67, 24);
            this.TXT_servoroll.Name = "TXT_servoroll";
            this.TXT_servoroll.ReadOnly = true;
            this.TXT_servoroll.Size = new System.Drawing.Size(100, 20);
            this.TXT_servoroll.TabIndex = 19;
            // 
            // TXT_servopitch
            // 
            this.TXT_servopitch.Location = new System.Drawing.Point(67, 50);
            this.TXT_servopitch.Name = "TXT_servopitch";
            this.TXT_servopitch.ReadOnly = true;
            this.TXT_servopitch.Size = new System.Drawing.Size(100, 20);
            this.TXT_servopitch.TabIndex = 20;
            // 
            // TXT_servorudder
            // 
            this.TXT_servorudder.Location = new System.Drawing.Point(67, 76);
            this.TXT_servorudder.Name = "TXT_servorudder";
            this.TXT_servorudder.ReadOnly = true;
            this.TXT_servorudder.Size = new System.Drawing.Size(100, 20);
            this.TXT_servorudder.TabIndex = 21;
            // 
            // TXT_servothrottle
            // 
            this.TXT_servothrottle.Location = new System.Drawing.Point(67, 102);
            this.TXT_servothrottle.Name = "TXT_servothrottle";
            this.TXT_servothrottle.ReadOnly = true;
            this.TXT_servothrottle.Size = new System.Drawing.Size(100, 20);
            this.TXT_servothrottle.TabIndex = 22;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.label4);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.TXT_lat);
            this.panel1.Controls.Add(this.TXT_long);
            this.panel1.Controls.Add(this.TXT_alt);
            this.panel1.Location = new System.Drawing.Point(13, 66);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(178, 100);
            this.panel1.TabIndex = 23;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(60, 3);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(59, 13);
            this.label4.TabIndex = 19;
            this.label4.Text = "Plane GPS";
            this.label4.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(7, 78);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(42, 13);
            this.label3.TabIndex = 18;
            this.label3.Text = "Altitude";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 52);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(54, 13);
            this.label2.TabIndex = 17;
            this.label2.Text = "Longitude";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 26);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(45, 13);
            this.label1.TabIndex = 16;
            this.label1.Text = "Latitude";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.label11);
            this.panel2.Controls.Add(this.label7);
            this.panel2.Controls.Add(this.label6);
            this.panel2.Controls.Add(this.label5);
            this.panel2.Controls.Add(this.TXT_roll);
            this.panel2.Controls.Add(this.TXT_pitch);
            this.panel2.Controls.Add(this.TXT_heading);
            this.panel2.Location = new System.Drawing.Point(12, 172);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(178, 102);
            this.panel2.TabIndex = 24;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(60, 4);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(57, 13);
            this.label11.TabIndex = 19;
            this.label11.Text = "Plane IMU";
            this.label11.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(7, 81);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(47, 13);
            this.label7.TabIndex = 15;
            this.label7.Text = "Heading";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(7, 55);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(31, 13);
            this.label6.TabIndex = 14;
            this.label6.Text = "Pitch";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(7, 29);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(25, 13);
            this.label5.TabIndex = 13;
            this.label5.Text = "Roll";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(7, 31);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(43, 13);
            this.label8.TabIndex = 16;
            this.label8.Text = "WPDist";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(7, 57);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(69, 13);
            this.label9.TabIndex = 17;
            this.label9.Text = "Bearing ERR";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(7, 83);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(68, 13);
            this.label10.TabIndex = 18;
            this.label10.Text = "Altitude ERR";
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.label16);
            this.panel3.Controls.Add(this.label15);
            this.panel3.Controls.Add(this.label14);
            this.panel3.Controls.Add(this.label13);
            this.panel3.Controls.Add(this.label12);
            this.panel3.Controls.Add(this.TXT_servoroll);
            this.panel3.Controls.Add(this.TXT_servopitch);
            this.panel3.Controls.Add(this.TXT_servorudder);
            this.panel3.Controls.Add(this.TXT_servothrottle);
            this.panel3.Location = new System.Drawing.Point(13, 280);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(178, 134);
            this.panel3.TabIndex = 25;
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(50, 8);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(83, 13);
            this.label16.TabIndex = 27;
            this.label16.Text = "Ardupilot Output";
            this.label16.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(10, 108);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(43, 13);
            this.label15.TabIndex = 26;
            this.label15.Text = "Throttle";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(10, 82);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(28, 13);
            this.label14.TabIndex = 25;
            this.label14.Text = "Yaw";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(10, 56);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(31, 13);
            this.label13.TabIndex = 24;
            this.label13.Text = "Pitch";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(10, 30);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(25, 13);
            this.label12.TabIndex = 23;
            this.label12.Text = "Roll";
            // 
            // panel4
            // 
            this.panel4.Controls.Add(this.label20);
            this.panel4.Controls.Add(this.label19);
            this.panel4.Controls.Add(this.TXT_control_mode);
            this.panel4.Controls.Add(this.TXT_WP);
            this.panel4.Controls.Add(this.label18);
            this.panel4.Controls.Add(this.label10);
            this.panel4.Controls.Add(this.label9);
            this.panel4.Controls.Add(this.label8);
            this.panel4.Controls.Add(this.TXT_wpdist);
            this.panel4.Controls.Add(this.TXT_bererror);
            this.panel4.Controls.Add(this.TXT_alterror);
            this.panel4.Location = new System.Drawing.Point(197, 280);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(178, 134);
            this.panel4.TabIndex = 26;
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(72, 101);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(34, 13);
            this.label20.TabIndex = 23;
            this.label20.Text = "Mode";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(7, 102);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(25, 13);
            this.label19.TabIndex = 22;
            this.label19.Text = "WP";
            // 
            // TXT_control_mode
            // 
            this.TXT_control_mode.Location = new System.Drawing.Point(112, 99);
            this.TXT_control_mode.Name = "TXT_control_mode";
            this.TXT_control_mode.ReadOnly = true;
            this.TXT_control_mode.Size = new System.Drawing.Size(63, 20);
            this.TXT_control_mode.TabIndex = 21;
            // 
            // TXT_WP
            // 
            this.TXT_WP.Location = new System.Drawing.Point(38, 98);
            this.TXT_WP.Name = "TXT_WP";
            this.TXT_WP.ReadOnly = true;
            this.TXT_WP.Size = new System.Drawing.Size(28, 20);
            this.TXT_WP.TabIndex = 20;
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(50, 8);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(81, 13);
            this.label18.TabIndex = 19;
            this.label18.Text = "Autopilot Status";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(535, 9);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(95, 13);
            this.label17.TabIndex = 27;
            this.label17.Text = "GPS Refresh Rate";
            // 
            // panel5
            // 
            this.panel5.Controls.Add(this.Comports);
            this.panel5.Controls.Add(this.ConnectComPort);
            this.panel5.Location = new System.Drawing.Point(13, 5);
            this.panel5.Name = "panel5";
            this.panel5.Size = new System.Drawing.Size(178, 52);
            this.panel5.TabIndex = 28;
            // 
            // zg1
            // 
            this.zg1.Location = new System.Drawing.Point(12, 420);
            this.zg1.Name = "zg1";
            this.zg1.ScrollGrace = 0D;
            this.zg1.ScrollMaxX = 0D;
            this.zg1.ScrollMaxY = 0D;
            this.zg1.ScrollMaxY2 = 0D;
            this.zg1.ScrollMinX = 0D;
            this.zg1.ScrollMinY = 0D;
            this.zg1.ScrollMinY2 = 0D;
            this.zg1.Size = new System.Drawing.Size(618, 325);
            this.zg1.TabIndex = 29;
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // panel6
            // 
            this.panel6.Controls.Add(this.label28);
            this.panel6.Controls.Add(this.label29);
            this.panel6.Controls.Add(this.label27);
            this.panel6.Controls.Add(this.label25);
            this.panel6.Controls.Add(this.TXT_throttlegain);
            this.panel6.Controls.Add(this.label24);
            this.panel6.Controls.Add(this.label23);
            this.panel6.Controls.Add(this.label22);
            this.panel6.Controls.Add(this.label21);
            this.panel6.Controls.Add(this.TXT_ruddergain);
            this.panel6.Controls.Add(this.TXT_pitchgain);
            this.panel6.Controls.Add(this.TXT_rollgain);
            this.panel6.Location = new System.Drawing.Point(382, 280);
            this.panel6.Name = "panel6";
            this.panel6.Size = new System.Drawing.Size(178, 134);
            this.panel6.TabIndex = 30;
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.Location = new System.Drawing.Point(126, 76);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(48, 13);
            this.label28.TabIndex = 32;
            this.label28.Text = "SIM only";
            // 
            // label29
            // 
            this.label29.AutoSize = true;
            this.label29.Location = new System.Drawing.Point(126, 37);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(43, 13);
            this.label29.TabIndex = 33;
            this.label29.Text = "NOTE: ";
            // 
            // label27
            // 
            this.label27.AutoSize = true;
            this.label27.Location = new System.Drawing.Point(126, 63);
            this.label27.Name = "label27";
            this.label27.Size = new System.Drawing.Size(22, 13);
            this.label27.TabIndex = 31;
            this.label27.Text = "are";
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(4, 105);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(68, 13);
            this.label25.TabIndex = 8;
            this.label25.Text = "Throttle Gain";
            // 
            // TXT_throttlegain
            // 
            this.TXT_throttlegain.Location = new System.Drawing.Point(75, 100);
            this.TXT_throttlegain.Name = "TXT_throttlegain";
            this.TXT_throttlegain.Size = new System.Drawing.Size(45, 20);
            this.TXT_throttlegain.TabIndex = 7;
            this.TXT_throttlegain.Text = "100";
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(4, 79);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(67, 13);
            this.label24.TabIndex = 6;
            this.label24.Text = "Rudder Gain";
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(4, 55);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(56, 13);
            this.label23.TabIndex = 5;
            this.label23.Text = "Pitch Gain";
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(4, 29);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(50, 13);
            this.label22.TabIndex = 4;
            this.label22.Text = "Roll Gain";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(2, 7);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(169, 13);
            this.label21.TabIndex = 3;
            this.label21.Text = "Simulator Authority - For diff planes";
            // 
            // TXT_ruddergain
            // 
            this.TXT_ruddergain.Location = new System.Drawing.Point(75, 74);
            this.TXT_ruddergain.Name = "TXT_ruddergain";
            this.TXT_ruddergain.Size = new System.Drawing.Size(45, 20);
            this.TXT_ruddergain.TabIndex = 2;
            this.TXT_ruddergain.Text = "6000";
            // 
            // TXT_pitchgain
            // 
            this.TXT_pitchgain.Location = new System.Drawing.Point(75, 48);
            this.TXT_pitchgain.Name = "TXT_pitchgain";
            this.TXT_pitchgain.Size = new System.Drawing.Size(45, 20);
            this.TXT_pitchgain.TabIndex = 1;
            this.TXT_pitchgain.Text = "6000";
            // 
            // TXT_rollgain
            // 
            this.TXT_rollgain.Location = new System.Drawing.Point(75, 23);
            this.TXT_rollgain.Name = "TXT_rollgain";
            this.TXT_rollgain.Size = new System.Drawing.Size(45, 20);
            this.TXT_rollgain.TabIndex = 0;
            this.TXT_rollgain.Text = "6000";
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Location = new System.Drawing.Point(508, 330);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(37, 13);
            this.label26.TabIndex = 9;
            this.label26.Text = "These";
            // 
            // Googleearthpanel
            // 
            this.Googleearthpanel.Location = new System.Drawing.Point(636, 12);
            this.Googleearthpanel.Name = "Googleearthpanel";
            this.Googleearthpanel.Size = new System.Drawing.Size(412, 391);
            this.Googleearthpanel.TabIndex = 35;
            // 
            // CHKdisplayall
            // 
            this.CHKdisplayall.AutoSize = true;
            this.CHKdisplayall.Location = new System.Drawing.Point(345, 41);
            this.CHKdisplayall.Name = "CHKdisplayall";
            this.CHKdisplayall.Size = new System.Drawing.Size(182, 17);
            this.CHKdisplayall.TabIndex = 36;
            this.CHKdisplayall.Text = "Display All (restart prog - Fast PC)";
            this.CHKdisplayall.UseVisualStyleBackColor = true;
            // 
            // CHKgraphroll
            // 
            this.CHKgraphroll.AutoSize = true;
            this.CHKgraphroll.Checked = true;
            this.CHKgraphroll.CheckState = System.Windows.Forms.CheckState.Checked;
            this.CHKgraphroll.Location = new System.Drawing.Point(637, 520);
            this.CHKgraphroll.Name = "CHKgraphroll";
            this.CHKgraphroll.Size = new System.Drawing.Size(74, 17);
            this.CHKgraphroll.TabIndex = 37;
            this.CHKgraphroll.Text = "Show Roll";
            this.CHKgraphroll.UseVisualStyleBackColor = true;
            // 
            // CHKgraphpitch
            // 
            this.CHKgraphpitch.AutoSize = true;
            this.CHKgraphpitch.Checked = true;
            this.CHKgraphpitch.CheckState = System.Windows.Forms.CheckState.Checked;
            this.CHKgraphpitch.Location = new System.Drawing.Point(637, 544);
            this.CHKgraphpitch.Name = "CHKgraphpitch";
            this.CHKgraphpitch.Size = new System.Drawing.Size(80, 17);
            this.CHKgraphpitch.TabIndex = 38;
            this.CHKgraphpitch.Text = "Show Pitch";
            this.CHKgraphpitch.UseVisualStyleBackColor = true;
            // 
            // CHKgraphrudder
            // 
            this.CHKgraphrudder.AutoSize = true;
            this.CHKgraphrudder.Checked = true;
            this.CHKgraphrudder.CheckState = System.Windows.Forms.CheckState.Checked;
            this.CHKgraphrudder.Location = new System.Drawing.Point(637, 568);
            this.CHKgraphrudder.Name = "CHKgraphrudder";
            this.CHKgraphrudder.Size = new System.Drawing.Size(91, 17);
            this.CHKgraphrudder.TabIndex = 39;
            this.CHKgraphrudder.Text = "Show Rudder";
            this.CHKgraphrudder.UseVisualStyleBackColor = true;
            // 
            // CHKgraphthrottle
            // 
            this.CHKgraphthrottle.AutoSize = true;
            this.CHKgraphthrottle.Checked = true;
            this.CHKgraphthrottle.CheckState = System.Windows.Forms.CheckState.Checked;
            this.CHKgraphthrottle.Location = new System.Drawing.Point(636, 592);
            this.CHKgraphthrottle.Name = "CHKgraphthrottle";
            this.CHKgraphthrottle.Size = new System.Drawing.Size(92, 17);
            this.CHKgraphthrottle.TabIndex = 40;
            this.CHKgraphthrottle.Text = "Show Throttle";
            this.CHKgraphthrottle.UseVisualStyleBackColor = true;
            // 
            // headingIndicatorInstrumentControl1
            // 
            this.headingIndicatorInstrumentControl1.Location = new System.Drawing.Point(727, 580);
            this.headingIndicatorInstrumentControl1.Name = "headingIndicatorInstrumentControl1";
            this.headingIndicatorInstrumentControl1.Size = new System.Drawing.Size(156, 165);
            this.headingIndicatorInstrumentControl1.TabIndex = 34;
            this.headingIndicatorInstrumentControl1.Text = "headingIndicatorInstrumentControl1";
            // 
            // altimeterInstrumentControl1
            // 
            this.altimeterInstrumentControl1.Location = new System.Drawing.Point(890, 580);
            this.altimeterInstrumentControl1.Name = "altimeterInstrumentControl1";
            this.altimeterInstrumentControl1.Size = new System.Drawing.Size(157, 159);
            this.altimeterInstrumentControl1.TabIndex = 33;
            this.altimeterInstrumentControl1.Text = "altimeterInstrumentControl1";
            // 
            // airSpeedIndicatorInstrumentControl1
            // 
            this.airSpeedIndicatorInstrumentControl1.Location = new System.Drawing.Point(727, 409);
            this.airSpeedIndicatorInstrumentControl1.Name = "airSpeedIndicatorInstrumentControl1";
            this.airSpeedIndicatorInstrumentControl1.Size = new System.Drawing.Size(157, 165);
            this.airSpeedIndicatorInstrumentControl1.TabIndex = 32;
            this.airSpeedIndicatorInstrumentControl1.Text = "airSpeedIndicatorInstrumentControl1";
            // 
            // attitudeIndicatorInstrumentControl1
            // 
            this.attitudeIndicatorInstrumentControl1.Location = new System.Drawing.Point(890, 409);
            this.attitudeIndicatorInstrumentControl1.Name = "attitudeIndicatorInstrumentControl1";
            this.attitudeIndicatorInstrumentControl1.Size = new System.Drawing.Size(158, 159);
            this.attitudeIndicatorInstrumentControl1.TabIndex = 31;
            this.attitudeIndicatorInstrumentControl1.Text = "attitudeIndicatorInstrumentControl1";
            // 
            // ArdupilotSim
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1055, 784);
            this.Controls.Add(this.CHKgraphthrottle);
            this.Controls.Add(this.CHKgraphrudder);
            this.Controls.Add(this.CHKgraphpitch);
            this.Controls.Add(this.CHKgraphroll);
            this.Controls.Add(this.CHKdisplayall);
            this.Controls.Add(this.Googleearthpanel);
            this.Controls.Add(this.headingIndicatorInstrumentControl1);
            this.Controls.Add(this.altimeterInstrumentControl1);
            this.Controls.Add(this.airSpeedIndicatorInstrumentControl1);
            this.Controls.Add(this.attitudeIndicatorInstrumentControl1);
            this.Controls.Add(this.label26);
            this.Controls.Add(this.panel6);
            this.Controls.Add(this.zg1);
            this.Controls.Add(this.panel5);
            this.Controls.Add(this.label17);
            this.Controls.Add(this.panel4);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.RAD_softFlightGear);
            this.Controls.Add(this.RAD_softXplanes);
            this.Controls.Add(this.SaveSettings);
            this.Controls.Add(this.OutputLog);
            this.Controls.Add(this.GPSrate);
            this.Controls.Add(this.CHKREV_rudder);
            this.Controls.Add(this.CHKREV_pitch);
            this.Controls.Add(this.CHKREV_roll);
            this.Name = "ArdupilotSim";
            this.Text = "ArdupilotSim by Michael Oborne V0.5";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ArdupilotSim_FormClosing);
            this.Load += new System.EventHandler(this.ArdupilotSim_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.panel4.ResumeLayout(false);
            this.panel4.PerformLayout();
            this.panel5.ResumeLayout(false);
            this.panel6.ResumeLayout(false);
            this.panel6.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox Comports;
        private System.Windows.Forms.CheckBox CHKREV_roll;
        private System.Windows.Forms.CheckBox CHKREV_pitch;
        private System.Windows.Forms.CheckBox CHKREV_rudder;
        private System.Windows.Forms.ComboBox GPSrate;
        private System.Windows.Forms.Button ConnectComPort;
        private System.Windows.Forms.RichTextBox OutputLog;
        private System.Windows.Forms.TextBox TXT_roll;
        private System.Windows.Forms.TextBox TXT_pitch;
        private System.Windows.Forms.TextBox TXT_heading;
        private System.Windows.Forms.TextBox TXT_wpdist;
        private System.Windows.Forms.TextBox TXT_bererror;
        private System.Windows.Forms.TextBox TXT_alterror;
        private System.Windows.Forms.TextBox TXT_lat;
        private System.Windows.Forms.TextBox TXT_long;
        private System.Windows.Forms.TextBox TXT_alt;
        private System.Windows.Forms.Button SaveSettings;
        private System.Windows.Forms.RadioButton RAD_softXplanes;
        private System.Windows.Forms.RadioButton RAD_softFlightGear;
        private System.Windows.Forms.TextBox TXT_servoroll;
        private System.Windows.Forms.TextBox TXT_servopitch;
        private System.Windows.Forms.TextBox TXT_servorudder;
        private System.Windows.Forms.TextBox TXT_servothrottle;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.TextBox TXT_WP;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Panel panel5;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.TextBox TXT_control_mode;
        private ZedGraph.ZedGraphControl zg1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Panel panel6;
        private System.Windows.Forms.TextBox TXT_ruddergain;
        private System.Windows.Forms.TextBox TXT_pitchgain;
        private System.Windows.Forms.TextBox TXT_rollgain;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.TextBox TXT_throttlegain;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.Label label26;
        private AvionicsInstrumentControlDemo.AttitudeIndicatorInstrumentControl attitudeIndicatorInstrumentControl1;
        private AvionicsInstrumentControlDemo.AirSpeedIndicatorInstrumentControl airSpeedIndicatorInstrumentControl1;
        private AvionicsInstrumentControlDemo.AltimeterInstrumentControl altimeterInstrumentControl1;
        private AvionicsInstrumentControlDemo.HeadingIndicatorInstrumentControl headingIndicatorInstrumentControl1;
        private System.Windows.Forms.Panel Googleearthpanel;
        private System.Windows.Forms.CheckBox CHKdisplayall;
        private System.Windows.Forms.CheckBox CHKgraphroll;
        private System.Windows.Forms.CheckBox CHKgraphpitch;
        private System.Windows.Forms.CheckBox CHKgraphrudder;
        private System.Windows.Forms.CheckBox CHKgraphthrottle;
    }
}

