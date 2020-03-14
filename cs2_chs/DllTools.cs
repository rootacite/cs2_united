using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.IO;
using Microsoft.IdentityModel.Logging;

namespace cs2_chs
{
    class DllTools
    {
        [DllImport("kernel32.dll")]
        public static extern uint GetLastError();

        [DllImport("kernel32.dll", EntryPoint = "LoadLibraryEx", SetLastError = true)]
        public static extern int LoadLibraryEx(string lpFileName, IntPtr hReservedNull, LoadLibraryFlags dwFlags);

        [DllImport("Kernel32", EntryPoint = "GetProcAddress", SetLastError = true)]
        public static extern int GetProcAddress(int handle, string funcname);

        [DllImport("Kernel32", EntryPoint = "FreeLibrary", SetLastError = true)]
        private static extern int FreeLibrary(int handle);

        [DllImport("kernel32.dll", EntryPoint = "GetModuleHandleA", SetLastError = true)]
        public static extern int GetModuleHandleA(string lpFileName);
        public static Delegate GetFunctionAddress(int dllModule, string functionName, Type t)
        {
            int address = GetProcAddress(dllModule, functionName);
            if (address == 0)
                return null;
            else
                return Marshal.GetDelegateForFunctionPointer(new IntPtr(address), t);
        }


        public static Delegate GetDelegateFromIntPtr(IntPtr address, Type t)
        {
            if (address == IntPtr.Zero)
                return null;
            else
                return Marshal.GetDelegateForFunctionPointer(address, t);
        }


        public static Delegate GetDelegateFromIntPtr(int address, Type t)
        {
            if (address == 0)
                return null;
            else
                return Marshal.GetDelegateForFunctionPointer(new IntPtr(address), t);
        }


        public static int LoadSDK(string lpFileName)
        {
            if (File.Exists(lpFileName))
            {
                var hReservedNull = IntPtr.Zero;
                var dwFlags = LoadLibraryFlags.LOAD_WITH_ALTERED_SEARCH_PATH;

                var result = LoadLibraryEx(lpFileName, hReservedNull, dwFlags);

                var errCode = GetLastError();
                LogHelper.LogInformation($"LoadSDK Result:{result}, ErrorCode: {errCode}");

                return result;
            }
            return 0;
        }

        public static int ReleaseSDK(int handle)
        {
            try
            {
                if (handle > 0)
                {
                    LogHelper.LogInformation($"FreeLibrary handle：{handle}");
                    var result = FreeLibrary(handle);
                    var errCode = GetLastError();
                    LogHelper.LogInformation($"FreeLibrary Result:{result}, ErrorCode: {errCode}");
                    return 0;
                }
                return -1;
            }
            catch (Exception ex)
            {
                //LogHelper.LogException<Exception>(ex);
                return -1;
            }
        }

        public enum LoadLibraryFlags : uint
        {
            /// <summary>
            /// DONT_RESOLVE_DLL_REFERENCES
            /// </summary>
            DONT_RESOLVE_DLL_REFERENCES = 0x00000001,

            /// <summary>
            /// LOAD_IGNORE_CODE_AUTHZ_LEVEL
            /// </summary>
            LOAD_IGNORE_CODE_AUTHZ_LEVEL = 0x00000010,

            /// <summary>
            /// LOAD_LIBRARY_AS_DATAFILE
            /// </summary>
            LOAD_LIBRARY_AS_DATAFILE = 0x00000002,

            /// <summary>
            /// LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE
            /// </summary>
            LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE = 0x00000040,

            /// <summary>
            /// LOAD_LIBRARY_AS_IMAGE_RESOURCE
            /// </summary>
            LOAD_LIBRARY_AS_IMAGE_RESOURCE = 0x00000020,

            /// <summary>
            /// LOAD_LIBRARY_SEARCH_APPLICATION_DIR
            /// </summary>
            LOAD_LIBRARY_SEARCH_APPLICATION_DIR = 0x00000200,

            /// <summary>
            /// LOAD_LIBRARY_SEARCH_DEFAULT_DIRS
            /// </summary>
            LOAD_LIBRARY_SEARCH_DEFAULT_DIRS = 0x00001000,

            /// <summary>
            /// LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR
            /// </summary>
            LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR = 0x00000100,

            /// <summary>
            /// LOAD_LIBRARY_SEARCH_SYSTEM32
            /// </summary>
            LOAD_LIBRARY_SEARCH_SYSTEM32 = 0x00000800,

            /// <summary>
            /// LOAD_LIBRARY_SEARCH_USER_DIRS
            /// </summary>
            LOAD_LIBRARY_SEARCH_USER_DIRS = 0x00000400,

            /// <summary>
            /// LOAD_WITH_ALTERED_SEARCH_PATH
            /// </summary>
            LOAD_WITH_ALTERED_SEARCH_PATH = 0x00000008
        }
    }
    public abstract class MProcess
    {
        [DllImport("kernel32", EntryPoint = "GetModuleHandleW")]
        public static extern int GetModuleHandle(string lpModuleName);
        [DllImport("kernel32.dll", EntryPoint = "ReadProcessMemory")]
        public static extern int _MemoryReadByteSet(int hProcess, int lpBaseAddress, byte[] lpBuffer, int nSize, int lpNumberOfBytesRead);

        [DllImport("kernel32.dll", EntryPoint = "ReadProcessMemory")]
        public static extern int _MemoryReadInt32(int hProcess, int lpBaseAddress, ref int lpBuffer, int nSize, int lpNumberOfBytesRead);

        [DllImport("kernel32.dll", EntryPoint = "WriteProcessMemory")]
        public static extern int _MemoryWriteByteSet(int hProcess, int lpBaseAddress, byte[] lpBuffer, int nSize, int lpNumberOfBytesWritten);

        [DllImport("kernel32.dll", EntryPoint = "WriteProcessMemory")]
        public static extern int _MemoryWriteInt32(int hProcess, int lpBaseAddress, ref int lpBuffer, int nSize, int lpNumberOfBytesWritten);

        [DllImport("kernel32.dll", EntryPoint = "GetCurrentProcess")]
        public static extern int GetCurrentProcess();

        [DllImport("kernel32.dll", EntryPoint = "OpenProcess")]
        public static extern int OpenProcess(int dwDesiredAccess, int bInheritHandle, int dwProcessId);

        [DllImport("kernel32.dll", EntryPoint = "CloseHandle")]
        public static extern int CloseHandle(int hObject);

        [DllImport("kernel32.dll", EntryPoint = "RtlMoveMemory")]
        public static extern int _CopyMemory_ByteSet_Float(ref float item, ref byte source, int length);


        const int PROCESS_POWER_MAX = 2035711;




        /// <summary>
        /// 读内存整数型
        /// </summary>
        /// <param name="pID">进程ID</param>
        /// <param name="bAddress">0x地址</param>
        /// <returns>0失败</returns>
        public static int ReadMemoryInt32(int pID, int bAddress)
        {
            int num = 0;
            int handle = GetProcessHandle(pID);
            int num3 = MProcess._MemoryReadInt32(handle, bAddress, ref num, 4, 0);
            MProcess.CloseHandle(handle);
            if (num3 == 0)
            {
                return 0;
            }
            else
            {
                return num;
            }
        }

        /// <summary>
        /// 写内存整数型
        /// </summary>
        /// <param name="pID">进程ID</param>
        /// <param name="bAddress">0x地址</param>
        /// <param name="value">写入值</param>
        /// <returns>false失败 true成功</returns>
        public static bool WriteMemoryInt32(int pID, int bAddress, int value)
        {
            int handle = GetProcessHandle(pID);
            int num2 = MProcess._MemoryWriteInt32(handle, bAddress, ref value, 4, 0);
            MProcess.CloseHandle(handle);
            return num2 != 0;
        }

        /// <summary>
        /// 读内存小数型
        /// </summary>
        /// <param name="pID">进程ID</param>
        /// <param name="bAddress">0x地址</param>
        /// <returns>0失败</returns>
        public static float ReadMemoryFloat(int pID, int bAddress)
        {
            //byte[] array = MProcess.GetVoidByteSet(4);
            byte[] array = new byte[4];//不取空字节集也可以正确转换成单精度小数型
            int handle = GetProcessHandle(pID);
            int temp = MProcess._MemoryReadByteSet(handle, bAddress, array, 4, 0);
            if (temp == 0)
            {
                return 0f;
            }
            else
            {
                return MProcess.GetFloatFromByteSet(array, 0);
            }
        }

        /// <summary>
        /// 写内存小数型
        /// </summary>
        /// <param name="pID">进程ID</param>
        /// <param name="bAddress">0x地址</param>
        /// <param name="value">写入数据</param>
        /// <returns>false失败</returns>
        public static bool WriteMemoryFloat(int pID, int bAddress, float value)
        {
            //byte[] byteSet = MProcess.GetByteSet(value);
            byte[] byteSet = BitConverter.GetBytes(value);//https://msdn.microsoft.com/en-us/library/yhwsaf3w
                                                          //byte[] byteSet = Encoding.GetEncoding("gb2312").GetBytes(value.ToString());
            return MProcess.WriteMemoryByteSet(pID, bAddress, byteSet, 0);
        }

        /// <summary>
        /// 写内存字节集
        /// </summary>
        /// <param name="pID">进程ID</param>
        /// <param name="bAddress">0x地址</param>
        /// <param name="value">字节数据</param>
        /// <param name="length">写入长度 0代表字节数据的长度</param>
        /// <returns>false失败</returns>
        private static bool WriteMemoryByteSet(int pID, int bAddress, byte[] value, int length = 0)
        {
            int handle = MProcess.GetProcessHandle(pID);
            int nSize = (length == 0) ? value.Length : length;
            int tmp = MProcess._MemoryWriteByteSet(handle, bAddress, value, nSize, 0);//byte[]属于引用类型 引用类型不用ref也是以传址方式进行运算
                                                                                      //MProcess.CloseHandle(pID);
            return tmp != 0;
        }

        /// <summary>
        /// 取空白字节集
        /// </summary>
        /// <param name="num"></param>
        /// <returns></returns>
        public static byte[] GetVoidByteSet(int num)
        {
            if (num <= 0)
            {
                num = 1;
            }
            string text = "";
            for (int i = 0; i < num; i++)
            {
                text += "0";
            }
            return Encoding.UTF8.GetBytes(text);
        }

        /// <summary>
        /// 取进程句柄
        /// </summary>
        /// <param name="pID">进程ID</param>
        /// <returns>进程句柄</returns>
        public static int GetProcessHandle(int pID)
        {
            if (pID == -1)
            {
                return MProcess.GetCurrentProcess();
            }
            else
            {
                return MProcess.OpenProcess(PROCESS_POWER_MAX, 0, pID);
            }
        }

        /// <summary>
        /// 字节集转小数型
        /// </summary>
        /// <param name="sourceValue">字节集</param>
        /// <param name="index">索引</param>
        /// <returns></returns>
        public static float GetFloatFromByteSet(byte[] sourceValue, int index)
        {
            float result = 0f;
            MProcess._CopyMemory_ByteSet_Float(ref result, ref sourceValue[index], 4);
            return result;
        }

        /// <summary>
        /// 获取字节集
        /// </summary>
        /// <param name="data">需要转换到字节集的数据</param>
        /// <returns></returns>
        public static byte[] GetByteSet(float data)
        {
            return Encoding.UTF8.GetBytes(data.ToString());
        }
    }
}
