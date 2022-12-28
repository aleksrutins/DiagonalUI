using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Controls.Primitives;
using Microsoft.UI.Xaml.Data;
using Microsoft.UI.Xaml.Input;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Navigation;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.Json;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.
using DiagonalUI.Logging;
using System.Collections.ObjectModel;

namespace DiagonalUI
{
    class Test
    {
        public string Name { get; set; }
        public bool IsRunning { get; set; }
        public bool HasRun { get; set; }
        public bool Success { get; set; }
        public Visibility ShowSuccess { get
            {
                return HasRun && Success ? Visibility.Visible : Visibility.Collapsed;
            } }
        public Visibility ShowFailure { get
            {
                return HasRun && !Success ? Visibility.Visible : Visibility.Collapsed;
            } }
        public Visibility ShowProgress { get
            {
                return !HasRun ? Visibility.Visible : Visibility.Collapsed;
            } }
    }

    /// <summary>
    /// An empty window that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainWindow : Window
    {
        readonly LogParser logParser = new("diagonal");
        readonly ObservableCollection<string> outputLines = new();
        readonly ObservableCollection<Test> tests = new();
        string CreateLine(string message, string data)
        {
            return $"/* log:tag:diagonal */ {{\"Message\": \"{message}\", \"Data\": {data}}}";
        }
        public MainWindow()
        {
            this.InitializeComponent();
            Output.ItemsSource = outputLines;
            TestList.ItemsSource = tests;
            logParser.AddHandler("logLine", text =>
            {
                outputLines.Add(text.ToString());
            });
            logParser.AddHandler("clearTests", _ =>
            {
                tests.Clear();
            });
            logParser.AddHandler("enumerateTest", name =>
            {
                tests.Add(new Test
                {
                    Name = name.ToString(),
                    IsRunning = false,
                    HasRun = false,
                    Success = false
                });
            });
            logParser.AddHandler("testStart", name =>
            {
                var test = tests.Where(t => t.Name == name.ToString()).First();
                var index = tests.IndexOf(test);
                tests.Remove(test);
                test.IsRunning = true;
                tests.Insert(index, test);
                outputLines.Add("** TEST START: " + name.ToString() + " **");
            });
            logParser.AddHandler("statusReport", data =>
            {
                var el = (JsonElement)data;
                var name = el.GetProperty("name").ToString();
                var success = el.GetProperty("success").GetBoolean();
                var test = tests.Where(t => t.Name == name.ToString()).First();
                var index = tests.IndexOf(test);
                tests.Remove(test);
                test.IsRunning = false;
                test.HasRun = true;
                test.Success = success;
                tests.Insert(index, test);
                outputLines.Add("** TEST FINISHED: " + name + " | STATUS: " + (success ? "Success" : "Failure") + " **");
            });
            logParser.ProcessLine(CreateLine("enumerateTest", "\"hello\""));
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            logParser.ProcessLine(CreateLine("testStart", "\"hello\""));
            logParser.ProcessLine(CreateLine("statusReport", "{\"name\": \"hello\", \"success\": true}"));
        }
    }
}
