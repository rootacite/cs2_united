﻿#pragma checksum "..\..\..\Advance.xaml" "{8829d00f-11b8-4213-878b-770e8597ac16}" "4086118BC3A2ED0FEBC5AB9C32B57D03818AE03DF764A269847A893F690582F4"
//------------------------------------------------------------------------------
// <auto-generated>
//     此代码由工具生成。
//     运行时版本:4.0.30319.42000
//
//     对此文件的更改可能会导致不正确的行为，并且如果
//     重新生成代码，这些更改将会丢失。
// </auto-generated>
//------------------------------------------------------------------------------

using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Media.TextFormatting;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Shell;
using cs2_chs;


namespace cs2_chs {
    
    
    /// <summary>
    /// Advance
    /// </summary>
    public partial class Advance : System.Windows.Window, System.Windows.Markup.IComponentConnector {
        
        
        #line 15 "..\..\..\Advance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox StartUpEdit;
        
        #line default
        #line hidden
        
        
        #line 23 "..\..\..\Advance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox AddressEdit;
        
        #line default
        #line hidden
        
        
        #line 24 "..\..\..\Advance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.CheckBox EnvioMode;
        
        #line default
        #line hidden
        
        
        #line 25 "..\..\..\Advance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.RadioButton OM_GPY;
        
        #line default
        #line hidden
        
        
        #line 26 "..\..\..\Advance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.RadioButton OM_TOT;
        
        #line default
        #line hidden
        
        
        #line 27 "..\..\..\Advance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox OutPutLog;
        
        #line default
        #line hidden
        
        
        #line 37 "..\..\..\Advance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox IDnPut;
        
        #line default
        #line hidden
        
        
        #line 38 "..\..\..\Advance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox TRAN_OPT;
        
        #line default
        #line hidden
        
        
        #line 47 "..\..\..\Advance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.CheckBox REASE;
        
        #line default
        #line hidden
        
        
        #line 48 "..\..\..\Advance.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.CheckBox ENREP;
        
        #line default
        #line hidden
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Uri resourceLocater = new System.Uri("/cs2_chs;component/advance.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\..\Advance.xaml"
            System.Windows.Application.LoadComponent(this, resourceLocater);
            
            #line default
            #line hidden
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
        [System.ComponentModel.EditorBrowsableAttribute(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Design", "CA1033:InterfaceMethodsShouldBeCallableByChildTypes")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Maintainability", "CA1502:AvoidExcessiveComplexity")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1800:DoNotCastUnnecessarily")]
        void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target) {
            switch (connectionId)
            {
            case 1:
            
            #line 8 "..\..\..\Advance.xaml"
            ((cs2_chs.Advance)(target)).MouseMove += new System.Windows.Input.MouseEventHandler(this.Window_MouseMove);
            
            #line default
            #line hidden
            return;
            case 2:
            
            #line 9 "..\..\..\Advance.xaml"
            ((System.Windows.Controls.Grid)(target)).MouseWheel += new System.Windows.Input.MouseWheelEventHandler(this.Grid_MouseWheel);
            
            #line default
            #line hidden
            return;
            case 3:
            this.StartUpEdit = ((System.Windows.Controls.TextBox)(target));
            
            #line 15 "..\..\..\Advance.xaml"
            this.StartUpEdit.TextChanged += new System.Windows.Controls.TextChangedEventHandler(this.StartUpEdit_TextChanged);
            
            #line default
            #line hidden
            return;
            case 4:
            
            #line 17 "..\..\..\Advance.xaml"
            ((System.Windows.Controls.Button)(target)).Click += new System.Windows.RoutedEventHandler(this.Button_Click);
            
            #line default
            #line hidden
            return;
            case 5:
            this.AddressEdit = ((System.Windows.Controls.TextBox)(target));
            
            #line 23 "..\..\..\Advance.xaml"
            this.AddressEdit.TextChanged += new System.Windows.Controls.TextChangedEventHandler(this.AddressEdit_TextChanged);
            
            #line default
            #line hidden
            return;
            case 6:
            this.EnvioMode = ((System.Windows.Controls.CheckBox)(target));
            
            #line 24 "..\..\..\Advance.xaml"
            this.EnvioMode.Click += new System.Windows.RoutedEventHandler(this.EnvioMode_Click);
            
            #line default
            #line hidden
            return;
            case 7:
            this.OM_GPY = ((System.Windows.Controls.RadioButton)(target));
            
            #line 25 "..\..\..\Advance.xaml"
            this.OM_GPY.Checked += new System.Windows.RoutedEventHandler(this.OM_GPY_Checked);
            
            #line default
            #line hidden
            return;
            case 8:
            this.OM_TOT = ((System.Windows.Controls.RadioButton)(target));
            
            #line 26 "..\..\..\Advance.xaml"
            this.OM_TOT.Checked += new System.Windows.RoutedEventHandler(this.OM_TOT_Checked);
            
            #line default
            #line hidden
            return;
            case 9:
            this.OutPutLog = ((System.Windows.Controls.TextBox)(target));
            return;
            case 10:
            this.IDnPut = ((System.Windows.Controls.TextBox)(target));
            return;
            case 11:
            this.TRAN_OPT = ((System.Windows.Controls.TextBox)(target));
            return;
            case 12:
            this.REASE = ((System.Windows.Controls.CheckBox)(target));
            
            #line 47 "..\..\..\Advance.xaml"
            this.REASE.Checked += new System.Windows.RoutedEventHandler(this.CheckBox_Checked);
            
            #line default
            #line hidden
            
            #line 47 "..\..\..\Advance.xaml"
            this.REASE.Unchecked += new System.Windows.RoutedEventHandler(this.REASE_Unchecked);
            
            #line default
            #line hidden
            return;
            case 13:
            this.ENREP = ((System.Windows.Controls.CheckBox)(target));
            
            #line 48 "..\..\..\Advance.xaml"
            this.ENREP.Checked += new System.Windows.RoutedEventHandler(this.ENREP_Checked);
            
            #line default
            #line hidden
            
            #line 48 "..\..\..\Advance.xaml"
            this.ENREP.Unchecked += new System.Windows.RoutedEventHandler(this.ENREP_Unchecked);
            
            #line default
            #line hidden
            return;
            }
            this._contentLoaded = true;
        }
    }
}

