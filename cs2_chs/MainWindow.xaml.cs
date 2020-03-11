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
using System.Windows.Navigation;
using System.Windows.Shapes;

using System.Runtime.InteropServices;
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
        public static string UTF8To16(string str)
        {
            string res;
            int i, len, c;
            int char2, char3;
            res = "";
            len = str.Length;
            i = 0;
            while (i < len)
            {
                c = Convert.ToByte(str[i++]);
                switch (c >> 4)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                        // 0xxxxxxx
                        res += str.CharAt(i - 1);
                        break;
                    case 12:
                    case 13:
                        // 110x xxxx 10xx xxxx
                        char2 = Convert.ToByte(str[i++]);
                        res += Convert.ToChar(((c & 0x1F) << 6) | (char2 & 0x3F));
                        break;
                    case 14:
                        // 1110 xxxx 10xx xxxx 10xx xxxx
                        char2 = Convert.ToByte(str[i++]);
                        char3 = Convert.ToByte(str[i++]);
                        res += Convert.ToChar(((c & 0x0F) << 12) |
                            ((char2 & 0x3F) << 6) |
                            ((char3 & 0x3F) << 0));
                        break;
                }
            }
            return res;
        }
        [DllImport("cs2_patch.dll", EntryPoint = "InjectSelfTo")]
        public static extern int pStart(string path);
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Grid_Loaded(object sender, RoutedEventArgs e)
        {
           // char[] a = { '1', '2', '3' };
            pStart("cs2.exe");
        }
    }
}
