using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using System.Windows.Threading;
using System.Messaging;
using System.Windows.Interop;

namespace cs2_chs
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    /// 
    public partial class MainWindow : Window
    {
        //TestA
        public static MainWindow thisPfc;

      
        [DllImport("Kernel32.dll", EntryPoint = "WaitOnAddress")]
        public extern static bool WaitOnAddress(uint Address, uint CompareAddress, uint AddressSize, uint dwMilliseconds);
        [DllImport("Kernel32.dll", EntryPoint = "WakeByAddressSingle")]
        public extern static void WakeByAddressSingle(uint Address);

        [DllImport("Kernel32.dll", EntryPoint = "WaitForSingleObject")]
        public extern static int WaitForSingleObject(uint hHandle, uint dwMilliseconds);

        [DllImport("cs2_patch.dll", EntryPoint = "InjectSelfTo")]
        public static extern uint InjectSelfTo(string path);

        [DllImport("cs2_patch.dll", EntryPoint = "CreateDataExport")]
        public static extern void CreateData([MarshalAs(UnmanagedType.LPWStr)] string src,[MarshalAs(UnmanagedType.LPWStr)] string path);
        [DllImport("Kernel32.dll", EntryPoint = "TerminateProcess")]
        public static extern bool TerminateProcess(uint hThread, uint dwExitCode);
        [DllImport("Kernel32.dll", EntryPoint = "OpenProcess")]
        public static extern uint OpenProcess(uint dwDesiredAccess, bool bInheritHandle, uint dwProcessId);
        [DllImport("Kernel32.dll", EntryPoint = "GetCurrentProcess")]
        public static extern uint GetCurrentProcess();
        [DllImport("user32", EntryPoint = "SetWindowLong")]
        private static extern uint SetWindowLong(IntPtr hwnd, int nIndex, int NewLong);
        public static uint pSaveProcess = 0;
        public  static uint hThread = 0;
        public static uint ms_str = 0;
        public static uint ptPid = 0;
        public  static uint ppMode;
        public static uint nID;
        public static uint cn_str = 0;
        public static uint IsSuccess = 0;
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if(this.Visibility != Visibility.Visible){
                e.Cancel = false;
                return;
            }
            MessageBoxResult res = MessageBox.Show("是否要不关闭游戏，而只隐藏文本编辑窗口？", "确认", MessageBoxButton.YesNoCancel);
            if (res == MessageBoxResult.Yes)
            {
                e.Cancel = true;
                this.ShowInTaskbar = false;
                this.Visibility = Visibility.Hidden;
            }
            else if (res == MessageBoxResult.No)
            {
                e.Cancel = false;
                unsafe
                {
                    TerminateProcess(OpenProcess(0x0001, false, *(uint*)ptPid), 1);
                    TerminateProcess(GetCurrentProcess(), 1);
                }
            }
            else if (res == MessageBoxResult.Cancel)
            {
                e.Cancel = true;
            }
        }
        public MainWindow()
        {
            InitializeComponent();
            this.Closing += Window_Closing;
        }
        private void Grid_Loaded(object sender, RoutedEventArgs e)
        {
      //      MessageBox.Show("");
            // char[] a = { '1', '2', '3' };
         //   MessageBox.Show("");
      
        
            thisPfc = this;

      //      MessageBox.Show("zero");
            hThread = InjectSelfTo("ExHIBIT.exe");//VioMode
            int hMod = DllTools.GetModuleHandleA("cs2_patch.dll");
        //    MessageBox.Show("");
            if (hMod == 0)
                MessageBox.Show("error");
         //   MessageBox.Show("");
            pSaveProcess = (uint)DllTools.GetProcAddress(hMod, "saveProcess");
            ms_str = (uint)DllTools.GetProcAddress(hMod, "ms_str");
            ptPid = (uint)DllTools.GetProcAddress(hMod, "tPid");
            nID= (uint)DllTools.GetProcAddress(hMod, "nID");
            cn_str = (uint)DllTools.GetProcAddress(hMod, "cn_str");
            IsSuccess= (uint)DllTools.GetProcAddress(hMod, "IsSuccess");


            Thread threadExit = new Thread(delegate ()
            {
                WaitForSingleObject(hThread, 0xFFFFFFFF);
                this.Dispatcher.Invoke(DispatcherPriority.Normal, (ThreadStart)delegate ()
                {
                    TerminateProcess(GetCurrentProcess(), 1);
                });
            });
            threadExit.Start();

            Thread thread3 = new Thread(delegate ()
            {
                while (true)
                {
                  
                    unsafe
                    {
                       
                        char* pms_str = (char*)ms_str;
                        string MsStr = new string(pms_str);
                        this.Dispatcher.Invoke(DispatcherPriority.Normal, (ThreadStart)delegate ()
                        {
                            if (*(bool*)IsSuccess)
                            {
                              //  MessageBox.Show("");
                                TEXT_INPUT.BorderBrush = new SolidColorBrush(Colors.Green);
                            }
                            else
                            {
                                TEXT_INPUT.BorderBrush = new SolidColorBrush(Colors.Red);
                            }
                            SRC_OUTPUT.Text = MsStr;
                        });
                    }
                    Thread.Sleep(50);
                }
            });
            thread3.Start();
           // MessageBox.Show("");
        }
        
        private void Grid_MouseUp(object sender, MouseButtonEventArgs e)
        {
           // this.Visibility = Visibility.Visible;
        }
        private void Grid_MouseDown(object sender, MouseButtonEventArgs e)
        {
        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Thread thread1 = new Thread(delegate ()
            {
                string LocalS = "";
                string LocalP = "";
                this.Dispatcher.Invoke(DispatcherPriority.Normal, (ThreadStart)delegate ()
                {
                    LocalS = TEXT_INPUT.Text;
                    LocalP = SRC_OUTPUT.Text;
                });

                CreateData(LocalP, LocalS);
            });

            Thread thread2 = new Thread(delegate ()
            {
                unsafe
                {
                    double* saveProcess = (double*)pSaveProcess;
                    this.Dispatcher.Invoke(DispatcherPriority.Normal, (ThreadStart)delegate ()
                    {
                        apply.IsEnabled = false;
                        PBS.Visibility = Visibility.Visible;
                    });
                    *saveProcess = 0;
                    while (*saveProcess != 1)
                    {
                        this.Dispatcher.Invoke(DispatcherPriority.Normal, (ThreadStart)delegate ()
                        {
                            PBS.Value = ((*saveProcess) * 100.0);
                        });
                        Thread.Sleep(15);
                    }
                    this.Dispatcher.Invoke(DispatcherPriority.Normal, (ThreadStart)delegate ()
                    {
                        apply.IsEnabled = true;
                        PBS.Visibility = Visibility.Collapsed;
                    });
                    *saveProcess = 0;
                }
            });
            thread2.Start();
            thread1.Start();
        }
        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            unsafe
            {
                char* pcn_str = (char*)cn_str;
                string MsStr = new string(pcn_str);
             
                TEXT_INPUT.Text = MsStr;
            }
        }
        private void CommandBinding_ShowMainWindow_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
           // AdvanceSetting.
            e.CanExecute = true;
        }
        private void CommandBinding_ShowMainWindow_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            
        }
        private void Grid_Unloaded(object sender, RoutedEventArgs e)
        {
        //    MessageBox.Show("");
        }

        private void Button_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            unsafe
            {
                char* pcn_str = (char*)ms_str;
                string MsStr = new string(pcn_str);
                TEXT_INPUT.Text = MsStr;

                string BMS = "";
                foreach (char i in MsStr)
                    if (i != '\n') BMS += i;
                Clipboard.SetDataObject(BMS);
            }
        }

        private void SRC_OUTPUT_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {

        }

        private void TEXT_INPUT_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            string data = Clipboard.GetText();
            TEXT_INPUT.Text = data;

            e.Handled = true;
        }

        private void TEXT_INPUT_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            e.Handled = true;
        }
    }
}
