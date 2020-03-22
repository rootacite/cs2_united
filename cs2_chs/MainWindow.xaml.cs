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
        public static InitData initdata = new InitData();//加载初始化信息
        public Advance AdvanceSetting = new Advance();//加载高级设置窗口
        public static MainWindow thisPfc;

        [DllImport("cs2_patch.dll", EntryPoint = "ChangeGToT")]
        public static extern void ChangeGToT();
        [DllImport("cs2_patch.dll", EntryPoint = "ChangeTToG")]
        public static extern void ChangeTToG();

        [DllImport("cs2_patch.dll", EntryPoint = "TestA")]
        public static extern void TestA();
        [DllImport("cs2_patch.dll", EntryPoint = "StartReplace")]
        public static extern void StartReplace();
        [DllImport("cs2_patch.dll", EntryPoint = "EndReplace")]
        public static extern void EndReplace();
        [DllImport("Kernel32.dll", EntryPoint = "WaitOnAddress")]
        public extern static bool WaitOnAddress(uint Address, uint CompareAddress, uint AddressSize, uint dwMilliseconds);
        [DllImport("Kernel32.dll", EntryPoint = "WakeByAddressSingle")]
        public extern static void WakeByAddressSingle(uint Address);

        [DllImport("Kernel32.dll", EntryPoint = "WaitForSingleObject")]
        public extern static int WaitForSingleObject(uint hHandle, uint dwMilliseconds);
        [DllImport("cs2_patch.dll", EntryPoint = "InjectSelfTo")]
        public static extern uint pStart([MarshalAs(UnmanagedType.LPStr)]  string path);
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
        public static uint ns_str = 0;
        public static uint ptPid = 0;
        public  static uint ppMode;
        public static uint nID;
        public static uint pblockRestoreSrc;
        public static uint cn_str = 0;
        public static uint IsSuccess = 0;
        public static uint enReplace = 0;
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
            // char[] a = { '1', '2', '3' };
           // MessageBox.Show("");
            TestA();
            int hMod = DllTools.GetModuleHandleA("cs2_patch.dll");
            thisPfc = this;
            if (!initdata.successedLoad)
            {
                MessageBox.Show("error:failed to load Init.xml!","Error!",MessageBoxButton.OK,MessageBoxImage.Error);
                TerminateProcess(GetCurrentProcess(), 1);
                return;
            }
            uint ppAddr = (uint)DllTools.GetProcAddress(hMod, "m_Addr");//m_Addr
            MainWindow.ppMode = (uint)DllTools.GetProcAddress(hMod, "VioMode");//m_Addr
            unsafe
            {
                (*(uint*)ppAddr) = initdata.Addr;
                (*(int*)ppMode) = initdata.VioMode;
            }

            AdvanceSetting.StartUpEdit.Text = initdata.StartUp;
            AdvanceSetting.StartUpEdit.Foreground = new SolidColorBrush(Colors.Black);
            AdvanceSetting.AddressEdit.Text = Convert.ToString(initdata.Addr, 16);
            AdvanceSetting.AddressEdit.Foreground = new SolidColorBrush(Colors.Black);
            AdvanceSetting.EnvioMode.IsChecked = initdata.Envio;
            AdvanceSetting.EnvioMode.Foreground = new SolidColorBrush(Colors.Black);

            pblockRestoreSrc = (uint)DllTools.GetProcAddress(hMod, "blockRestoreSrc");
            enReplace = (uint)DllTools.GetProcAddress(hMod, "enReplace");
            unsafe
            {
                (*(bool*)pblockRestoreSrc) = initdata.Envio;
                (*(bool*)enReplace) = initdata.EnRep;
            }

            AdvanceSetting.ENREP.IsChecked = initdata.EnRep;
            if (initdata.EnRep)
            {
                AdvanceSetting.tv.Show();
            }
            switch (initdata.VioMode)
            {
                case 0:
                    AdvanceSetting.OM_GPY.IsChecked = true;
                    break;
                case 1:
                    AdvanceSetting.OM_TOT.IsChecked = true;
                    break;
            }
           // MessageBox.Show("");
            AdvanceSetting.enChanged = false;

           // MessageBox.Show(initdata.StartUp);
            hThread = pStart(initdata.StartUp);//VioMode
            if (initdata.Envio)
            {
                AdvanceSetting.OM_GPY.IsEnabled = true;
                AdvanceSetting.OM_TOT.IsEnabled = true;
                AdvanceSetting.OutPutLog.IsEnabled = true;
                AdvanceSetting.IDnPut.IsEnabled = false;
                AdvanceSetting.ENREP.IsEnabled = true;
                SRC_OUTPUT.IsReadOnly = false;
                EndReplace();
            }
            else
            {
                AdvanceSetting.OM_GPY.IsEnabled = false;
                AdvanceSetting.OM_TOT.IsEnabled = false;
                AdvanceSetting.OutPutLog.IsEnabled = false;

                AdvanceSetting.IDnPut.IsEnabled = true;
                AdvanceSetting.ENREP.IsEnabled = false;

                StartReplace();
            }
            if (hMod == 0)
                MessageBox.Show("error");
            pSaveProcess = (uint)DllTools.GetProcAddress(hMod, "saveProcess");
            ms_str = (uint)DllTools.GetProcAddress(hMod, "ms_str");
            ns_str = (uint)DllTools.GetProcAddress(hMod, "ns_str");
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
                        while (*(bool*)pblockRestoreSrc)
                        {
                            // bool aloc = true;
                            //MessageBox.Show("");
                            //WaitOnAddress(pblockRestoreSrc, (uint)&aloc, sizeof(bool), 0xFFFFFFFF);
                            Thread.Sleep(10);
                        }
                       
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
           Advance.threadRestore.Start();
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
                if (initdata.Envio)
                {
                    Advance.UnTranSpleteProc(ref LocalP);
                }
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
            AdvanceSetting.Show();
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
