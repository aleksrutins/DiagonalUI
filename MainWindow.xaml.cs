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
using System.Threading;
using System.Text.Json;
using System.Diagnostics;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.
using DiagonalUI.Logging;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using Windows.UI.Core;
using System.Runtime.CompilerServices;
using Microsoft.UI.Dispatching;

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
        readonly LogParser LogParser = new("diagonal");
        readonly ObservableCollection<string> OutputLines = new();
        readonly ObservableCollection<Test> Tests = new();
        readonly DispatcherQueue dispatcherQueue = DispatcherQueue.GetForCurrentThread();
        string CreateLine(string message, string data)
        {
            return $"/* log:tag:diagonal */ {{\"Message\": \"{message}\", \"Data\": {data}}}";
        }
        void RunAsync(DispatcherQueueHandler cb)
        {
            dispatcherQueue.TryEnqueue(DispatcherQueuePriority.Normal, cb);
        }
        public MainWindow()
        {
            this.InitializeComponent();
            Output.ItemsSource = OutputLines;
            TestList.ItemsSource = Tests;
            LogParser.AddHandler("logLine", text =>
            {
                OutputLines.Add(text.ToString());
            });
            LogParser.AddHandler("clearTests", _ =>
            {
                Tests.Clear();
            });
            LogParser.AddHandler("enumerateTest", name =>
            {
                Tests.Add(new Test
                {
                    Name = name.ToString(),
                    IsRunning = false,
                    HasRun = false,
                    Success = false
                });
            });
            LogParser.AddHandler("testStart", name =>
            {
                var test = Tests.Where(t => t.Name == name.ToString()).First();
                var index = Tests.IndexOf(test);
                Tests.Remove(test);
                test.IsRunning = true;
                Tests.Insert(index, test);
                OutputLines.Add("** TEST START: " + name.ToString() + " **");
            });
            LogParser.AddHandler("statusReport", data =>
            {
                var el = (JsonElement)data;
                var name = el.GetProperty("name").ToString();
                var success = el.GetProperty("success").GetBoolean();
                var test = Tests.Where(t => t.Name == name.ToString()).First();
                var index = Tests.IndexOf(test);
                Tests.Remove(test);
                test.IsRunning = false;
                test.HasRun = true;
                test.Success = success;
                Tests.Insert(index, test);
                OutputLines.Add("** TEST FINISHED: " + name + " | STATUS: " + (success ? "Success" : "Failure") + " **");
            });
        }
        private void LogcatButton_Click(object sender, RoutedEventArgs e)
        {
            var startInfo = new ProcessStartInfo
            {
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                FileName = AdbPath.Text,
                Arguments = "logcat",
                UseShellExecute = false,
                CreateNoWindow = true,
            };
            var proc = new Process { StartInfo = startInfo };
            proc.OutputDataReceived += ProcessLogcatLine;
            proc.ErrorDataReceived += ProcessLogcatLine;
            proc.Start();
            proc.BeginOutputReadLine();
            proc.BeginErrorReadLine();
            LogcatButton.Visibility = Visibility.Collapsed;
            AdbPath.Visibility = Visibility.Collapsed;
        }

        private void ProcessLogcatLine(object sender, DataReceivedEventArgs e)
        {
            RunAsync(() => LogParser.ProcessLine(e.Data));
        }
    }
}
