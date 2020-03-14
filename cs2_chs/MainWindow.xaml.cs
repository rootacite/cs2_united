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

namespace cs2_chs
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    /// 

    public static class StrMop
    {
        public static char CharAt(this string str, int index)
        {
            if (index > str.Length)
                return ' ';

            string res = str.Substring(index, 1);
            return Convert.ToChar(res);
        }
    }
    public partial class MainWindow : Window
    {
        [DllImport("Kernel32.dll", EntryPoint = "WaitForSingleObject")]
        public extern static int WaitForSingleObject(uint hHandle, uint dwMilliseconds);

        [DllImport("cs2_patch.dll", EntryPoint = "InjectSelfTo")]
        public static extern uint pStart(string path);

        [DllImport("cs2_patch.dll", EntryPoint = "CreateDataExport")]
        public static extern void CreateData([MarshalAs(UnmanagedType.LPWStr)] string path);
        [DllImport("Kernel32.dll", EntryPoint = "TerminateProcess")]
        public static extern bool TerminateProcess(uint hThread, uint dwExitCode);
        [DllImport("Kernel32.dll", EntryPoint = "OpenProcess")]
        public static extern uint OpenProcess(uint dwDesiredAccess, bool bInheritHandle, uint dwProcessId);

        int pSaveProcess = 0;
        uint hThread = 0;
        int ms_str = 0;
        int ptPid = 0;

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
            hThread = pStart("cs2.exe");
            int hMod = DllTools.GetModuleHandleA("cs2_patch.dll");
            if (hMod == 0)
                MessageBox.Show("error");
            pSaveProcess = DllTools.GetProcAddress(hMod, "saveProcess");
            ms_str = DllTools.GetProcAddress(hMod, "ms_str");
            ptPid = DllTools.GetProcAddress(hMod, "tPid");
            Thread threadExit = new Thread(delegate ()
            {
                WaitForSingleObject(hThread, 0xFFFFFFFF);
                this.Dispatcher.Invoke(DispatcherPriority.Normal, (ThreadStart)delegate ()
                {
                    this.Close();
                });
            });
            threadExit.Start();
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
                this.Dispatcher.Invoke(DispatcherPriority.Normal, (ThreadStart)delegate ()
                {
                    LocalS = TEXT_INPUT.Text;
                });
                CreateData(LocalS);
            });
            thread1.Start();

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
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            unsafe
            {
                char* pms_str = (char*)ms_str;

                string MsStr = new string(pms_str);
                TEXT_INPUT.Text = MsStr;
            }
        }

        private void CommandBinding_ShowMainWindow_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = false;
        }

        private void CommandBinding_ShowMainWindow_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            //this.Visibility = Visibility.Visible;
        }

        private void Grid_Unloaded(object sender, RoutedEventArgs e)
        {
        //    MessageBox.Show("");
        }
    }
}
