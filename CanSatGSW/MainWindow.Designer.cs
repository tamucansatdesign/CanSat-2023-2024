namespace CanSat_GSW
{
    partial class MainWindow
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainWindow));
            this.panel1 = new System.Windows.Forms.Panel();
            this.serialPorts = new System.Windows.Forms.ComboBox();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.loadButton = new System.Windows.Forms.Button();
            this.startTelemetry = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.outputName = new System.Windows.Forms.TextBox();
            this.saveButton = new System.Windows.Forms.Button();
            this.serialConnect = new System.Windows.Forms.Button();
            this.port = new System.Windows.Forms.Label();
            this.radioChecker = new System.Windows.Forms.Timer(this.components);
            this.statusBox = new System.Windows.Forms.TextBox();
            this.telemetryBox = new System.Windows.Forms.TextBox();
            this.graphPanel = new System.Windows.Forms.TableLayoutPanel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.label2 = new System.Windows.Forms.Label();
            this.panel3 = new System.Windows.Forms.Panel();
            this.label3 = new System.Windows.Forms.Label();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.pointingGraph = new CanSat_GSW.RealTimeChart();
            this.magGraph = new CanSat_GSW.RealTimeChart();
            this.accelGraph = new CanSat_GSW.RealTimeChart();
            this.gyroGraph = new CanSat_GSW.RealTimeChart();
            this.vbatGraph = new CanSat_GSW.RealTimeChart();
            this.tempGraph = new CanSat_GSW.RealTimeChart();
            this.satsGraph = new CanSat_GSW.RealTimeChart();
            this.posGraph = new CanSat_GSW.RealTimeChart();
            this.altGraph = new CanSat_GSW.RealTimeChart();
            this.panel1.SuspendLayout();
            this.graphPanel.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.BackColor = System.Drawing.SystemColors.InfoText;
            this.panel1.Controls.Add(this.serialPorts);
            this.panel1.Controls.Add(this.button2);
            this.panel1.Controls.Add(this.button1);
            this.panel1.Controls.Add(this.loadButton);
            this.panel1.Controls.Add(this.startTelemetry);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.outputName);
            this.panel1.Controls.Add(this.saveButton);
            this.panel1.Controls.Add(this.serialConnect);
            this.panel1.Controls.Add(this.port);
            this.panel1.Location = new System.Drawing.Point(0, -4);
            this.panel1.Margin = new System.Windows.Forms.Padding(0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1251, 47);
            this.panel1.TabIndex = 0;
            // 
            // serialPorts
            // 
            this.serialPorts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.serialPorts.FormattingEnabled = true;
            this.serialPorts.Location = new System.Drawing.Point(58, 15);
            this.serialPorts.Name = "serialPorts";
            this.serialPorts.Size = new System.Drawing.Size(69, 21);
            this.serialPorts.TabIndex = 8;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(318, 14);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(89, 23);
            this.button2.TabIndex = 7;
            this.button2.Text = "SIM ENABLE";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(274, 14);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(38, 23);
            this.button1.TabIndex = 6;
            this.button1.Text = "ST";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // loadButton
            // 
            this.loadButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.loadButton.Location = new System.Drawing.Point(1162, 16);
            this.loadButton.Name = "loadButton";
            this.loadButton.Size = new System.Drawing.Size(75, 24);
            this.loadButton.TabIndex = 5;
            this.loadButton.Text = "Load Data";
            this.loadButton.UseVisualStyleBackColor = true;
            this.loadButton.Click += new System.EventHandler(this.loadData);
            // 
            // startTelemetry
            // 
            this.startTelemetry.Location = new System.Drawing.Point(211, 14);
            this.startTelemetry.Name = "startTelemetry";
            this.startTelemetry.Size = new System.Drawing.Size(57, 23);
            this.startTelemetry.TabIndex = 1;
            this.startTelemetry.Text = "CX ON";
            this.startTelemetry.UseVisualStyleBackColor = true;
            this.startTelemetry.Click += new System.EventHandler(this.startTelemetry_Click);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.ForeColor = System.Drawing.SystemColors.ControlLightLight;
            this.label1.Location = new System.Drawing.Point(890, 23);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(78, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "CSV File Name";
            // 
            // outputName
            // 
            this.outputName.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.outputName.Location = new System.Drawing.Point(974, 19);
            this.outputName.Name = "outputName";
            this.outputName.Size = new System.Drawing.Size(100, 20);
            this.outputName.TabIndex = 2;
            // 
            // saveButton
            // 
            this.saveButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.saveButton.Location = new System.Drawing.Point(1080, 17);
            this.saveButton.Name = "saveButton";
            this.saveButton.Size = new System.Drawing.Size(75, 23);
            this.saveButton.TabIndex = 1;
            this.saveButton.Text = "Save Data";
            this.saveButton.UseVisualStyleBackColor = true;
            this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
            // 
            // serialConnect
            // 
            this.serialConnect.Location = new System.Drawing.Point(133, 14);
            this.serialConnect.Name = "serialConnect";
            this.serialConnect.Size = new System.Drawing.Size(72, 23);
            this.serialConnect.TabIndex = 1;
            this.serialConnect.Text = "Connect";
            this.serialConnect.UseVisualStyleBackColor = true;
            this.serialConnect.Click += new System.EventHandler(this.serialConnect_Click);
            // 
            // port
            // 
            this.port.AutoSize = true;
            this.port.ForeColor = System.Drawing.SystemColors.Control;
            this.port.Location = new System.Drawing.Point(3, 19);
            this.port.Name = "port";
            this.port.Size = new System.Drawing.Size(55, 13);
            this.port.TabIndex = 2;
            this.port.Text = "Serial Port";
            // 
            // radioChecker
            // 
            this.radioChecker.Enabled = true;
            this.radioChecker.Interval = 500;
            this.radioChecker.Tick += new System.EventHandler(this.checkRadio);
            // 
            // statusBox
            // 
            this.statusBox.AcceptsReturn = true;
            this.statusBox.AcceptsTab = true;
            this.statusBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.statusBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(28)))), ((int)(((byte)(28)))), ((int)(((byte)(28)))));
            this.statusBox.ForeColor = System.Drawing.SystemColors.Window;
            this.statusBox.Location = new System.Drawing.Point(0, 32);
            this.statusBox.Margin = new System.Windows.Forms.Padding(0);
            this.statusBox.Multiline = true;
            this.statusBox.Name = "statusBox";
            this.statusBox.Size = new System.Drawing.Size(312, 172);
            this.statusBox.TabIndex = 14;
            // 
            // telemetryBox
            // 
            this.telemetryBox.AcceptsReturn = true;
            this.telemetryBox.AcceptsTab = true;
            this.telemetryBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.telemetryBox.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(28)))), ((int)(((byte)(28)))), ((int)(((byte)(28)))));
            this.telemetryBox.ForeColor = System.Drawing.SystemColors.Window;
            this.telemetryBox.Location = new System.Drawing.Point(0, 32);
            this.telemetryBox.Margin = new System.Windows.Forms.Padding(0);
            this.telemetryBox.Multiline = true;
            this.telemetryBox.Name = "telemetryBox";
            this.telemetryBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.telemetryBox.Size = new System.Drawing.Size(624, 172);
            this.telemetryBox.TabIndex = 15;
            // 
            // graphPanel
            // 
            this.graphPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.graphPanel.BackColor = System.Drawing.Color.Magenta;
            this.graphPanel.ColumnCount = 4;
            this.graphPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.graphPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.graphPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.graphPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.graphPanel.Controls.Add(this.pointingGraph, 0, 2);
            this.graphPanel.Controls.Add(this.magGraph, 3, 1);
            this.graphPanel.Controls.Add(this.accelGraph, 2, 1);
            this.graphPanel.Controls.Add(this.gyroGraph, 1, 1);
            this.graphPanel.Controls.Add(this.vbatGraph, 0, 1);
            this.graphPanel.Controls.Add(this.tempGraph, 3, 0);
            this.graphPanel.Controls.Add(this.satsGraph, 2, 0);
            this.graphPanel.Controls.Add(this.posGraph, 1, 0);
            this.graphPanel.Controls.Add(this.panel2, 1, 2);
            this.graphPanel.Controls.Add(this.panel3, 2, 2);
            this.graphPanel.Controls.Add(this.altGraph, 0, 0);
            this.graphPanel.Location = new System.Drawing.Point(0, 43);
            this.graphPanel.Margin = new System.Windows.Forms.Padding(0);
            this.graphPanel.Name = "graphPanel";
            this.graphPanel.RowCount = 3;
            this.graphPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.graphPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            this.graphPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.graphPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.graphPanel.Size = new System.Drawing.Size(1251, 609);
            this.graphPanel.TabIndex = 4;
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.Black;
            this.panel2.Controls.Add(this.label2);
            this.panel2.Controls.Add(this.statusBox);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(312, 405);
            this.panel2.Margin = new System.Windows.Forms.Padding(0);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(312, 204);
            this.panel2.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label2.BackColor = System.Drawing.Color.Black;
            this.label2.ForeColor = System.Drawing.Color.White;
            this.label2.Location = new System.Drawing.Point(3, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(309, 32);
            this.label2.TabIndex = 15;
            this.label2.Text = "CanSat Status";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // panel3
            // 
            this.panel3.BackColor = System.Drawing.Color.Black;
            this.graphPanel.SetColumnSpan(this.panel3, 2);
            this.panel3.Controls.Add(this.label3);
            this.panel3.Controls.Add(this.telemetryBox);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel3.Location = new System.Drawing.Point(624, 405);
            this.panel3.Margin = new System.Windows.Forms.Padding(0);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(627, 204);
            this.panel3.TabIndex = 9;
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.label3.BackColor = System.Drawing.Color.Black;
            this.label3.ForeColor = System.Drawing.Color.White;
            this.label3.Location = new System.Drawing.Point(0, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(624, 32);
            this.label3.TabIndex = 16;
            this.label3.Text = "Telemetry Feed";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // serialPort
            // 
            this.serialPort.BaudRate = 115200;
            this.serialPort.DiscardNull = true;
            this.serialPort.ReadTimeout = 150;
            // 
            // pointingGraph
            // 
            this.pointingGraph.BackColor = System.Drawing.Color.Gainsboro;
            this.pointingGraph.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pointingGraph.Location = new System.Drawing.Point(0, 405);
            this.pointingGraph.Margin = new System.Windows.Forms.Padding(0);
            this.pointingGraph.Name = "pointingGraph";
            this.pointingGraph.Size = new System.Drawing.Size(312, 204);
            this.pointingGraph.TabIndex = 18;
            // 
            // magGraph
            // 
            this.magGraph.BackColor = System.Drawing.Color.Gainsboro;
            this.magGraph.Dock = System.Windows.Forms.DockStyle.Fill;
            this.magGraph.Location = new System.Drawing.Point(936, 202);
            this.magGraph.Margin = new System.Windows.Forms.Padding(0);
            this.magGraph.Name = "magGraph";
            this.magGraph.Size = new System.Drawing.Size(315, 203);
            this.magGraph.TabIndex = 17;
            // 
            // accelGraph
            // 
            this.accelGraph.BackColor = System.Drawing.Color.Gainsboro;
            this.accelGraph.Dock = System.Windows.Forms.DockStyle.Fill;
            this.accelGraph.Location = new System.Drawing.Point(624, 202);
            this.accelGraph.Margin = new System.Windows.Forms.Padding(0);
            this.accelGraph.Name = "accelGraph";
            this.accelGraph.Size = new System.Drawing.Size(312, 203);
            this.accelGraph.TabIndex = 16;
            // 
            // gyroGraph
            // 
            this.gyroGraph.BackColor = System.Drawing.Color.Gainsboro;
            this.gyroGraph.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gyroGraph.Location = new System.Drawing.Point(312, 202);
            this.gyroGraph.Margin = new System.Windows.Forms.Padding(0);
            this.gyroGraph.Name = "gyroGraph";
            this.gyroGraph.Size = new System.Drawing.Size(312, 203);
            this.gyroGraph.TabIndex = 15;
            // 
            // vbatGraph
            // 
            this.vbatGraph.BackColor = System.Drawing.Color.Gainsboro;
            this.vbatGraph.Dock = System.Windows.Forms.DockStyle.Fill;
            this.vbatGraph.Location = new System.Drawing.Point(0, 202);
            this.vbatGraph.Margin = new System.Windows.Forms.Padding(0);
            this.vbatGraph.Name = "vbatGraph";
            this.vbatGraph.Size = new System.Drawing.Size(312, 203);
            this.vbatGraph.TabIndex = 14;
            // 
            // tempGraph
            // 
            this.tempGraph.BackColor = System.Drawing.Color.Gainsboro;
            this.tempGraph.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tempGraph.Location = new System.Drawing.Point(936, 0);
            this.tempGraph.Margin = new System.Windows.Forms.Padding(0);
            this.tempGraph.Name = "tempGraph";
            this.tempGraph.Size = new System.Drawing.Size(315, 202);
            this.tempGraph.TabIndex = 13;
            // 
            // satsGraph
            // 
            this.satsGraph.BackColor = System.Drawing.Color.Gainsboro;
            this.satsGraph.Dock = System.Windows.Forms.DockStyle.Fill;
            this.satsGraph.Location = new System.Drawing.Point(624, 0);
            this.satsGraph.Margin = new System.Windows.Forms.Padding(0);
            this.satsGraph.Name = "satsGraph";
            this.satsGraph.Size = new System.Drawing.Size(312, 202);
            this.satsGraph.TabIndex = 12;
            // 
            // posGraph
            // 
            this.posGraph.BackColor = System.Drawing.Color.Gainsboro;
            this.posGraph.Dock = System.Windows.Forms.DockStyle.Fill;
            this.posGraph.Location = new System.Drawing.Point(312, 0);
            this.posGraph.Margin = new System.Windows.Forms.Padding(0);
            this.posGraph.Name = "posGraph";
            this.posGraph.Size = new System.Drawing.Size(312, 202);
            this.posGraph.TabIndex = 11;
            // 
            // altGraph
            // 
            this.altGraph.BackColor = System.Drawing.Color.Gainsboro;
            this.altGraph.Dock = System.Windows.Forms.DockStyle.Fill;
            this.altGraph.Location = new System.Drawing.Point(0, 0);
            this.altGraph.Margin = new System.Windows.Forms.Padding(0);
            this.altGraph.Name = "altGraph";
            this.altGraph.Size = new System.Drawing.Size(312, 202);
            this.altGraph.TabIndex = 10;
            // 
            // MainWindow
            // 
            this.BackColor = System.Drawing.SystemColors.WindowFrame;
            this.ClientSize = new System.Drawing.Size(1249, 651);
            this.Controls.Add(this.graphPanel);
            this.Controls.Add(this.panel1);
            this.DoubleBuffered = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainWindow";
            this.Text = "CanSat Ground Station";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainWindow_FormClosed);
            this.Leave += new System.EventHandler(this.MainWindow_Leave);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.graphPanel.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.ResumeLayout(false);

        }
        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label port;
        private System.Windows.Forms.Button serialConnect;
        private System.Windows.Forms.Button saveButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox outputName;
        private System.Windows.Forms.Button startTelemetry;
        private System.Windows.Forms.Timer radioChecker;
        private System.Windows.Forms.Button loadButton;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ComboBox serialPorts;
        private System.Windows.Forms.TextBox statusBox;
        private System.Windows.Forms.TextBox telemetryBox;
        private System.Windows.Forms.TableLayoutPanel graphPanel;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Label label3;
        private RealTimeChart posGraph;
        private RealTimeChart altGraph;
        private RealTimeChart pointingGraph;
        private RealTimeChart magGraph;
        private RealTimeChart accelGraph;
        private RealTimeChart gyroGraph;
        private RealTimeChart vbatGraph;
        private RealTimeChart tempGraph;
        private RealTimeChart satsGraph;
        private System.IO.Ports.SerialPort serialPort;
    }
}

