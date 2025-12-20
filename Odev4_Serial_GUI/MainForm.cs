/*
 * Created by SharpDevelop.
 * User: ardayavuz
 * Date: 4.12.2025
 * Time: 14:53
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace bb3
{
	/// <summary>
	/// Description of MainForm.
	/// </summary>
	public partial class MainForm : Form
	{
		string gelenveri = "";
		
		public MainForm()
		{
			//
			// The InitializeComponent() call is required for Windows Forms designer support.
			//
			InitializeComponent();
			
			
			
			//
			// TODO: Add constructor code after the InitializeComponent() call.
			//
		}
		
		void ButtonPORTACClick(object sender, EventArgs e)
		{
			if(!serialPort1.IsOpen)
			{
				try
				{
					serialPort1.PortName = textBoxPORTNO.Text;
					serialPort1.BaudRate = Convert.ToInt32(textBoxBaundrate.Text);
					serialPort1.Open();
					MessageBox.Show("Bağlantı Başarılı!");
				}
				catch
				{
					MessageBox.Show("Bağlantı Başarısız!");
				}
			}
		}
		
		void ButtonportkapatClick(object sender, EventArgs e)
		{
			if(serialPort1.IsOpen)
			{
				serialPort1.Close();
				MessageBox.Show("Bağlantı Kesildi!");
			}
		}
		
		void ButtometinGondernClick(object sender, EventArgs e)
		{
			
			string saat = texboxmikromejsi.Text;
				serialPort1.WriteLine(saat);
				
		}	
		
		
		void SerialPort1DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
		{
			try
			{
				gelenveri = serialPort1.ReadLine();
				this.Invoke(new MethodInvoker(DisplayText));
			}
			catch
			{
				
			}
		}
		
		private void DisplayText()
		{
			if (string.IsNullOrEmpty(gelenveri)) return;
			
			if (gelenveri.StartsWith("T"))
			{
				string sayi = gelenveri.Substring(1);
				textBoxSaat.Text = "SAAT: " + sayi;
			}
			else if (gelenveri.StartsWith("D"))
			{
				string sayi = gelenveri.Substring(1);
				textBoxADCdeger.Text = "SICAKLIK: " + sayi;
			}
			
		}
			
		
		void MainFormLoad(object sender, EventArgs e)
		{
	
		}
		

		}
	}

