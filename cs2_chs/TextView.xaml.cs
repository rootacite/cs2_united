using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace cs2_chs
{
    /// <summary>
    /// TextView.xaml 的交互逻辑
    /// </summary>
    public partial class TextView : Window
    {
        [DllImport("user32", EntryPoint = "SetWindowLong")]
        private static extern uint SetWindowLong(IntPtr hwnd, int nIndex, int NewLong);
        
        public TextView()
        {
            InitializeComponent();
            Show();
            IntPtr hwnd = new WindowInteropHelper(this).Handle;
        //    SetWindowLong(hwnd, (-20), 0x20);
            Hide();

            this.Top = MainWindow.initdata.Loct.Y;
            this.Left = MainWindow.initdata.Loct.X;
            this.Height = MainWindow.initdata.Loct.Height;
            this.Width = MainWindow.initdata.Loct.Width;
        }

        private void Grid_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (Keyboard.IsKeyDown(Key.LeftAlt))
            {
                this.DragMove();
            }
        }

        private void textbar_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key != Key.LeftAlt)
                return;
            textbar.Background = new SolidColorBrush(Colors.Pink);
            IntPtr hwnd = new WindowInteropHelper(this).Handle;
        //    SetWindowLong(hwnd, (-20), ~0x20);

        }

        private void textbar_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key != Key.LeftAlt)
                return;
            textbar.Background = new SolidColorBrush(Colors.Transparent);
            IntPtr hwnd = new WindowInteropHelper(this).Handle;
       //     SetWindowLong(hwnd, (-20), 0x20);
        }

        private void Grid_MouseUp(object sender, MouseButtonEventArgs e)
        {
            LocationData bData;
            bData.X = (int)Left;
            bData.Y = (int)Top;
            bData.Height = (int)Height;
            bData.Width = (int)Width;

            MainWindow.initdata.Loct = bData;
        }

        private void Grid_MouseWheel(object sender, MouseWheelEventArgs e)
        {
         
        }
    }
}
