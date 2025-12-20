/*
 * Created by SharpDevelop.
 * User: ardayavuz
 * Date: 4.12.2025
 * Time: 14:53
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
namespace bb3
{
	partial class MainForm
	{
		/// <summary>
		/// Designer variable used to keep track of non-visual components.
		/// </summary>
		private System.ComponentModel.IContainer components = null;
		
		/// <summary>
		/// Disposes resources used by the form.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing) {
				if (components != null) {
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}
		
		/// <summary>
		/// This method is required for Windows Forms designer support.
		/// Do not change the method contents inside the source code editor. The Forms designer might
		/// not be able to load this method if it was changed manually.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
			this.COMname = new System.Windows.Forms.Label();
			this.textBoxPORTNO = new System.Windows.Forms.TextBox();
			this.buttonPORTAC = new System.Windows.Forms.Button();
			this.buttonportkapat = new System.Windows.Forms.Button();
			this.label2 = new System.Windows.Forms.Label();
			this.texboxmikromejsi = new System.Windows.Forms.TextBox();
			this.buttometinGondern = new System.Windows.Forms.Button();
			this.baundrate = new System.Windows.Forms.Label();
			this.textBoxBaundrate = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.s = new System.Windows.Forms.Label();
			this.textBoxADCdeger = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.textBoxSaat = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// serialPort1
			// 
			this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.SerialPort1DataReceived);
			// 
			// COMname
			// 
			this.COMname.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.COMname.Location = new System.Drawing.Point(19, 42);
			this.COMname.Margin = new System.Windows.Forms.Padding(10, 0, 10, 0);
			this.COMname.Name = "COMname";
			this.COMname.Size = new System.Drawing.Size(202, 41);
			this.COMname.TabIndex = 0;
			this.COMname.Text = "COM No:";
			// 
			// textBoxPORTNO
			// 
			this.textBoxPORTNO.Location = new System.Drawing.Point(214, 42);
			this.textBoxPORTNO.Name = "textBoxPORTNO";
			this.textBoxPORTNO.Size = new System.Drawing.Size(160, 53);
			this.textBoxPORTNO.TabIndex = 1;
			this.textBoxPORTNO.Text = "COM4";
			// 
			// buttonPORTAC
			// 
			this.buttonPORTAC.BackColor = System.Drawing.SystemColors.Window;
			this.buttonPORTAC.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.buttonPORTAC.Location = new System.Drawing.Point(430, 36);
			this.buttonPORTAC.Name = "buttonPORTAC";
			this.buttonPORTAC.Size = new System.Drawing.Size(212, 114);
			this.buttonPORTAC.TabIndex = 2;
			this.buttonPORTAC.Text = "Bağlan ";
			this.buttonPORTAC.UseVisualStyleBackColor = false;
			this.buttonPORTAC.Click += new System.EventHandler(this.ButtonPORTACClick);
			// 
			// buttonportkapat
			// 
			this.buttonportkapat.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.buttonportkapat.Location = new System.Drawing.Point(666, 36);
			this.buttonportkapat.Name = "buttonportkapat";
			this.buttonportkapat.Size = new System.Drawing.Size(212, 110);
			this.buttonportkapat.TabIndex = 3;
			this.buttonportkapat.Text = "Bağlantıyı Kes";
			this.buttonportkapat.UseVisualStyleBackColor = true;
			this.buttonportkapat.Click += new System.EventHandler(this.ButtonportkapatClick);
			// 
			// label2
			// 
			this.label2.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.label2.Location = new System.Drawing.Point(104, 188);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(117, 53);
			this.label2.TabIndex = 4;
			this.label2.Text = "Veri:";
			// 
			// texboxmikromejsi
			// 
			this.texboxmikromejsi.Location = new System.Drawing.Point(214, 188);
			this.texboxmikromejsi.Name = "texboxmikromejsi";
			this.texboxmikromejsi.Size = new System.Drawing.Size(199, 53);
			this.texboxmikromejsi.TabIndex = 5;
			// 
			// buttometinGondern
			// 
			this.buttometinGondern.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.buttometinGondern.Location = new System.Drawing.Point(430, 188);
			this.buttometinGondern.Name = "buttometinGondern";
			this.buttometinGondern.Size = new System.Drawing.Size(212, 114);
			this.buttometinGondern.TabIndex = 6;
			this.buttometinGondern.Text = "Gönder";
			this.buttometinGondern.UseVisualStyleBackColor = true;
			this.buttometinGondern.Click += new System.EventHandler(this.ButtometinGondernClick);
			// 
			// baundrate
			// 
			this.baundrate.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.baundrate.Location = new System.Drawing.Point(19, 120);
			this.baundrate.Name = "baundrate";
			this.baundrate.Size = new System.Drawing.Size(225, 52);
			this.baundrate.TabIndex = 7;
			this.baundrate.Text = "Baudrate:";
			// 
			// textBoxBaundrate
			// 
			this.textBoxBaundrate.Location = new System.Drawing.Point(214, 120);
			this.textBoxBaundrate.Name = "textBoxBaundrate";
			this.textBoxBaundrate.Size = new System.Drawing.Size(160, 53);
			this.textBoxBaundrate.TabIndex = 8;
			this.textBoxBaundrate.Text = "9600";
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(0, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(100, 23);
			this.label1.TabIndex = 0;
			// 
			// s
			// 
			this.s.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.s.Location = new System.Drawing.Point(23, 333);
			this.s.Name = "s";
			this.s.Size = new System.Drawing.Size(260, 64);
			this.s.TabIndex = 9;
			this.s.Text = "ADC Değeri:";
			// 
			// textBoxADCdeger
			// 
			this.textBoxADCdeger.Location = new System.Drawing.Point(289, 333);
			this.textBoxADCdeger.Multiline = true;
			this.textBoxADCdeger.Name = "textBoxADCdeger";
			this.textBoxADCdeger.Size = new System.Drawing.Size(290, 102);
			this.textBoxADCdeger.TabIndex = 10;
			// 
			// label3
			// 
			this.label3.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.label3.Location = new System.Drawing.Point(654, 333);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(158, 53);
			this.label3.TabIndex = 11;
			this.label3.Text = "Saat:";
			// 
			// textBoxSaat
			// 
			this.textBoxSaat.Location = new System.Drawing.Point(762, 330);
			this.textBoxSaat.Name = "textBoxSaat";
			this.textBoxSaat.Size = new System.Drawing.Size(290, 53);
			this.textBoxSaat.TabIndex = 12;
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(23F, 46F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.Info;
			this.ClientSize = new System.Drawing.Size(1277, 483);
			this.Controls.Add(this.textBoxSaat);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.textBoxADCdeger);
			this.Controls.Add(this.s);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.textBoxBaundrate);
			this.Controls.Add(this.baundrate);
			this.Controls.Add(this.buttometinGondern);
			this.Controls.Add(this.texboxmikromejsi);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.buttonportkapat);
			this.Controls.Add(this.buttonPORTAC);
			this.Controls.Add(this.textBoxPORTNO);
			this.Controls.Add(this.COMname);
			this.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
			this.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.Margin = new System.Windows.Forms.Padding(10, 9, 10, 9);
			this.Name = "MainForm";
			this.Text = "bb3";
			this.Load += new System.EventHandler(this.MainFormLoad);
			this.ResumeLayout(false);
			this.PerformLayout();
		}
		private System.Windows.Forms.TextBox textBoxSaat;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox textBoxADCdeger;
		private System.Windows.Forms.Label s;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button buttometinGondern;
		private System.Windows.Forms.TextBox texboxmikromejsi;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Button buttonportkapat;
		private System.Windows.Forms.Button buttonPORTAC;
		private System.Windows.Forms.TextBox textBoxPORTNO;
		private System.Windows.Forms.Label COMname;
		private System.IO.Ports.SerialPort serialPort1;
		private System.Windows.Forms.Label baundrate;
		private System.Windows.Forms.TextBox textBoxBaundrate;
	}
}
