   M S B A M L   `   `   `  ����B  >cs2_chs, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null!clr-namespace:cs2_chscs2_chs  5   	       cs2_chs.MainWindow   5   	   W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   'localclr-namespace:cs2_chs     5   	     � Title$  Text Controler��5   	   $	��200��6    $	��600��6-    � 
ResizeMode$ NoResize=�69   $y�Visible=�6O    � UseLayoutRounding .  .��5	      � -   5         � ColumnDefinitions 5
   
   �� 5      $	��74* �6   �� 5      $
��223* �6   ��5   
   �� 5       �� ImageSource$ 01.jpg��6   .+�5   
   �� -   5         ��Name$ 
TEXT_INPUT��5      $
��Left=�6&   $	��120��6A   $��	10,10,0,0q�6N    �� TextWrapping$
 Wrap=�6a   $��TextBox��6u   $	��Top=�6�   $	��574��6�    �� AcceptsReturn . $��2��6�   .��6�   ��5      �� 5       �� EndPoint$ 0.5,1&�6&   	 �� 
StartPoint$	 0.5,0&�67   .��5      � 
 � Color$
 	#FFFFB700��6#    � Offset$ 0.027Q�65   � 5      $
 	#FFFF9A9A��6#   $ 1Q�65   5      .��5   #   ��5      �� 5      $ 0.5,1&�6&   $	 0.5,0&�67   .��5      � $
 Black��6#   $ 0Q�61   � 5      $
 	#FFD8FFFB��6#   $ 1Q�65   � 5      $
 	#99FFFFFF��6#   $ 0Q�65   5      .��5   "   5      �� 5    
   -   5         ��Name$ apply��5       $��Apply��6!   $
��Left=�61   $��25��6L   $��348.267,135,0,0q�6X   $	��Top=�6q   $��88��6�   $��1��6�   ��5!      �� 5"      $ 0.5,1&�6&   $	 0.5,0&�67   .��5#      � $	
 Red��6#   $ 0Q�6/   � 5$      $
 	#7FFFBBBB��6#   $
 0.99Q�65   5%      5'      � 5(   
   -   5         �Name$	 PBS��5(      $
��Left=�6#   $��25��6>   $��103,135,0,0q�6J   $	��Top=�6_   $	��388��6w   $y�	Collapsed=�6�   $��2��6�   �� 5)   
   -   5        $��Show Japanese��5)      $
��Left=�6)   $��
10,135,0,0q�6D   $	��Top=�6X   $��88��6p   $��25��6{   ��5*      �� 5+      $ 0.5,1&�6&   $	 0.5,0&�67   .��5,      � $
 	#FFFFEE00��6#   $ 0Q�65   � 5-      $
 	#8CFFFAB3��6#   $ 1Q�65   5.      50      51       � 	Resources 52         ShowMainWindow �� Text��%2   &	        �� 53   
   $ ShowMainWindow��61   54       J� InputBindings 55      �� 56   
    �� 	Modifiers$	 Altf�6   
 �� Key$ A��6%    �� Command# [  6-    J� CommandBindings 58      )  #System.Windows.Input.CommandBinding  59   
   -   5           Command# [  59      5;      5=      