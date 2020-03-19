using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Threading;
using System.Windows.Threading;

namespace cs2_chs
{
    /// <summary>
    /// Advance.xaml 的交互逻辑
    /// </summary>
    public partial class Advance : Window
    {
        public bool enChanged = false;
        static public Thread threadRestore;
        static bool enFaileden = false;
        static public void TranSpleteProc(ref string itoc)
        {
            string pitoc = itoc;
          

            if (enFaileden)
            {
                itoc = "";
                char lob = '\0';
                foreach (char t in pitoc)
                {
                    if (t != lob)
                    {
                        itoc += t;
                        lob = t;
                    }
                }
            }
            else
            {
                return;
            }
        }

        static public void UnTranSpleteProc(ref string itoc)
        {
            string pitoc = itoc;


            if (enFaileden)
            {
                itoc = "";
                foreach (char t in pitoc)
                {
                    itoc += t;
                    itoc += t;
                }
            }
            else
            {
                return;
            }
        }
        public Advance()
        {
            InitializeComponent();
            REASE.IsChecked = false;
            this.Closing += Window_Closing;
            Thread threadExit = new Thread(delegate ()
            {
                while (true)
                {
                    unsafe
                    {
                        this.Dispatcher.Invoke(DispatcherPriority.Normal, (ThreadStart)delegate ()
                        {
                            if (MainWindow.nID != 0)
                                IDnPut.Text = Convert.ToString(*(int*)MainWindow.nID, 10);
                        });
                        Thread.Sleep(100);
                    }
                }
            });
            threadExit.Start();

            threadRestore = new Thread(delegate ()
            {
                unsafe
                {

                    while (true)
                    {

                        string loacl = new string((char*)MainWindow.ns_str);
                        string lobnob = loacl;

                        TranSpleteProc(ref lobnob);
                        this.Dispatcher.Invoke(DispatcherPriority.Normal, (ThreadStart)delegate ()
                        {
                            TRAN_OPT.Text = lobnob;
                            OutPutLog.Text = loacl;
                        });
                        Thread.Sleep(50);
                    }
                }
            });

        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            e.Cancel = true;
            this.Hide();
        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (!enChanged)
                return;
            try
            {
                uint test = Convert.ToUInt32(AddressEdit.Text, 16);
                MainWindow.initdata.StartUp = StartUpEdit.Text;
                MainWindow.initdata.Addr = Convert.ToUInt32(AddressEdit.Text, 16);
                MainWindow.initdata.Envio = EnvioMode.IsChecked == true ? true : false;
                AddressEdit.Foreground = new SolidColorBrush(Colors.Black);
                StartUpEdit.Foreground = new SolidColorBrush(Colors.Black);
                EnvioMode.Foreground = new SolidColorBrush(Colors.Black);
            }
            catch(Exception err)
            {
               string msg="Error!\nPlease check if the expression that you entered is valid.\n"+ err.ToString();
                MessageBox.Show(msg, "error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }
            if (MainWindow.initdata.Envio)
            {
                OM_GPY.IsEnabled = true;
                OM_TOT.IsEnabled = true;
                OutPutLog.IsEnabled = true;
                MainWindow.EndReplace();
                MainWindow.thisPfc.SRC_OUTPUT.IsReadOnly = false;
                unsafe
                {
                    (*(bool*)MainWindow.pblockRestoreSrc) = true;
                }
            }
            else
            {
                OM_GPY.IsEnabled = false;
                OM_TOT.IsEnabled = false;
                OutPutLog.IsEnabled = false;
                MainWindow.StartReplace();
                MainWindow.thisPfc.SRC_OUTPUT.IsReadOnly = true;
                unsafe
                {
                    (*(bool*)MainWindow.pblockRestoreSrc) = false;
                }
            }
            MessageBox.Show("👴知道🌶！\n🍋の🐍☞已经应用🌶！\n暴力提取之外的设置将会在下次启动时按照更改后的设置工作。");
        }
        private void AddressEdit_TextChanged(object sender, TextChangedEventArgs e)
        {
            AddressEdit.Foreground = new SolidColorBrush(Colors.Red);
            enChanged = true;
        }
        private void StartUpEdit_TextChanged(object sender, TextChangedEventArgs e)
        {
            StartUpEdit.Foreground= new SolidColorBrush(Colors.Red);
            enChanged = true;
        }
        private void EnvioMode_Click(object sender, RoutedEventArgs e)
        {
            EnvioMode.Foreground = new SolidColorBrush(Colors.Red);
            enChanged = true;
        }

        private void OM_GPY_Checked(object sender, RoutedEventArgs e)
        {
            if (MainWindow.initdata.VioMode == 0)
                return;
            MainWindow.initdata.VioMode = 0;
            unsafe
            {
                (*(int*)MainWindow.ppMode) = MainWindow.initdata.VioMode;
            }
            MainWindow.ChangeTToG();
        }

        private void OM_TOT_Checked(object sender, RoutedEventArgs e)
        {
            if (MainWindow.initdata.VioMode == 1)
                return;
            MainWindow.initdata.VioMode = 1;
            unsafe
            {
                (*(int*)MainWindow.ppMode) = MainWindow.initdata.VioMode;
            }

          
            MainWindow.ChangeGToT();
        }

        private void Window_MouseMove(object sender, MouseEventArgs e)
        {
            unsafe
            {
             
            }
        }

        private void CheckBox_Checked(object sender, RoutedEventArgs e)
        {
            enFaileden = true;
        }

        private void REASE_Unchecked(object sender, RoutedEventArgs e)
        {
            enFaileden = false;
        }
    }
}
